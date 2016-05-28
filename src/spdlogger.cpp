#include "spdlogger.h"

#include <boost/filesystem.hpp>

#include <spdlog/spdlog.h>

namespace spd = spdlog;

struct spdlogger::impl {
  std::shared_ptr<spd::logger> log;

  // 500mb max / 10 files max
  impl(std::string name, std::string path)
      : log(spd::rotating_logger_mt("rotating_log", path + "/log",
                                    1048576 * 500, 10)) {}
};

spdlogger::spdlogger(std::string name, std::string path) {
  boost::filesystem::path dir(path);
  boost::filesystem::create_directory(dir);
  pimpl.reset(new impl(name, path));
}

spdlogger::~spdlogger() {}

void spdlogger::log(std::string const& message) { pimpl->log->info(message); }
