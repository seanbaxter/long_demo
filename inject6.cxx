#include <string>
#include <fstream>
#include <cstdio>
#include "json.hpp"

@macro void visit_json_contents(
  const char* filename, 
  void(*callback)(std::string, std::string)
) {

  @meta printf("Opening file %s\n", filename);

  @meta std::ifstream json_file(filename);
  @meta nlohmann::json j;
  @meta json_file>> j;

  // For each item in the json...
  @meta for(auto& item : j.items()) {
    // Extract the key/value pair and expand the callback macro.
    @meta std::string name = item.value()["name"];
    @meta std::string definition = item.value()["definition"];
    @macro callback(name, definition);
  }
}

@meta std::vector<std::string> function_names;

@macro void declare_function(std::string name, std::string definition) {
  @meta printf("Injecting function \"%s\" = \"%s\"\n", name.c_str(), 
    definition.c_str());

  double @(name)(double x) {
    return @expression(definition);
  }

  @meta function_names.push_back(name);
}

template<const char filename[]>
struct my_injected_t {
  // Expand the macro *at instantiation*.
  @macro visit_json_contents(filename, declare_function);
};

int main() {
  my_injected_t<"json_data/inject1.json"> injected { };
  double x = 15;

  @meta for(const std::string& name : function_names)
    printf("%s(%f) -> %f\n", @string(name), x, injected.@(name)(x));

  return 0;
}
