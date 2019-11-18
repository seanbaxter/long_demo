#include <cstdio>

struct day_pair_t {
  const char* name;
  int value;
};

inline constexpr day_pair_t weekdays[] {
  { "Monday",    10 },
  { "Tuesday",   11 },
  { "Wednesday", 12 },
  { "Thursday",  13 },
  { "Friday",    14 },
  { "Saturday",  15 },
  { "Sunday",    16 },
};

enum class weekday_t {
  @meta for(day_pair_t pair : weekdays)
    @(pair.name) = pair.value;
};

@meta printf("%s = %d\n", @enum_names(weekday_t), @enum_pack(weekday_t)) ...;

int main() {
  return 0;
}