#pragma once

#include <string>

class file_contents {
    std::string filename;

public:
    file_contents(std::string const& fn);

public:
    bool exists() const;
    // WARNING: inefficient and might interfere with log rotation
    std::string contents() const;
};
