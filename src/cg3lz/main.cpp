#include <stdexcept>

#include "app.h"

int main(int argc, char* argv[]) {
  try {
    simple_log_server app;
    app.run();
  }
  catch (std::bad_alloc& e) {
    std::cerr << "Sorry, bad alloc: " << e.what() << std::endl;
  }
  catch (std::exception& e) {
    std::cerr << "Sorry, crashed: " << e.what() << std::endl;
  }
}
