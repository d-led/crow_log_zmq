#pragma once

#include "config.h"

#include <vector>
#include <string>

struct file_entry {
  std::string filename;
  size_t size;
};

class log_view {
  config& cfg;

public:
  log_view(config& c):cfg(c) {}

public:
  std::vector<file_entry> get_logs() const;
};
