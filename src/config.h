#pragma once

#include <string>

struct config {
  bool logging = true;
  unsigned int port = 18080;
  unsigned int zeromq_log_port = 18090;
  std::string log_path = "logs/";
};
