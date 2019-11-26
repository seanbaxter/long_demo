#include <cstdio>
#include <algorithm>
#include <array>

// Back the tuple member types back out into a type list. Add a reference.
enum typename unsorted_t {
  int,
  double,
  char[4],
  const int&,
  int*,
  void,
  void*,
  const double[3]
};

// Print ref_list_t's contents.
@meta puts("\nunsorted_t:");
@meta printf("%d: %s\n", int..., @enum_type_strings(unsorted_t))...;

enum typename sorted1_t {
  // Expand the string spellings of the types into an array, along with the
  // index into the type.
  @meta std::array types {
    std::make_pair<std::string, int>( 
      @enum_type_strings(unsorted_t),
      int...
    )...
  };

  // Lexicographically sort the types.
  @meta std::sort(types.begin(), types.end());

  // Gather the types and define enumerators.
  @enum_type(unsorted_t, @pack_nontype(types).second) ...;
};

// Print sorted1_t's contents.
@meta puts("\nsorted1_t:");
@meta printf("%d: %s\n", int..., @enum_type_strings(sorted1_t))...;


enum typename sorted2_t {
  // Expand the string spellings of the types into an array, along with the
  // index into the type.
  @meta std::array types { 
    std::make_pair<std::string, @mtype>(
      @enum_type_strings(unsorted_t),
      @dynamic_type(@enum_types(unsorted_t))
    )... 
  };

  // Lexicographically sort the types.
  @meta std::sort(types.begin(), types.end());

  // Convert the @mtype values back to real types.
  @static_type(@pack_nontype(types).second) ...;
};

// Print sorted2_t's contents.
@meta puts("\nsorted2_t:");
@meta printf("%d: %s\n", int..., @enum_type_strings(sorted2_t))...;


enum typename sorted3_t {
  // Expand the string spellings of the types into an array, along with the
  // index into the type.
  @meta std::array types {
    std::string(@enum_type_strings(unsorted_t)) ...
  };

  // Lexicographically sort the types.
  @meta std::sort(types.begin(), types.end());

  // Inject types from strings.
  @type_id(@pack_nontype(types)) ...;
};

// Print sorted3_t's contents.
@meta puts("\nsorted3_t:");
@meta printf("%d: %s\n", int..., @enum_type_strings(sorted3_t))...;

int main() {
  return 0;
} 