#pragma once

#include "config.h"

#include <functional>
#include <string>

class config_persistence {
  char const * CONFIG_FILENAME = "cg3lz_config.json";
  typedef std::function<void(std::string const&)> log_t;
  log_t log;

public:
  void configure_logging(log_t l);
  config load();
  void save(config &);
};