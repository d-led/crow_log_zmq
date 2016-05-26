#include <crow_all.h>

// based on original example_with_all.cpp

#include <string>
#include <chrono>
#include <thread>

#include "config.h"
#include "default_logger.h"
#include "g3logger.h"
#include "zeromq_log_sink.h"

static void run(std::string name) {
  crow::SimpleApp app;

  config cfg;

  DefaultLogger default_log(cfg);
  g3logLogger log(name, cfg.log_path);

  zeromq_log_sink sink(cfg, [&default_log](std::string const& m) {
                              default_log.log(m, crow::LogLevel::INFO);
                            },
                       log);

  default_log.log(std::string("Saving logs to: ") + cfg.log_path + "\n",
                  crow::LogLevel::INFO);

  sink.start_once();

  CROW_ROUTE(app, "/").name("hello")([] { return "Hello World!"; });

  // echo "bla\c" | http put http://localhost:18080/log
  CROW_ROUTE(app, "/log")
      .methods("PUT"_method)([&log](const crow::request& req) {
         log.log(req.body);
         return crow::response(200);
       });

  // http get http://localhost:18080/toggle_logging
  CROW_ROUTE(app, "/toggle_logging")
  ([&cfg] {
    cfg.logging = !cfg.logging;
    return std::string("Logging now: ") + (cfg.logging ? "on" : "off");
  });

  // http get http://localhost:18080/kill
  CROW_ROUTE(app, "/kill")
  ([&sink] {
    sink.stop();
    std::thread([] {
                  std::this_thread::sleep_for(std::chrono::seconds(1));
                  std::exit(0);
                }).detach();
    return "OK! Shutting down!";
  });

  crow::logger::setLogLevel(crow::LogLevel::DEBUG);
  auto handler(std::make_shared<DefaultLogger>(cfg));
  crow::logger::setHandler(handler.get());

  app.port(cfg.port).multithreaded().run();
}

int main(int argc, char* argv[]) {
  try {
    run(argv[0]);
  }
  catch (std::bad_alloc& e) {
    std::cerr << "Sorry, bad alloc: " << e.what() << std::endl;
  }
  catch (std::exception& e) {
    std::cerr << "Sorry, crashed: " << e.what() << std::endl;
  }
}
