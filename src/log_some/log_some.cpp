//i.e. g++ zmqclient.cpp -lzmq -o zmqclient -O3 -std=c++11
#include "../common/config.h"

#include <zmq.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>
#include <cstdio>

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char* argv[]) {
    std::atomic<bool> slow(true);

    std::string server = (argc == 1) ?
        "localhost"
        :
        argv[1]
        ;

    const std::string prefix(1024, 'X');

    auto ticker = std::thread([&slow, server] {
        config cfg;
        zmq::context_t context(1);
        zmq::socket_t push(context, ZMQ_DEALER);
        auto count = 100000;
        unsigned long long total = 0;

        auto connection_string = std::string("tcp://") + server + ":" + std::to_string(cfg.zeromq_log_port);
        std::cout << "connecting to " << connection_string << std::endl;
        push.connect(connection_string.c_str());
        while (true) {
            auto t1 = Clock::now();

            for (auto i = 0; i < count; i++) {
                //std::string msg(prefix + std::to_string(total));

                if (slow)
                    std::this_thread::sleep_for(std::chrono::milliseconds(600));

                std::string msg(std::to_string(total + i));
                zmq::message_t hi(msg.length());
                memcpy(hi.data(), msg.data(), msg.length());
                push.send(hi);
            }

            total += count;
            auto t2 = Clock::now();
            auto diff = t2 - t1;
            auto count_per_second = static_cast<double>(count) / (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() / 1000.);
            std::cout << count_per_second << "/s, total: " << total << std::endl;
        }

        return 0;
    });

    while (true) {
        std::getchar();
        slow = !slow;
    }
}
