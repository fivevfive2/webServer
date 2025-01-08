
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <memory>
#include "mysql/jdbc.h"
namespace bre{
    class Sqlconnpool{
        public:
            std::unique_lock<sql::Connection> Getinstance(){
                
            }

    };
};
