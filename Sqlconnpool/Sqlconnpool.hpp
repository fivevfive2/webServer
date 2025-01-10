#include "../config/Config.hpp"
#include "mysql/jdbc.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
namespace bre {
    class MySqlPool {
    public:
        static MySqlPool &Instance() {

            static MySqlPool instance;
            static std::once_flag flag;
            std::call_once(flag, [&]() {
                try {
                    auto &config = bre::Config::GetInstance();
                    std::string url = config.Get("DB_URL").value();
                    std::string user = config.Get("USER").value();
                    std::string pass = config.Get("PASS").value();
                    std::string schema = config.Get("SCHEME").value();
                    int poolSize = std::stoi(config.Get("POOL_SIZE").value_or("8"));
                    instance.init(url, user, pass, schema, poolSize);
                } catch (const std::exception &e) {
                    std::cerr << "sql read config has no value: " << e.what() << '\n';
                    throw;
                }
            });
            return instance;
        }
        MySqlPool() = default;
        void init(const std::string &Url, const std::string &User, const std::string &Pass,
                  const std::string &Schema, int PoolSize = 8) {
            url = Url;
            user = User;
            pass = Pass;
            schema = Schema;
            poolSize = PoolSize;
            b_stop = false;
            try {
                for (int i = 0; i < poolSize; i++) {
                    sql::mysql::MySQL_Driver *driver = sql::mysql::get_driver_instance();
                    sql::Connection *con = driver->connect(url.c_str(), user.c_str(), pass.c_str());
                    con->setSchema(schema);
                    pool.emplace(con);
                }
            } catch (sql::SQLException &e) {

                std::cout << "sql::SQLException, error is " << e.what() << std::endl;
            } catch (std::exception &e) {

                std::cout << "mysql pool init failed, error is " << e.what() << std::endl;
            }
        }

        std::unique_ptr<sql::Connection> GetConn()

        {
            std::unique_lock<std::mutex> lock(mux);
            cond.wait(lock, [this] {
                          if(b_stop == true){
                              return true;
                          }
                          return !pool.empty(); });
            if (b_stop)
                return nullptr;
            std::unique_ptr<sql::Connection> con(std::move(pool.front()));
            pool.pop();
            return con;
        }

        void FreeConn(std::unique_ptr<sql::Connection> con) {
           {
             std::lock_guard<std::mutex> lock(mux);
            if (b_stop) {
                return;
            }
            pool.push(std::move(con));
           }
            cond.notify_one();
        }

        void Close() {
            b_stop = true;
            cond.notify_all();
        }

        ~MySqlPool() {
            std::lock_guard<std::mutex> lock(mux);
            if (!b_stop) {
                Close();
            }
            while (!pool.empty()) {
                pool.pop();
            }
        }

    private:
        void checkConnection() {
        }

    private:
        std::string url;
        std::string user;
        std::string pass;
        std::string schema;
        int poolSize;
        std::queue<std::unique_ptr<sql::Connection>> pool;
        std::mutex mux;
        std::condition_variable cond;
        std::atomic<bool> b_stop;
    };
};// namespace bre
