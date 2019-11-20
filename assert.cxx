#include <cstdio>
#include <cassert>
// #define assert(x) __builtin_assert(x, #x, __FILE__, __LINE__, __PRETTY_FUNCTION__)

template<typename type_t>
void func2(type_t x, type_t y) {
  assert(x > y);
}

template<typename type_t>
void func1(type_t x) {
  func2(x + 1, 6);
}

int main() {
  constexpr int x = 4;
  @meta assert(x == 3);

  @meta func1(3);
  return 0;
}