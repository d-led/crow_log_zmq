#include "zeromq_log_source.h"

#include <string>
#include <chrono>
#include <thread>

// zeromq
#include <zmq.hpp>

struct zeromq_log_source::impl {
  unsigned int zeromq_log_port;
  default_log_t log_sink;
  zmq::context_t context;
  zmq::socket_t pull;
  std::atomic<bool> started;
  std::thread loop;

  impl(unsigned int zp, default_log_t sink)
      : zeromq_log_port(zp),
        log_sink(sink),
        context(1),
        pull(context, ZMQ_PULL),
        started(false) {}

  ~impl() {
    loop.join();
  }
};

zeromq_log_source::zeromq_log_source(unsigned int zp, default_log_t default_log,
                                 default_log_t sink)
    : log(default_log), pimpl(new impl(zp, sink)) {

  std::string port = std::to_string(pimpl->zeromq_log_port);
  std::string socket_config = "tcp://*:";
  socket_config += port;
  pimpl->pull.bind(socket_config.c_str());

  log(std::string("Listening to 0mq incoming logs on: ") + socket_config +
      "\n");
}

zeromq_log_source::~zeromq_log_source() { this->stop(); }

void zeromq_log_source::start_once() {
  if (pimpl->started) return;

  pimpl->loop = std::move(std::thread([this] {
                pimpl->started = true;
                while (pimpl->started) {
                  zmq::message_t request;
                  pimpl->pull.recv(&request);
                  std::string log_line(static_cast<char*>(request.data()),
                                       request.size());
                  pimpl->log_sink(log_line);
                }
              }));
}

void zeromq_log_source::stop() { pimpl->started = false; }
