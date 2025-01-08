#include "Config.hpp"
#include <iostream>
void configtest()
{
    std::cout << "Getting configuration values:" << std::endl;
    std::cout << "key1: " << bre::Config::GetInstance().Get("SQL_PORT").value() << std::endl;
    std::cout << "key2: " << bre::Config::GetInstance().Get("PORT").value() << std::endl;
    std::cout << "missing_key: " << bre::Config::GetInstance().Get("missing_key").value() << std::endl;
}

int main()
{
    configtest();
    return 0;
}