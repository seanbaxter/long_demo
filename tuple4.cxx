#include <cstdio>
#include <iostream>
#include <vector>
#include "serialization.hxx"

template<typename type_t>
void print_type() {
  std::cout<< @type_string(type_t)<< ":\n";
  std::cout<< "  "<< @member_decl_strings(type_t)<< "\n" ...;
}

template<typename type_t>
struct struct_of_pointers_t {
  // Use a for loop to visit each member of type_t and declare one 
  // pointer member.
  @meta for(int i = 0; i < @member_count(type_t); ++i)
    @member_type(type_t, i)* @(@member_name(type_t, i));
};

template<typename type_t>
struct struct_of_vectors_t {
  // Use a pack declaration to declare a std::vector over each member
  // of type_t.
  std::vector<@member_types(type_t)> @(@member_names(type_t))...;

  // Four ways to push an object.
  void push_object1(const type_t& obj) {
    // Use name lookup to push each member of obj to its corresponding
    // vector.
    @meta for(int i = 0; i < @member_count(type_t); ++i)
      this->@(@member_name(type_t, i)).push_back(@member_value(obj, i));
  }

  void push_object2(const type_t& obj) {
    // Iterate over members of the operands.
    @meta for(int i = 0; i < @member_count(type_t); ++i)
      @member_value(*this, i).push_back(@member_value(obj, i));
  }

  void push_object3(const type_t& obj) {
    // Use name lookup with a parameter pack dependent member name.
    this->@(@member_names(type_t)).push_back(@member_values(obj)) ...;
  }

  void push_object4(const type_t& obj) {
    // Pack expand over members of the operands.
    @member_values(*this).push_back(@member_values(obj)) ...;
  }

  type_t operator[](size_t index) const {
    return { @member_values(*this)[index]... };
  }

  void set(size_t index, const type_t& obj) {
    @member_values(*this)[index] = @member_values(obj) ...;
  }
};

struct foo_t {
  int x;
  double y;
  std::string z;
};

typedef struct_of_pointers_t<foo_t> foo2_t;
typedef struct_of_vectors_t<foo_t> foo3_t;

@meta print_type<foo2_t>();
@meta print_type<foo3_t>();

int main() {

  foo3_t container;
  container.push_object1(foo_t { 1, 1.5, "First" });
  container.push_object2(foo_t { 2, 2.5, "Second" });
  container.push_object3(foo_t { 3, 3.5, "Third" });
  container.push_object4(foo_t { 4, 4.5, "Fourth" });

  std::cout<< stream_simple(container)<< "\n";

  std::cout<< stream_simple(container[2])<< "\n";

  return 0;
}
