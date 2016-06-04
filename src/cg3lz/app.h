#pragma once

#include "../common/config.h"
#include "default_logger.h"
#include "config_persistence.h"

#include <memory>

class spdlogger;
class zeromq_log_source;

class simple_log_server {
    config cfg;
    config_persistence cfg_persistence;
    DefaultLogger default_log;
    std::unique_ptr<spdlogger> sink;
    std::unique_ptr<zeromq_log_source> source;

public:
    simple_log_server();
    ~simple_log_server();
public:
    void run();
    void shutdown();
private:
    void load_config();
    void save_config();
    void configure_sink();
    void configure_source();
};
