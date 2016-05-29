#pragma once

#include <memory>
#include <string>
#include <functional>

class spdlogger {
  struct impl;
  std::unique_ptr<impl> pimpl;

 public:
   spdlogger(std::string name, std::string path, size_t max_file_size, size_t max_number_of_files);
  ~spdlogger();

 public:
  void log(std::string const& message);
  void shutdown();
};
