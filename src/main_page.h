#pragma once

#include "log_view.h"
#include "config.h"

#include <string>

class main_page {
    config& cfg;
    log_view view;
public:
    main_page(config& c):cfg(c),view(c) {}
public:
	std::string render() const;
};
