#include "file_contents.h"

#include <fstream>
#include <iomanip>

#include <boost/filesystem.hpp>

file_contents::file_contents(std::string const& fn) : filename(fn) {}

bool file_contents::exists() const {
  return boost::filesystem::exists(filename);
}

std::string file_contents::contents() const {
  std::ifstream input(filename, std::ios::binary);

  if (!input) return "Sorry! The file has disappeared into the void.";

  std::istreambuf_iterator<char> end;

  return std::string(std::istreambuf_iterator<char>(input), end);
}
