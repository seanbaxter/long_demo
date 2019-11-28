#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

template<typename type_t>
int cmp_pred(const void* a, const void* b) {
  type_t x = *static_cast<const type_t*>(a);
  type_t y = *static_cast<const type_t*>(b);
  return x < y ? -1 : x > y ? +1 : 0;
}

template<typename type_t>
void sort_vec(std::vector<type_t>& vec) {
  // cmp_pred<type_t> returns the address of an FFI closure here.
  // The Circle interpreter makes a foreign function call to 
  // qsort, and qsort invokes the closure. The interpreter,
  // which implements the closure, regains control, and executes
  // cmp_pred by traversing its AST.
  qsort(vec.data(), vec.size(), sizeof(type_t), cmp_pred<type_t>);
}

template<typename type_t>
void sort_test(size_t count, bool verbose = false) {
  std::vector<type_t> vec(count);
  for(size_t i = 0; i < count; ++i)
    vec[i] = rand() % 1000;

  sort_vec(vec);
  bool sorted = std::is_sorted(vec.begin(), vec.end());
  if(!sorted) {
    fputs("input array not sorted", stderr);

  } else if(verbose) {
    for(auto x : vec)
      std::cout<< x<< "\n";
  }
}

int main() {
  @meta sort_test<float>(50, true);

  return 0;
}