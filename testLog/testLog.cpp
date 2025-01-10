
#include "Log.hpp"
#include <iostream>
#include <string>
#include <chrono>
using namespace std;
void example() {
    using namespace bre;
    auto& log = bre::Log::Instance();
    log.Init(bre::LogLevel::ERROR,1); // 设置日志级别为DEBUG
    Log::debug(std::string("Debug message: {}."), "some value");
    Log::info("Info message.");
    Log::warn("Warning message.{},{}",1,2);
    Log::err("Error message.");
}
int main() {
    cout << "================== test log! ==============" << endl;
    example();
    // for(int i = 0; i < 20; ++i)
    //     bre::debug("Error message for main.{}", "aaaa");
    return 0;
}
