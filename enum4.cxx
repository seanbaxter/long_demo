#include <cstdio>
#include <vector>
#include <algorithm>

// Define two typed enums.
enum typename type_list1_t {
  int,
  double*,
  char*,
};

enum typename type_list2_t {
  void**,
  int[3],
  char32_t
};

// Concatenate them into one.
enum typename joined_t {
  // Use a loop...
  @meta for enum(auto e : type_list1_t)
    @enum_type(e);

  // Or use a pack expansion.
  @enum_types(type_list2_t)...;
};

// Print joined_t's contents.
@meta puts("joined_t:");
@meta printf("%s = %s\n", @enum_names(joined_t), @enum_type_strings(joined_t))...;

// Expand into a tuple's template arguments.
template<typename... types_t>
struct tuple_t {
  types_t @(int...) ...;
};

typedef tuple_t<@enum_types(joined_t)...> my_tuple_t;
@meta puts("\nmy_tuple_t:");
@meta puts(@member_decl_strings(my_tuple_t))...;

int main() {
  return 0;
}