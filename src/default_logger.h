#pragma once

#include <crow.h>
#include <string>
#include <spdlog/spdlog.h>
#include <boost/algorithm/string/trim.hpp>

namespace spd = spdlog;

class DefaultLogger : public crow::ILogHandler {
  bool& logging;
  std::shared_ptr<spd::logger> console;
#if defined(WINVER) && WINVER < _WIN32_WINNT_WIN10
  static const bool DO_COLOR = false;
#else
  static const bool DO_COLOR = true;
#endif

 public:
  DefaultLogger(bool& l) : logging(l),console(spd::stdout_logger_mt("cg3lz_crow", DO_COLOR)) {
    console->set_level(spd::level::debug);
  }

  inline void log(std::string message, crow::LogLevel level) override {
    if (!logging)
      return;
    message = boost::trim_copy(message);
    switch (level) {
    case crow::LogLevel::CRITICAL:
      console->critical(message);
      break;
    case crow::LogLevel::DEBUG:
      console->debug(message);
      break;
    case crow::LogLevel::ERROR:
      console->error(message);
      break;
    case crow::LogLevel::INFO:
      console->info(message);
      break;
    case crow::LogLevel::WARNING:
      console->warn(message);
      break;
    default:
      console->info(message);
      break;
    }
  }
};
