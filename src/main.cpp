#include <crow_all.h>

//based on original example_with_all.cpp

#include <sstream>
#include <atomic>
#include <chrono>
#include <thread>

struct config {
    bool logging = true;
    int port = 18080;
};

class DefaultLogger : public crow::ILogHandler {
    config& cfg;
public:

    DefaultLogger(config& c):cfg(c) {}

    void log(std::string message, crow::LogLevel level) override {
        if (cfg.logging)
            std::cout << "cg3lz -> " << message;
    }
};

class g3logLogger : public crow::ILogHandler {
public:
    void log(std::string message, crow::LogLevel level) override {
        std::cout << "cg3lz -> " << message;
    }
};

int main(int argc, char* argv[])
{
    crow::SimpleApp app;

    config cfg;

    g3logLogger log;
    
    CROW_ROUTE(app, "/")
    .name("hello")
    ([]{
        return "Hello World!";
    });

    // echo bla | http put http://localhost:18080/log
    CROW_ROUTE(app, "/log")
    .methods("PUT"_method)
    ([&log](const crow::request& req){
        log.log(req.body,crow::LogLevel::INFO);
        return crow::response(200);
    });

    // http get http://localhost:18080/kill
    CROW_ROUTE(app, "/kill")
    ([]{
        std::thread([]{
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::exit(0);
        }).detach();
        return "OK! Shutting down!";
    });

    // ignore all log
    crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    auto handler(std::make_shared<DefaultLogger>(cfg));
    crow::logger::setHandler(handler.get());

    app.port(cfg.port)
        .multithreaded()
        .run();
}
