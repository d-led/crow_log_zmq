#include "mal_logger.h"

#include <boost/filesystem.hpp>

#include <mal_log/extras/boost_filesystem_list_all_files.hpp>
#include <mal_log/mal_log.hpp>

using namespace mal;

struct mal_logger::impl {
  frontend fe;
  backend_cfg be_cfg;

  impl(std::string name, std::string path):fe(),be_cfg(fe.get_backend_cfg()) {
    be_cfg.file.out_folder = path;
    be_cfg.file.aprox_size = 500000 * 1024;
    be_cfg.file.rotation.file_count = 10;
    be_cfg.file.rotation.delayed_file_count = 1;
    be_cfg.file.rotation.past_files = extras::list_all_files(path.c_str());
    if (fe.init_backend(be_cfg) != frontend::init_ok)
      throw std::exception("mal_backend initialization failed");
  }
};

mal_logger::mal_logger(std::string name, std::string path, tick_t t)
    : pimpl(new impl(name,path)), tick(t) {
  boost::filesystem::path dir(path);
  boost::filesystem::create_directory(dir);
}

mal_logger::~mal_logger() {
  try {
    pimpl->fe.on_termination();
  }
  catch (...) {}
}

void mal_logger::log(std::string const& message) {
  log_debug_i(pimpl->fe, "%s", message.c_str());
  tick();
}
