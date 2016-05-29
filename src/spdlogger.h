#pragma once

#include <memory>
#include <string>
#include <functional>

class spdlogger {
  struct impl;
  std::unique_ptr<impl> pimpl;

 public:
   spdlogger(std::string name, std::string path);
  ~spdlogger();

 public:
  void log(std::string const& message);
  void shutdown();
};
