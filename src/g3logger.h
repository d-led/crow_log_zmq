#pragma once

#include <memory>

namespace g3 { class LogWorker; }

class g3logLogger {
  std::unique_ptr<g3::LogWorker> worker;

 public:
  g3logLogger(std::string name, std::string path);
  ~g3logLogger();

 public:
  void log(std::string const& message);
};
