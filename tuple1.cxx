#include <iostream>

template<typename... types_t>
struct tuple_t {
  @meta for(int i = 0; i < sizeof...(types_t); ++i)
    types_t...[i] @("member_", i);
};

template<typename... types_t>
tuple_t(types_t... args) -> tuple_t<types_t...>;

template<typename type_t>
void print_object(const type_t& obj) {
  @meta for(int i = 0; i < @member_count(type_t); ++i)
    std::cout<< 
      @member_decl_string(type_t, i)<< ": "<<
      @member_value(obj, i)<< "\n";
}

// @member_count - number of public non-static data members
// @member_has_default - if i'th member object has default initializer
// @member_default - default initializer of i'th member object
// @member_name - string name of i'th member object
// @member_ptr - pointer to i'th member object
// @member_type - type of i'th member object
// @member_type_string - type string of i'th member object
// @member_decl_string - declaration string of i'th member object
// @member_value - glvalue of i'th member object

@meta print_object(tuple_t {
  5, "Hello tuple - loop style", 1.618
});

int main() {
  return 0;
}