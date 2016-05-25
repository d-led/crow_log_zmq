#include <crow_all.h>

//based on original example_with_all.cpp

#include <sstream>
#include <atomic>

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
            std::cout << "DefaultLogger -> " << message;
    }
};

int main()
{
    crow::SimpleApp app;

    config cfg;
    
    CROW_ROUTE(app, "/")
    .name("hello")
    ([]{
        return "Hello World!";
    });

    // ignore all log
    crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    auto handler(std::make_shared<DefaultLogger>(cfg));
    crow::logger::setHandler(handler.get());

    app.port(cfg.port)
    .multithreaded()
    .run();
}
