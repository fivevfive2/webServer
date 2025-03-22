cmake_minimum_required(VERSION 3.20)

project(breWebserver VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出路径


# 使用仅头文件的好处是减少库和文件的依次添加
add_executable(main testSqlconnpool.cpp)

# 注意mysql-connector-c++文件路径，因为window安装使用jdbc，请根据自己的选项写入
# 网上很多资料的 mysql-connector-c++ 很老，路径很多不正确
target_include_directories(main PRIVATE /home/lwdw/mysql-connector-c++-9.1.0-linux-glibc2.28-x86-64bit/include)
find_package(Threads REQUIRED)


target_link_libraries(main PRIVATE 
   "/home/lwdw/mysql-connector-c++-9.1.0-linux-glibc2.28-x86-64bit/lib64/libmysqlcppconn.so"
    Threads::Threads
)

