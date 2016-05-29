#pragma once

#include <string>
#include <picojson_serializer.h>

struct config {
  bool logging = true;
  unsigned int port = 18080;
  unsigned int zeromq_log_port = 18090;
  std::string log_path = "logs/";
  size_t max_file_size = 1048576 * 100; // ~100mb
  size_t max_number_of_files = 10;

  template<class Archive>
  void json(Archive & ar)
  {
    ar & picojson::convert::member("logging", logging);
    ar & picojson::convert::member("port", port);
    ar & picojson::convert::member("zeromq_log_port", zeromq_log_port);
    ar & picojson::convert::member("log_path", log_path);
    ar & picojson::convert::member("max_file_size", max_file_size);
    ar & picojson::convert::member("max_number_of_files", max_number_of_files);
  }
};
