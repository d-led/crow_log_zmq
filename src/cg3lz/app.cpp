#include <crow.h>

#include "app.h"
#include "spdlogger.h"
#include "zeromq_log_source.h"

#include <stdexcept>

simple_log_server::simple_log_server():
default_log(cfg.logging)
{
    load_config();
    configure_sink();
    configure_source();
    configure_app();
}

simple_log_server::~simple_log_server() {
    try {
        shutdown();
    } catch (...) {}
}

void simple_log_server::run() {
    app.run();
}

void simple_log_server::shutdown() {
    save_config();
    if (source)
        source->stop();
    if (sink)
        sink->shutdown();
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

void simple_log_server::configure_source() {
    source = std::unique_ptr<zeromq_log_source>(
        new zeromq_log_source(cfg.zeromq_log_port)
    );
}

void simple_log_server::configure_app() {
    configure_app_logging();
    CROW_ROUTE(app, "/")([] {
        return "bla";
    });
}

void simple_log_server::check_configuration() {
    if (!source || !sink)
        throw std::runtime_error("App not configured correctly!");
}

void simple_log_server::configure_app_logging() {
    crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    crow::logger::setHandler(&default_log);
}
