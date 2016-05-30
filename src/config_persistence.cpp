#include "config_persistence.h"
#include "config_serialization.h"
#include "config_validation.h"
#include "file_contents.h"

#include <fstream>

void config_persistence::configure_logging(log_t l) {
  log = l;
}

config config_persistence::load() const {
  if (log)
    log(std::string("Reading ") + CONFIG_FILENAME);

  config res;

  auto cfg = file_contents(CONFIG_FILENAME);

  if (cfg.exists()) {
    auto contents = cfg.contents();
    picojson::convert::from_string(contents, res);
    auto errors = validate_and_reset_to_default(res);
    if (!errors.empty() && log)
      log(errors);
  }
  else {
    if (log)
      log(std::string("Couldn't find ") + CONFIG_FILENAME + "! Using defaults");
  }

  return res;
}

void config_persistence::save(config c) const {
  if (log)
    log(std::string("Writing ") + CONFIG_FILENAME);

  std::ofstream res(CONFIG_FILENAME);
  if (!res) {
    if (log)
      log(std::string("Sorry, ") + CONFIG_FILENAME + " could not be written");

    return;
  }

  res << picojson::convert::to_string(c);
}
