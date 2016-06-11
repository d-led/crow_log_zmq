#include <crow.h>

#include "app.h"
#include "spdlogger.h"
#include "zeromq_log_source.h"
#include "resources.h"
#include "websocket_tail.h"

#include "file_contents.h"

#include <stdexcept>

simple_log_server::simple_log_server(zmq::context_t& context):
ctx(context),
default_log(cfg.logging),
count(0)
{
    load_config();
    configure_sink();
    configure_source();
    configure_app_logging();
    configure_app_routing();
    configure_source_logging();
    configure_views();
    configure_websocket_ticker();
}

simple_log_server::~simple_log_server() {
    try {
        shutdown();
    } catch (...) {}
}

void simple_log_server::log(std::string const& m) {
  sink->log(m);
  forward_log(m);
  tick();
}

void simple_log_server::forward_log(std::string const& m) {
}

void simple_log_server::run() {
  check_configuration();
  source->start_once();
  ticker->run();
  app.port(cfg.port).multithreaded().run();
}

void simple_log_server::shutdown() {
    save_config();
    if (source)
        source->stop();
    default_log.shutdown();
    if (sink)
        sink->shutdown();
}

void simple_log_server::tick()
{
  count++;
  if (count % 100000 == 0)
    default_log.log(
      std::string("Received ") + std::to_string(count) + " entries",
      crow::LogLevel::INFO);
}

void simple_log_server::load_config() {
    cfg_persistence.configure_logging([this](std::string const& m) {
      default_log.log(m, crow::LogLevel::INFO);
    });
    cfg = cfg_persistence.load();
}

void simple_log_server::save_config() {
    cfg_persistence.save(cfg);
}

void simple_log_server::configure_sink() {
    sink = std::unique_ptr<spdlogger>(new spdlogger(
        "simple_log_server",
        cfg.log_path,
        static_cast<size_t>(cfg.max_file_size),
        static_cast<size_t>(cfg.max_number_of_files),
        cfg.flush_period_seconds
    ));
}

void simple_log_server::configure_source() {
    source = std::unique_ptr<zeromq_log_source>(
        new zeromq_log_source(ctx, cfg.zeromq_log_port)
    );
}

void simple_log_server::configure_app_routing() {
  add_front_page();
  add_logging_rest_endpoint();
  add_crow_logging_toggle();
  add_kill_switch();
  add_naive_log_file_download();
  add_websocket_ticker();
}

void simple_log_server::configure_source_logging() {
  default_log.log(std::string("Saving logs to: ") + cfg.log_path, crow::LogLevel::INFO);

  auto console = [this](std::string const& m) {
    default_log.log(m, crow::LogLevel::INFO);
  };

  auto files = [this](std::string const& m) { log(m); };

  source->configure_logging(console, files);
}

void simple_log_server::configure_views()
{
  index = std::unique_ptr<main_page>(new main_page(cfg.log_path, cfg.logging));
  logs = std::unique_ptr<log_view>(new log_view(cfg.log_path));
}

void simple_log_server::check_configuration() {
    if (!source || !sink || !index || !logs)
        throw std::runtime_error("App not configured correctly!");
}

void simple_log_server::configure_websocket_ticker()
{
    ticker = std::make_unique<websocket_ticker>(ctx, 9002);
}

void simple_log_server::configure_app_logging() {
    crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    crow::logger::setHandler(&default_log);
}

void simple_log_server::add_kill_switch() {
  // http get http://localhost:18080/kill
  CROW_ROUTE(app, "/kill")
    ([this] {
    this->shutdown();
    std::thread([] {
      std::this_thread::sleep_for(std::chrono::seconds(3));
      std::exit(0);
    }).detach();
    return "OK! Shutting down!";
  });
}

void simple_log_server::add_crow_logging_toggle() {
  // http get http://localhost:18080/toggle_logging
  CROW_ROUTE(app, "/toggle_logging")
    ([this] {
    cfg.logging = !cfg.logging;
    return std::string("Logging now: ") + (cfg.logging ? "on" : "off");
  });
}

void simple_log_server::add_logging_rest_endpoint() {
  // echo "bla\c" | http put http://localhost:18080/log
  CROW_ROUTE(app, "/log")
    .methods("PUT"_method)([this](const crow::request& req) {
    log(req.body);
    return crow::response(200);
  });
}

void simple_log_server::add_front_page() {
  CROW_ROUTE(app, "/").name("front_end")([this] {
    auto response = crow::response(index->render());
    response.set_header("Content-Type", "text/html;charset=UTF-8");
    return response;
  });
}

void simple_log_server::add_naive_log_file_download() {
  CROW_ROUTE(app,
    "/logs/<string>")([this](std::string filename)->crow::response {
    auto file = file_contents(logs->path_for_filename(filename));
    if (!file.exists()) {
      auto response = crow::response();
      response.code = 404;
      return response;
    }
    else {
      try {
        auto response = crow::response(file.contents());
        response.set_header(
          "Content-Type",
          "application/octet-stream");  // or set to whatever you need
        return response;
      }
      catch (std::bad_alloc&) {
        auto response = crow::response(std::string("Sorry, not enough memory to download ") + filename);
        response.code = 500;
        return response;
      }
    }
  });
}

void simple_log_server::add_websocket_ticker() {
    CROW_ROUTE(app,"/ticker")([this] {
        auto response = crow::response(resources::ticker_html);
        response.set_header("Content-Type", "text/html;charset=UTF-8");
        return response;
    });
}
