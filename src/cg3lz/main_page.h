#pragma once

#include "log_view.h"
#include "../common/config.h"

#include <string>

class main_page {
    std::string log_path;
    log_view view;
    bool& app_logging_enabled;
public:
    main_page(std::string const& p, bool& logging):log_path(p),view(p),app_logging_enabled(logging) {}
public:
	std::string render() const;
};
