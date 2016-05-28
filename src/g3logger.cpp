#include "g3logger.h"

#include <boost/filesystem.hpp>

// g3log
#include <g3sinks/LogRotate.h>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/std2_make_unique.hpp>


g3logLogger::g3logLogger(std::string name, std::string path, tick_t t)
    : worker(g3::LogWorker::createLogWorker()), tick(t) {
  boost::filesystem::path dir(path);
  boost::filesystem::create_directory(dir);
#ifdef _MSC_VER
  auto logger = worker->addDefaultLogger(name, path);
#else
  auto logger = worker->addSink(std2::make_unique<LogRotate>(name, path),
                                &LogRotate::save);
#endif
  g3::initializeLogging(worker.get());
}

g3logLogger::~g3logLogger() {}

void g3logLogger::log(std::string const& message) {
  LOG(INFO) << message;
  tick();
}
