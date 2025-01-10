
#include <iostream>
#include <chrono>
#include <thread>
#include "HeapTimer.hpp"

std::chrono::time_point start = std::chrono::high_resolution_clock::now();
// 任务函数
void taskFunction(std::chrono::milliseconds delay) {
    auto now = std::chrono::high_resolution_clock::now();
    std::cout << "Task started after " << delay.count() << "ms, current time is "
        << std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() << "ms \n";
}

// 测试函数
void testTaskFunction() {
    using namespace std::chrono_literals;
    // 创建一个堆定时器实例
    start = std::chrono::high_resolution_clock::now();
    bre::HeapTimer timer;
    for (int i = 1; i < 5; ++i) {
        int taskId = i;
        timer.Add(taskId, 200 * i, std::bind(&taskFunction, i * 200ms));
    }
    // 等待一段时间，确保任务有足够的时间被执行

    bre::MS sleepTime(2000);
    while (std::chrono::high_resolution_clock::now() - sleepTime < start) {
        auto t = timer.GetNextTick();
		if (t == bre::MS::max()) {
			std::cout << "No task to run, sleep for 1s\n";
			std::this_thread::sleep_for(1s);
			continue;
		}
        std::cout << "Next tick: " << t.count() << "ms\n";
		std::this_thread::sleep_for(t+1ms); // sleep linux中输入0和负数就不正常了
    }
}

int main() {
    testTaskFunction();
    return 0;
}