#pragma once

#include <crow.h>
#include <string>


class DefaultLogger : public crow::ILogHandler {
  bool& logging;

 public:
  DefaultLogger(bool& l) : logging(l) {}

  inline void log(std::string message, crow::LogLevel level) override {
    if (logging)
    	std::cout << "cg3lz " << message;
  }
};
