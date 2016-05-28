#pragma once

#include <memory>
#include <string>
#include <functional>

namespace g3 { class LogWorker; }

class g3logLogger {
  std::unique_ptr<g3::LogWorker> worker;
  typedef std::function<void()> tick_t;
  tick_t tick;

 public:
  g3logLogger(std::string name, std::string path, tick_t t);
  ~g3logLogger();

 public:
  void log(std::string const& message);
};
