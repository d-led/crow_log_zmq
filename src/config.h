#pragma once

#include <string>

struct config {
  bool logging = true;
  int port = 18080;
  int zeromq_log_port = 18090;
  std::string log_path = "logs";
};
