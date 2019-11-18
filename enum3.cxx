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

// Print their contents.
@meta puts("joined_t:");
@meta printf("%d: %s\n", int..., @type_name(@enum_types(joined_t)))...;

// Expand into a tuple's template arguments.
template<typename... types_t>
struct tuple_t {
  types_t @(int...) ...;
};

typedef tuple_t<@enum_types(joined_t)...> my_tuple_t;
@meta puts("\nmy_tuple_t:");
@meta puts(@decl_string(@member_types(my_tuple_t), @member_names(my_tuple_t)))...;

// Back the tuple member types back out into a type list. Add a reference.
enum typename ref_list_t {
  @member_types(my_tuple_t)& ...;
};

// Print their contents.
@meta puts("\nref_list_t:");
@meta printf("%d: %s\n", int..., @type_name(@enum_types(ref_list_t)))...;

enum typename sorted_t {
  // Expand the string spellings of the types into an array, along with the
  // index into the type.
  @meta std::vector<std::pair<std::string, int> > types {
    std::make_pair( 
      @type_name(@enum_types(ref_list_t)),
      int...
    )...
  };

  // Lexicographically sort the types.
  @meta std::sort(types.begin(), types.end());

  // Gather the types and define enumerators.
  @enum_type(ref_list_t, @pack_nontype(types).second) ...;
};

// Print their contents.
@meta puts("\nsorted_t:");
@meta printf("%d: %s\n", int..., @type_name(@enum_types(sorted_t)))...;


int main() {
  return 0;
}