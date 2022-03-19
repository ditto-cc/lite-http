//
// Created by cheng on 2021/11/29.
//

#include "util/Queue.h"
#include <thread>
#include <iostream>

int main() {
    using namespace std;
    lite_http::Queue<int> q;

    thread t1([&]() {
        for (int i = 0; i < 100; i += 2)
            q.push(i);
    });

    thread t2([&]() {
        for (int i = 1; i < 100; i += 2)
            q.push(i);
    });

    t1.join();
    t2.join();

    cout << q.size() << endl;
    for (auto i = q.size(); i > 0; --i)
        cout << q.front() << ' ', q.pop();

    return 0;
}
