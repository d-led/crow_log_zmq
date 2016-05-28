#pragma once

#include <memory>
#include <string>
#include <functional>

class spdlogger {
  struct impl;
  std::unique_ptr<impl> pimpl;
  typedef std::function<void()> tick_t;
  tick_t tick;

 public:
   spdlogger(std::string name, std::string path, tick_t t);
  ~spdlogger();

 public:
  void log(std::string const& message);
};
