#pragma once

#include <memory>
#include <string>
#include <functional>

class mal_logger {
  struct impl;
  std::unique_ptr<impl> pimpl;
  typedef std::function<void()> tick_t;
  tick_t tick;

 public:
   mal_logger(std::string name, std::string path, tick_t t);
  ~mal_logger();

 public:
  void log(std::string const& message);
};
