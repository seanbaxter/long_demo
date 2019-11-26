#include <cstdarg>
#include <string>
#include <optional>
#include <iostream>

template<typename obj_t, typename arg_t>
bool dynamic_assign(obj_t& obj, const char* name, const arg_t& arg) {
  // Loop over all data members.
  @meta for(int i = 0; i < @member_count(obj_t); ++i) {

    // Check the argument name against the member name.
    if(!strcmp(@member_name(obj_t, i), name)) {

      // Attempt an assignment...
      if constexpr(requires { @member_value(obj, i) = arg; })
        return @member_value(obj, i) = arg, true;
      else
        return false;
    }
  }

  return false;
}

template<typename ret_t, typename obj_t>
std::optional<ret_t> dynamic_get(const obj_t& obj, const char* name) {
  // Loop over all data members.
  @meta for(int i = 0; i < @member_count(obj_t); ++i) {

    // Check the argument name against the member name.
    if(!strcmp(@member_name(obj_t, i), name)) {
      
      // Attempt a conversion...
      if constexpr(requires { (ret_t)@member_value(obj, i); })
        return { (ret_t)@member_value(obj, i) };
      else
        return { };
    }
  }

  return { };
}

////////////////////////////////////////////////////////////////////////////////

struct foo_t {
  int x;
  std::string y;
  double z;
};

int main() {
  foo_t obj { 4, "Hello", 1.618 };

  const char* names[] { "x", "y", "z", "w" };

  puts("Assignment:");
  for(const char* name : names) {
    if(dynamic_assign(obj, name, "Goodbye"))
      printf("Assigned 'Goodbye' to '%s'\n", name);
    else
      printf("Could not assign 'Goodbye' to '%s'\n", name);
  }

  puts("\nAccess:");
  for(const char* name : names) {
    if(auto val = dynamic_get<float>(obj, name))
      printf("obj.%s -> %f\n", name, *val);
    else
      printf("Could not get float obj.%s\n", name);
  }

  puts("\nObject values:");
  std::cout<< @member_names(foo_t)<< ": "<< @member_values(obj)<< "\n"...;

  return 0;
}