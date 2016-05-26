#include <crow_all.h>

// based on original example_with_all.cpp

#include <string>
#include <chrono>
#include <thread>

#include "config.h"
#include "default_logger.h"
#include "g3logger.h"
#include "zeromq_log_sink.h"
#include "log_view.h"

class cg3lz {
  crow::SimpleApp app;
  config cfg;

  DefaultLogger default_log;
  g3logLogger log;
  zeromq_log_sink sink;
  log_view view;

 public:
  cg3lz(std::string const& name)
      : default_log(cfg),
        log(name, cfg.log_path),
        sink(cfg, [this](std::string const& m) {
                    default_log.log(m, crow::LogLevel::INFO);
                  },
             log),
        view(cfg) {
    configure_routing();
    configure_crow_logging();
  }

  void run() {
    default_log.log(std::string("Saving logs to: ") + cfg.log_path + "\n",
                    crow::LogLevel::INFO);

    sink.start_once();

    app.port(cfg.port).multithreaded().run();
  }

 private:
  void configure_crow_logging() {
    crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    crow::logger::setHandler(&default_log);
  }

  void configure_routing() {
    CROW_ROUTE(app, "/").name("front_end")([this] {
      std::string res("Logs: \n");
      auto logs = view.get_logs();
      for (auto& entry : logs) {
        res += " " + entry.filename + " [" + std::to_string(entry.size) + "B] \n";
      }
      res += "\n";
      return res;
    });

    // echo "bla\c" | http put http://localhost:18080/log
    CROW_ROUTE(app, "/log")
        .methods("PUT"_method)([this](const crow::request& req) {
           log.log(req.body);
           return crow::response(200);
         });

    // http get http://localhost:18080/toggle_logging
    CROW_ROUTE(app, "/toggle_logging")
    ([this] {
      cfg.logging = !cfg.logging;
      return std::string("Logging now: ") + (cfg.logging ? "on" : "off");
    });

    // http get http://localhost:18080/kill
    CROW_ROUTE(app, "/kill")
    ([this] {
      sink.stop();
      std::thread([] {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::exit(0);
                  }).detach();
      return "OK! Shutting down!";
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
