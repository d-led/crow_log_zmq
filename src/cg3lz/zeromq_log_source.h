#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include <memory>
#include <functional>

namespace zmq { class context_t; }

class zeromq_log_source {
  typedef std::function<void(std::string const&)> default_log_t;
  default_log_t log;
  default_log_t sink;
  struct impl;

  std::unique_ptr<impl> pimpl;
 public:
  zeromq_log_source(zmq::context_t& context, unsigned int zmq_port);
  ~zeromq_log_source();

 public:
  void configure_logging(default_log_t default_log, default_log_t sink);
  void start_once();
  void stop();

};
