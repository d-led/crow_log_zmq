#include "spdlogger.h"

#include <boost/filesystem.hpp>

#include <spdlog/spdlog.h>

namespace spd = spdlog;

struct spdlogger::impl {
  std::shared_ptr<spd::logger> log;

  // 500mb max / 10 files max
  impl(std::string name, std::string path) {
        const size_t q_size = 8192;
        spd::set_async_mode(q_size, spdlog::async_overflow_policy::block_retry,
                       nullptr,
                       std::chrono::seconds(2));
        log = spd::rotating_logger_mt("rotating_log", path + "/log",
                                    1048576 * 500, 10);
      }

  ~impl() {
    spd::drop("rotating_log");
  }
};

spdlogger::spdlogger(std::string name, std::string path) {
  boost::filesystem::path dir(path);
  boost::filesystem::create_directory(dir);
  pimpl.reset(new impl(name, path));
}

spdlogger::~spdlogger() {}

void spdlogger::log(std::string const& message) { pimpl->log->info(message); }
