#pragma once

#include "config.h"

#include <crow_all.h>
#include <string>


class DefaultLogger : public crow::ILogHandler {
  config& cfg;

 public:
  DefaultLogger(config& c) : cfg(c) {}

  inline void log(std::string message, crow::LogLevel level) override {
    if (cfg.logging)
    	std::cout << "cg3lz " << message;
  }
};
