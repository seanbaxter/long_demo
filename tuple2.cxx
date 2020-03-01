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
    @member_type_strings(type_t)<< ": "<<
    @member_values(obj)<< "\n" ...;
}

// @member_names - pack of all public non-static data member names
// @member_ptrs - pack of pointers to member objects
// @member_types - pack of member types
// @member_type_strings - pack of member type strings
// @member_decl_strings - pack of member declaration strings
// @member_values - pack of member glvalues

@meta print_object(tuple_t {
  6, "Hello tuple - pack style", 3.142
});

int main() {
  return 0;
}