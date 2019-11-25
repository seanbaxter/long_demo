#include <cstdio>

struct day_pair_t {
  const char* name;
  int value;
};

// or inline constexpr day_pair_t weekdays[] { ...
@meta const day_pair_t weekdays[] {
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

@meta for enum(weekday_t e : weekday_t)
  @meta printf("%s = %d\n", @enum_name(e), (int)e);

int main() {
  return 0;
}