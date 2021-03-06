#include "log_view.h"

#include <string>

#include <boost/filesystem.hpp>


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
        e.size = static_cast<size_t>(file_size(it->path()));
        res.emplace_back(e);
      }
    }
    return res;
  }
};

std::vector<file_entry> log_view::get_logs() const {
  return file_info(log_path).get();
}

void log_view::delete_logs() const {
  directory_iterator end;
  for (directory_iterator it(log_path); it != end; ++it) {
    try {
      remove_all(it->path());
    }
    catch (...) {
      //ignore locked or moved files
    }
  }
}

std::string log_view::path_for_filename(std::string const& filename) const {
  return (path(log_path) / filename).string();
}
