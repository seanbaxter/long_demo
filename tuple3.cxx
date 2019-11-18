#include <vector>
#include <algorithm>
#include <iostream>

template<typename value_t>
void stable_unique(std::vector<value_t>& vec) {
  auto begin = vec.begin();
  auto end = begin;
  for(value_t& value : vec) {
    if(end == std::find(begin, end, value))
      *end++ = std::move(value);
  }
  vec.resize(end - begin);
}

template<typename... types_t>
struct tuple_t {
  types_t @(int...) ...;
};

template<typename... types_t>
tuple_t(types_t... args) -> tuple_t<types_t...>;

template<typename... types_t>
struct unique_tuple_t {
  // Create a compile-time std::vector<@mtype>. @mtype encapsulates a 
  // type, allowing you to manipulate it like a variable. This means we can
  // sort them! @dynamic_type converts a type to an @mtype prvalue.
  @meta std::vector<@mtype> types { @dynamic_type(types_t)... };

  // Use an ordinary function to unique these types.
  @meta stable_unique(types);

  // Typedef a tuple_t over these unique types.
  typedef tuple_t<@pack_type(types)...> type_t;
};

template<typename... types_t>
using unique_tuple = typename unique_tuple_t<types_t...>::type_t;

// Turn these 7 elements into 4 unique ones.
typedef unique_tuple<int, double, char*, double, char*, float> my_tuple;

@meta std::cout<< 
  @type_name(@member_types(my_tuple))<< " "<<
  @member_names(my_tuple)<< "\n" ...;

int main() {
  return 0;
}
