//
// Created by cheng on 2022/2/18.
//

#include <iostream>
#include <chrono>
#include "core/Thread.h"
#include "log/Log.h"

using namespace std;
using namespace lite_http;

int main() {
    {
        ThreadPool threadPool(1);
        threadPool.addTask([](){ cout << "No.1 process." << endl; });
        threadPool.addTask([](){ cout << "No.2 process." << endl; });
        threadPool.addTask([](){ cout << "No.3 process." << endl; });
        threadPool.addTask([](){ cout << "No.4 process." << endl; });
        threadPool.addTask([](){ cout << "No.5 process." << endl; });
        threadPool.addTask([](){ cout << "No.6 process." << endl; });
        threadPool.addTask([](){ cout << "No.7 process." << endl; });
        std::this_thread::sleep_for(std::chrono::seconds (1));
    }
    
    return 0;
}
