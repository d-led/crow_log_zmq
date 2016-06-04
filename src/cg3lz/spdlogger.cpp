#include "spdlogger.h"

#include <boost/filesystem.hpp>

#include <spdlog/spdlog.h>

namespace spd = spdlog;

struct spdlogger::impl {
  std::shared_ptr<spd::logger> log;

  // 500mb max / 10 files max
  impl(std::string name, std::string path, size_t max_file_size, size_t max_number_of_files, int flush_period_seconds) {
        const size_t q_size = 8192;
        spd::set_async_mode(q_size, spdlog::async_overflow_policy::block_retry,
                       nullptr,
                       std::chrono::seconds(flush_period_seconds));
        log = spd::rotating_logger_mt("rotating_log", path + "/" + name,
          max_file_size, max_number_of_files);
      }

  ~impl() {
    try {
      shutdown();
    } catch (...) {}
  }

  void shutdown() {
    if (log) {
      log->flush();
      spd::drop("rotating_log");
    }
    log.reset();
  }
};

spdlogger::spdlogger(std::string name, std::string path, size_t max_file_size, size_t max_number_of_files, int flush_period_seconds) {
  boost::filesystem::path dir(path);
  boost::filesystem::create_directory(dir);
  pimpl.reset(new impl(name, path, max_file_size, max_number_of_files, flush_period_seconds));
}

spdlogger::~spdlogger() {}

void spdlogger::log(std::string const& message) { pimpl->log->info(message); }

void spdlogger::shutdown() { pimpl->shutdown(); }
