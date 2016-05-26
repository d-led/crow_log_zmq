#include "zeromq_log_sink.h"
#include "g3logger.h"

#include <string>
#include <chrono>
#include <thread>

// zeromq
#include <zmq.hpp>


struct zeromq_log_sink::impl {
  config& cfg;
  g3logLogger& log_sink;
  zmq::context_t context;
  zmq::socket_t pull;
  std::atomic<bool> started;

  impl(config& c, g3logLogger& sink)
      : cfg(c),
        log_sink(sink),
        context(1),
        pull(context, ZMQ_PULL),
        started(false) {}
};

zeromq_log_sink::zeromq_log_sink(config& c, default_log_t default_log,
                                 g3logLogger& sink)
    : log(default_log), pimpl(new impl(c, sink)) {

  std::string port = std::to_string(pimpl->cfg.zeromq_log_port);
  std::string socket_config = "tcp://*:";
  socket_config += port;
  pimpl->pull.bind(socket_config.c_str());

  log(std::string("Listening to 0mq incoming logs on: ") + socket_config +
      "\n");
}

zeromq_log_sink::~zeromq_log_sink() { this->stop(); }

void zeromq_log_sink::start_once() {
  if (pimpl->started) return;

  std::thread([this] {
                while (pimpl->started) {
                  zmq::message_t request;
                  pimpl->pull.recv(&request);
                  std::string log_line(static_cast<char*>(request.data()),
                                       request.size());
                  pimpl->log_sink.log(log_line);
                }
              }).detach();

  pimpl->started = true;
}

void zeromq_log_sink::stop() { pimpl->started = false; }
