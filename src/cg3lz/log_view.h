#pragma once

#include <vector>
#include <string>

struct file_entry {
  std::string filename;
  size_t size;
};

class log_view {
  const std::string log_path;

public:
  log_view(std::string const& p):log_path(p) {}

public:
  std::vector<file_entry> get_logs() const;
  void delete_logs() const;
  std::string path_for_filename(std::string const& filename) const;
};
