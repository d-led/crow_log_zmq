#include <crow.h>

#include "app.h"
#include "spdlogger.h"

simple_log_server::simple_log_server():
default_log(cfg.logging)
{
    load_config();
    configure_sink();
}

simple_log_server::~simple_log_server() {
    try {
        shutdown();
        if (sink)
            sink->shutdown();
    } catch (...) {}
}

void simple_log_server::run() {

}

void simple_log_server::shutdown() {
    save_config();
}

void simple_log_server::load_config() {
    cfg_persistence.configure_logging([this](std::string const& m) {
      default_log.log(m, crow::LogLevel::INFO);
    });
    cfg = cfg_persistence.load();
}

void simple_log_server::save_config() {
    cfg_persistence.save(cfg);
}

void simple_log_server::configure_sink() {
    sink = std::unique_ptr<spdlogger>(new spdlogger(
        "simple_log_server",
        cfg.log_path,
        static_cast<size_t>(cfg.max_file_size),
        static_cast<size_t>(cfg.max_number_of_files),
        cfg.flush_period_seconds
    ));
}
