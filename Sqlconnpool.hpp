
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <memory>
#include "mysql/jdbc.h"
#include "Config.hpp"
#include <atomic>
namespace bre
{
    class MySqlPool
    {
    public:
        MySqlPool(const std::string &Url, const std::string &User, const std::string &Pass,
                  const std::string &Schema, int PoolSize = 8)
        {
        }

        std::unique_ptr<sql::Connection> GetConn() {}
        void FreeConn(std::unique_ptr<sql::Connection> con) {}
        void Close() {}
        ~MySqlPool() {}

    private:
        void checkConnection() {}

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
};
