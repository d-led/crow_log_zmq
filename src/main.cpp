#include <crow.h>
#include <common.h>

// based on original example_with_all.cpp

#include <string>
#include <chrono>
#include <thread>
#include <cstdint>

#include "config.h"
#include "default_logger.h"
#include "zeromq_log_source.h"
#include "file_contents.h"
#include "main_page.h"
#include "log_view.h"
#include "spdlogger.h"

#pragma comment(lib, "dbghelp.lib")

class cg3lz {
  crow::SimpleApp app;
  config cfg;
  main_page index;
  DefaultLogger default_log;
  spdlogger sink;
  zeromq_log_source source;
  std::uint64_t count = 0;
  log_view logs;

 public:
  //////////////////////////////
  cg3lz(std::string const& name)
      : default_log(cfg.logging),
        sink(name, cfg.log_path),
        source(cfg.zeromq_log_port, [this](std::string const& m) {
                                      default_log.log(m, crow::LogLevel::INFO);
                                    },
               [this](std::string const& m) { log(m); }),
        index(cfg.log_path),
        logs(cfg.log_path) {
    configure_routing();
    configure_crow_logging();
  }

  ~cg3lz() {
    try {
    }
    catch (...) {
    }
  }

  ////////////
  void run() {
    default_log.log(std::string("Saving logs to: ") + cfg.log_path + "\n",
                    crow::LogLevel::INFO);

    source.start_once();

    app.port(cfg.port).multithreaded().run();
  }

 private:
  void configure_crow_logging() {
    crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    crow::logger::setHandler(&default_log);
  }

  void configure_routing() {
    add_front_page();
    add_logging_rest_endpoint();
    add_crow_logging_toggle();
    add_kill_switch();
    add_naive_log_file_download();
  }

  void tick() {
    count++;
    if (count % 100000 == 0)
      default_log.log(
          std::string("Received ") + std::to_string(count) + " entries\n",
          crow::LogLevel::INFO);
  }

  void add_kill_switch() {
    // http get http://localhost:18080/kill
    CROW_ROUTE(app, "/kill")
    ([this] {
      source.stop();
      std::thread([] {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::exit(0);
                  }).detach();
      return "OK! Shutting down!";
    });
  }

  void add_crow_logging_toggle() {
    // http get http://localhost:18080/toggle_logging
    CROW_ROUTE(app, "/toggle_logging")
    ([this] {
      cfg.logging = !cfg.logging;
      return std::string("Logging now: ") + (cfg.logging ? "on" : "off");
    });
  }

  void add_logging_rest_endpoint() {
    // echo "bla\c" | http put http://localhost:18080/log
    CROW_ROUTE(app, "/log")
        .methods("PUT"_method)([this](const crow::request& req) {
           log(req.body);
           return crow::response(200);
         });
  }

  void log(std::string const& m) {
    sink.log(m);
    tick();
  }

  void add_front_page() {
    CROW_ROUTE(app, "/").name("front_end")([this] {
      auto response = crow::response(index.render());
      response.set_header("Content-Type", "text/html;charset=UTF-8");
      return response;
    });
  }

  void add_naive_log_file_download() {
    CROW_ROUTE(app,
               "/logs/<string>")([this](std::string filename)->crow::response {
      auto file = file_contents(logs.path_for_filename(filename));
      if (!file.exists()) {
        auto response = crow::response();
        response.code = 404;
        return response;
      } else {
        auto response = crow::response(file.contents());
        response.set_header(
            "Content-Type",
            "application/octet-stream");  // or set to whatever you need
        return response;
      }
    });
  }
};

int main(int argc, char* argv[]) {
  try {
    cg3lz app(argv[0]);
    app.run();
  }
  catch (std::bad_alloc& e) {
    std::cerr << "Sorry, bad alloc: " << e.what() << std::endl;
  }
  catch (std::exception& e) {
    std::cerr << "Sorry, crashed: " << e.what() << std::endl;
  }
}
