#include "app.h"

#include <stdexcept>
#include <zmq.hpp>

int main(int argc, char* argv[]) {
  try {
    zmq::context_t ctx(1);
    simple_log_server app(ctx);
    app.run();
  }
  catch (std::bad_alloc& e) {
    std::cerr << "Sorry, bad alloc: " << e.what() << std::endl;
  }
  catch (std::exception& e) {
    std::cerr << "Sorry, crashed: " << e.what() << std::endl;
  }
}
