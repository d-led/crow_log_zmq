#pragma once

namespace zmq { class context_t; }

class websocket_ticker {
    zmq::context_t& ctx;

public:
    websocket_ticker(zmq::context_t&,int port);
};

void start_websocket_ticker();
