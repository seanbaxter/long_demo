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
    @type_name(@member_types(type_t))<< " "<< 
    @member_names(type_t)<< ": "<< 
    @member_pack(obj)<< "\n" ...;
}

@meta print_object(tuple_t {
  6, "Hello tuple - pack style", 3.142
});

int main() {
  return 0;
}