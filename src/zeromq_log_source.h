#pragma once

#include "g3logger.h"

#include <atomic>
#include <chrono>
#include <thread>
#include <memory>
#include <functional>

class zeromq_log_source {
  typedef std::function<void(std::string const&)> default_log_t;
  default_log_t log;
  struct impl;

  std::unique_ptr<impl> pimpl;
 public:
  zeromq_log_source(unsigned int zmq_port, default_log_t default_log, g3logLogger& sink);
  ~zeromq_log_source();

 public:
  void start_once();
  void stop();

};