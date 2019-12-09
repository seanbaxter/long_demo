#include <cstdio>
#include <fstream>
#include <iostream>
#include "json.hpp"

@meta std::vector<std::string> function_names;

@mvoid inject_json_file(const char* filename) {
  @meta printf("Opening resource %s\n", filename);
  @meta std::ifstream json_file(filename);
  @meta nlohmann::json j;
  @meta json_file>> j;

  @meta for(auto& item : j.items()) {
    @meta std::string name = item.value()["name"];
    @meta std::string definition = item.value()["definition"];

    @meta printf("Injecting function \"%s\" = \"%s\"\n", 
      name.c_str(), definition.c_str());

    double @(name)(double x) {
      return @expression(definition);
    }

    @meta function_names.push_back(std::move(name));
  }  
}

@macro inject_json_file("json_data/inject1.json");
@macro inject_json_file("json_data/inject2.json");

int main() {
  double x = 15;

  @meta for(const std::string& name : function_names)
    printf("%s(%f) -> %f\n", @string(name), x, @(name)(x));

  return 0;
}
