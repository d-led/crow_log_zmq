#include "main_page.h"
#include "resources.h"

#include <mstch/mstch.hpp>

std::string main_page::render() const {
  mstch::array logs_context;
  auto logs = view.get_logs();
  for (auto& entry : logs) {
    logs_context.emplace_back(
        mstch::map{{"filename", entry.filename},
                   {"url", log_path + entry.filename},
                   {"size", std::to_string(entry.size) + "B"}});
  }

  mstch::map context{
    {"logs", logs_context},
    {"app_logging_enabled", app_logging_enabled},
  };
  return mstch::render(resources::index_html, context);
}
