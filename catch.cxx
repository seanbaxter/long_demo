#include <stdexcept>
#include <cstdio>

// Forward declare a function defined in libthrow.so. The function is
// invoked with a foreign function call. It throws a real C++ exception,
// which is caught by a direct or indirect meta try clause.
void throw_func(const char* msg);

// Catch an exception in a meta-try block.
@meta try {
  @meta throw_func("direct meta-try");

} catch(std::exception& e) {
  @meta printf("Caught exception '%s' in meta-try\n", e.what());
}

// Catch an exception in a normal function, called from a meta
// expression statement.
inline void try_test() {
  try {
    throw_func("indirect meta-try");

  } catch(std::exception& e) {
    printf("Caught exception '%s' in try_test\n", e.what());
  }
}

// Execute try_test at compile time.
@meta try_test();

int main() {
  return 0;
}