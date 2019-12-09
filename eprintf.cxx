#include <vector>
#include <cmath>
#include <stdexcept>
#include <cstdio>
#include "serialization.hxx"

// Scan through until the closing '}'.
inline const char* parse_braces(const char* text) {
  const char* begin = text;

  while(char c = *text) {
    if('{' == c)
      return parse_braces(text + 1);
    else if('}' == c)
      return text + 1;
    else
      ++text;    
  }

  throw std::runtime_error("mismatched { } in parse_braces");
}

// Edit the format specifier. Dump the text inside 
inline void transform_format(const char* fmt, std::string& fmt2, 
  std::vector<std::string>& exprs) {

  std::vector<char> text;
  while(char c = *fmt) {
    if('{' == c) {
      // Parse the contents of the braces.
      const char* end = parse_braces(fmt + 1);
      exprs.push_back(std::string(fmt + 1, end - 1));
      fmt = end;
      text.push_back('%');
      text.push_back('s');

    } else if('%' == c && '{' == fmt[1]) {
      // %{ is the way to include a { character in the format string.
      fmt += 2;
      text.push_back('{');

    } else {
      ++fmt;
      text.push_back(c);
    }
  }

  fmt2 = std::string(text.begin(), text.end());
}

@mauto eprintf(const char* fmt) {
  // Process the input specifier. Remove {name} and replace with %s.
  // Store the names in the array.
  @meta std::string fmt2;
  @meta std::vector<std::string> exprs;
  @meta transform_format(fmt, fmt2, exprs);

  // Print the modified format specifier and each printf argument as
  // a diagnostic.
  @meta printf("Format specifier: '%s'\n", fmt2.c_str());
  @meta printf("%d: %s\n", int..., @pack_nontype(exprs).c_str()) ...;

  // Convert each name to an expression and from that to a string.
  // Pass to sprintf via format.
  return printf(
    @string(fmt2.c_str()), 
    stream_simple(@@expression(@pack_nontype(exprs))).c_str()...
  );
}

struct vec3_t {
  double x, y, z;

  friend vec3_t operator*(double s, const vec3_t& v) {
    return { s * v.x, s * v.y, s * v.z };
  }
};


int main() {
  double x = 5;
  eprintf("x = {x}, sqrt = {sqrt(x)}, exp = {exp(x)}\n");

  vec3_t v { 4, 5, 6 };
  eprintf("v = {v}, v2 = {2 * v}\n");

  return 0;
}