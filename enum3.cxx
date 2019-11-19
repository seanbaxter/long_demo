#include <cstdio>

@meta const char* weekdays[] {
  "Monday", 
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday",
  "Sunday",
};

enum class weekday_t {
  @(@pack_nontype(weekdays)) = 10 + int... ...;
};

@meta printf("%s = %d\n", @enum_names(weekday_t), @enum_pack(weekday_t)) ...;

int main() {
  return 0;
}