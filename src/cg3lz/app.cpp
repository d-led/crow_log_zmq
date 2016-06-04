#include <crow.h>

#include "app.h"

simple_log_server::simple_log_server():default_log(cfg.logging) {
    load_config();
}

simple_log_server::~simple_log_server(){
    try {
        shutdown();
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