#include "zeromq_log_source.h"

#include <string>
#include <chrono>
#include <thread>

// zeromq
#include <zmq.hpp>
#include <zmq_addon.hpp>

struct zeromq_log_source::impl {
  unsigned int zeromq_log_port;
  default_log_t log_sink;
  zmq::context_t& context;
  zmq::socket_t pull;
  std::atomic<bool> started;
  std::thread loop;

  impl(zmq::context_t& ctx, unsigned int zp)
      : zeromq_log_port(zp),
        context(ctx),
        pull(context, ZMQ_ROUTER),
        started(false) {
    pull.setsockopt(ZMQ_RCVTIMEO, 2000);
  }

  ~impl() {
      if (loop.joinable())
        loop.join();
  }
};

zeromq_log_source::zeromq_log_source(zmq::context_t& context, unsigned int zp)
    : pimpl(new impl(context,zp)) {

  std::string port = std::to_string(pimpl->zeromq_log_port);
  std::string socket_config = "tcp://*:";
  socket_config += port;
  pimpl->pull.bind(socket_config.c_str());

  if (log)
    log(std::string("Listening to 0mq incoming logs on: ") + socket_config);
}

zeromq_log_source::~zeromq_log_source() { this->stop(); }

void zeromq_log_source::configure_logging(default_log_t default_log, default_log_t sink) {
  this->log = default_log;
  this->sink = sink;
  pimpl->log_sink = sink;
}

void zeromq_log_source::start_once() {
  if (pimpl->started) return;

  pimpl->loop = std::move(std::thread([this] {
                pimpl->started = true;
                while (pimpl->started) {
                  zmq::multipart_t request(pimpl->pull);
                  if (!request.peek(0))
                    continue;
                  request.pop();
                  if (pimpl->log_sink)
                    pimpl->log_sink(request.popstr());
                }
              }));
}

void zeromq_log_source::stop() { pimpl->started = false; }
