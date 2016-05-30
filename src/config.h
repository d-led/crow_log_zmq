#pragma once

#include <string>

struct config {
  bool logging = true;
  int port = 18080;
  int zeromq_log_port = 18090;
  std::string log_path = "logs/";
  int64_t max_file_size = 1048576 * 100; // ~100mb
  int64_t max_number_of_files = 10;
  int flush_period_seconds = 2; // flush log files ever N seconds (performance vs. fidelity)
};
