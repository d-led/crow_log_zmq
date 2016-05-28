#pragma once

#include "log_view.h"
#include "config.h"

#include <string>

class main_page {
    std::string log_path;
    log_view view;
public:
    main_page(std::string const& p):log_path(p),view(p) {}
public:
	std::string render() const;
};
