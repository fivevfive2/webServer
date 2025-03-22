#include "Config.hpp"
#include <optional>
using namespace std;
using namespace bre;
// 测试函数
void testConfig() {
    // 假设 config.txt 文件内容如下：
    // key1:value1
    // key2:value2
    Config::getInstance();
    std::cout << "Getting configuration values:" << std::endl;
    std::cout << "key1: " << Config::getInstance().Get("DB_URL").value() << std::endl;
    std::cout << "key2: " << Config::getInstance().Get("USER").value() << std::endl;
    std::cout << "key1: " << Config::getInstance().Get("PASS").value() << std::endl;
    std::cout << "key1: " << Config::getInstance().Get("SCHAME").value() << std::endl;
    std::cout << "key1: " << Config::getInstance().Get("POOL_SIZE").value() << std::endl;
  //  std::cout << "missing_key: " << Config::getInstance().Get("missing_key").value() << std::endl;
}

int main() {
    testConfig();
    return 0;
}