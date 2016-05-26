#pragma once

#include "config.h"

#include <vector>
#include <string>

class log_view {
  config& cfg;

public:
  log_view(config& c):cfg(c) {}

public:
  std::vector<std::string> get_logs() const;
};