#include <cstdio>
#include <cstring>
#include <optional>

// @meta for over @enum_count
template<typename type_t>
const char* enum_to_name_loop(type_t e) {
  switch(e) {
    @meta for(int i = 0; i < @enum_count(type_t); ++i)
      case @enum_value(type_t, i):
        return @enum_name(type_t, i);

    default:
      return nullptr;
  }
}

// @meta for enum for range-for over enumerators.
template<typename type_t>
const char* enum_to_name(type_t e) {
  switch(e) {
    @meta for enum(type_t e2 : type_t)
      case e2:
        return @enum_name(e2);

    default:
      return nullptr;
  }
}

template<typename type_t>
std::optional<type_t> name_to_enum(const char* name) {
  @meta for enum(type_t e2 : type_t)
    if(!strcmp(@enum_name(e2), name))
      return e2;
  
  return { };
}

enum class shape_t {
  circle, 
  square,
  triangle,
  hexagon,
};

int main() {
  const char* names[] {
    "square", "octagon", "hexagon", "rhombus", "circle"
  };

  puts("Map name to enum:");
  for(const char* name : names) {
    if(auto e = name_to_enum<shape_t>(name)) {
      printf("%s = %d\n", name, (int)*e);

    } else {
      printf("%s is unknown\n", name);

    }
  }

  // Get names from enum values.
  shape_t shapes[] { 
    shape_t::square, (shape_t)7, shape_t::triangle, (shape_t)10
  };

  puts("\nMap value to name:");
  for(shape_t shape : shapes) {
    if(const char* name = enum_to_name(shape)) {
      printf("%d -> '%s'\n", (int)shape, name);

    } else {
      printf("%d is not a shape_t\n", shape);
    }
  }
  
  return 0;
}