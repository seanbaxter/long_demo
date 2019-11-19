#include <cstdio>
#include <type_traits>

@meta const char* types[] {
  "int",
  "double",
  "char[4]",
  "const int&",
  "int*",
  "void",
  "void*",
  "const double[3]"
};

template<int N>
struct foo_t {
  enum typename class my_enum_t {
    static_assert(N <= std::extent<decltype(types)>::value);

    // Create enumerators for the first N types spelled out in types array.
    @("enum_", int...) = @type_id(types[__integer_pack(N)]) ...;
  };
};


typedef foo_t<5>::my_enum_t X;
@meta printf("%s = %s\n", @enum_names(X), @type_name(@enum_types(X)))...;

int main() {
  return 0;
}