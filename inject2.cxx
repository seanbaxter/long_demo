#include <cstdio>
#include <fstream>
#include <iostream>
#include "json.hpp"

const char* inject_filename = "json_data/inject1.json";

@meta printf("Opening resource %s\n", inject_filename);
@meta std::ifstream json_file(inject_filename);
@meta nlohmann::json j;
@meta json_file>> j;

// Record the function names in this array.
@meta std::vector<std::string> function_names;

@meta for(auto& item : j.items()) {
  @meta std::string name = item.value()["name"];
  @meta std::string definition = item.value()["definition"];

  @meta printf("Injecting function '%s' = '%s'\n", 
    name.c_str(), definition.c_str());

  double @(name)(double x) {
    return @expression(definition);
  }

  @meta function_names.push_back(std::move(name));
}

int main() {
  double x = 15;

  @meta for(const std::string& name : function_names)
    printf("%s(%f) -> %f\n", @string(name), x, @(name)(x));

  return 0;
}
