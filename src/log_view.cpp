#include "log_view.h"

#include <string>

#include <boost/filesystem.hpp>

struct file_entry {
  std::string filename;
  size_t size;
};

using namespace boost::filesystem;

struct file_info {
  path dir;

  file_info(std::string const& path) : dir(path) {}

  std::vector<file_entry> get() const {
    std::vector<file_entry> res;
    directory_iterator end;

    for (directory_iterator it(dir); it != end; ++it) {
      if (is_regular_file(it->path())) {
        file_entry e;
        e.filename = it->path().filename().string();
        res.emplace_back(e);
      }
    }
    return res;
  }
};

std::vector<std::string> log_view::get_logs() const {
  file_info info(cfg.log_path);
  std::vector<std::string> res;
  auto logs = info.get();
  for(auto& entry: logs) {
    res.emplace_back(entry.filename);
  }
  return res;
}
