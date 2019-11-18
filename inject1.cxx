#include <cstdio>

struct function_t {
  const char* name;
  const char* definition;
};

inline const function_t functions[] {
  { "sq",         "x * x"                    },
  { "cube",       "sq(x) * x"                },
  { "invsqrt",    "1 / sq(x)"                },
  { "sum_powers", "x + sq(x) + cube(x)"      },
};

@meta for(function_t f : functions) {
  // Declare a function in the innermost real scope (global ns).
  double @(f.name)(double x) {
    return @expression(f.definition);
  }
}

int main() {

  // Call all the functions declared with reflection.
  double x = 15;
  @meta for(function_t f : functions)
    printf("%s(%f) -> %f\n", f.name, x, @(f.name)(x));

  return 0;
}

