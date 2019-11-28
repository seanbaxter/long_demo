#include <stdexcept>

void throw_func(const char* msg) {
  printf("Throwing an exception with msg '%s'\n", msg);
  throw std::runtime_error(msg);
}
