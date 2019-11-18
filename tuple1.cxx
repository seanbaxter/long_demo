#include <iostream>

template<typename... types_t>
struct tuple_t {
  @meta for(int i = 0; i < sizeof...(types_t); ++i)
    types_t...[i] @(i);
};

template<typename... types_t>
tuple_t(types_t... args) -> tuple_t<types_t...>;

template<typename type_t>
void print_object(const type_t& obj) {
  @meta for(int i = 0; i < @member_count(type_t); ++i)
    std::cout<< 
      @type_name(@member_type(type_t, i))<< " "<< 
      @member_name(type_t, i)<< ": "<< 
      @member_ref(obj, i)<< "\n";
}

@meta print_object(tuple_t {
  5, "Hello tuple - loop style", 1.618
});

int main() {
  return 0;
}