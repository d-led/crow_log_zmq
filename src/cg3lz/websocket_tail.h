#pragma once

#include <memory>
#include <thread>

namespace zmq { class context_t; }

class websocket_ticker {
    zmq::context_t& ctx;
    std::thread ws_loop;
    std::thread pair_loop;
    std::thread run_thread;
    struct impl;
    std::unique_ptr<impl> pimpl;

public:
    websocket_ticker(zmq::context_t&,int port);
    ~websocket_ticker();
    void run();
    void stop();
};

