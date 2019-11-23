#include "serialization.hxx"
#include <iostream>

struct vec3_t {
  double x, y, z;
};
typedef std::map<std::string, vec3_t> vec_map_t;

enum class robot_t {
  T800,
  R2D2,
  RutgerHauer,
  Mechagodzilla,
  Bishop,
};

struct struct1_t {
  std::string s;
  std::vector<int> a;
  vec3_t vec;
  robot_t r1, r2;
  vec_map_t axes;
  std::array<char, 3> chars;
  std::optional<int> opt_1;
  std::optional<vec3_t> opt_2;
  int x;
};

int main() {
  struct1_t obj { };
  obj.s = "struct1_t instance";
  obj.a.push_back(4);
  obj.a.push_back(5);
  obj.a.push_back(6);
  obj.vec = { 1, 2, 3 };
  obj.r1 = robot_t::R2D2;
  obj.r2 = robot_t::RutgerHauer;
  obj.axes["x"] = { 1, 0, 0 };
  obj.axes["y"] = { 0, 1, 0 };
  obj.axes["z"] = { 0, 0, 1 };
  obj.chars = { 'a', 'b', 'c' };
  obj.opt_1 = 500;
  // Don't set opt_2.
  obj.x = 600;

  std::cout<< stream(obj)<< "\n";
  return 0;
}
