#include <crow_all.h>

//based on original example_with_all.cpp

#include <sstream>
#include <atomic>

class ExampleLogHandler : public crow::ILogHandler {
    public:
        void log(std::string message, crow::LogLevel level) override {
           std::cout << "ExampleLogHandler -> " << message;
        }
};

#ifdef CROW_MSVC_WORKAROUND
#undef CROW_ROUTE
#define CROW_ROUTE(app, url) app.route_dynamic(url)
#endif

int main()
{
    crow::SimpleApp app;
    
    CROW_ROUTE(app, "/")
        .name("hello")
    ([]{
        return "Hello World!";
    });

    // ignore all log
    crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    auto handler(std::make_shared<ExampleLogHandler>());
    crow::logger::setHandler(handler.get());

    app.port(18080)
        .multithreaded()
        .run();
}
