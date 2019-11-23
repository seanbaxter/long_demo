#include <iostream>

template<typename... types_t>
struct tuple_t {
  types_t @(int...) ...;
};

template<typename... types_t>
tuple_t(types_t... args) -> tuple_t<types_t...>;

template<typename type_t>
void print_object(const type_t& obj) {
  std::cout<< 
    @member_decl_strings(type_t)<< ": "<<
    @member_values(obj)<< "\n" ...;
}

@meta print_object(tuple_t {
  6, "Hello tuple - pack style", 3.142
});

int main() {
  return 0;
}