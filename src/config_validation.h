#pragma once

#include "config.h"

#include <unordered_map>
#include <string>
#include <functional>

template <typename T>
bool reset_if_false(T& value, bool check, T default_value) {
    if (!check)
        value = default_value;
    return check;
}

inline std::string validate_and_reset_to_default(config& c) {
    config def;
    static std::unordered_map<std::string,std::function<bool(config& c)>> checks = {
        {"port", [&](config& c) { return reset_if_false(c.port, c.port > 0 && c.port <= 65535, def.port); } },
        {"zeromq_log_port", [&](config& c) { return reset_if_false(c.zeromq_log_port, c.zeromq_log_port > 0 && c.zeromq_log_port <= 65535, def.zeromq_log_port); } },
        {"max_file_size", [&](config& c) { return reset_if_false(c.max_file_size, c.max_file_size > 100 && c.max_file_size <= 1024000000, def.max_file_size); /*~1Gb*/ } },
        {"max_number_of_files", [&](config& c) { return reset_if_false(c.max_number_of_files, c.max_number_of_files > 0, def.max_number_of_files); } },
        {"flush_period_seconds", [&](config& c) { return reset_if_false(c.flush_period_seconds, c.flush_period_seconds >= 0, def.flush_period_seconds); } },
    };
    std::string res;
    const std::string newline("\n");
    for (auto& check: checks) {
        if (!check.second(c)) {
            res += (res.empty() ? std::string() : newline) + check.first + " is out of range, set to default";
        }
    }
    return res;
}
