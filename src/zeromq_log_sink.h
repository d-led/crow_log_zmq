#pragma once

#include "config.h"
#include "g3logger.h"

#include <atomic>
#include <chrono>
#include <thread>
#include <memory>
#include <functional>

class zeromq_log_sink {
  typedef std::function<void(std::string const&)> default_log_t;
  default_log_t log;
  struct impl;

  std::unique_ptr<impl> pimpl;
 public:
  zeromq_log_sink(config& c, default_log_t default_log, g3logLogger& sink);
  ~zeromq_log_sink();

 public:
  void start_once();
  void stop();

};
