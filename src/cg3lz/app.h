#pragma once

#include "../common/config.h"
#include "default_logger.h"
#include "config_persistence.h"

class simple_log_server {
    config cfg;
    config_persistence cfg_persistence;
    DefaultLogger default_log;
public:
    simple_log_server();
    ~simple_log_server();
public:
    void run();
    void shutdown();
private:
    void load_config();
    void save_config();
};