#include <string>
#include <fstream>
#include <cstdio>
#include <dirent.h>
#include "json.hpp"

inline std::string get_extension(const std::string& filename) {  
  return filename.substr(filename.find_last_of(".") + 1);
}

inline bool match_extension(const char* filename, const char* ext) {
  return ext == get_extension(filename);
}

template<typename func_t, typename cookie_t>
@mvoid visit_json_files(
  std::string dirname, 
  func_t callback, 
  cookie_t cookie
) {

  @meta DIR* dir = opendir(dirname.c_str());
  @meta while(dirent* ent = readdir(dir)) {
    @meta if(match_extension(ent->d_name, "json")) {
      // ent->d_name is relative to the current directory.
      // Prepend the dirname for a relative path.
      @meta std::string filename = dirname + "/" + ent->d_name;

      // Expand the callback macro and pass the filename and the cookie.
      @macro callback(filename.c_str(), cookie);
    }
  }

  @meta closedir(dir);
}

@mvoid visit_json_contents(
  const char* filename, 
  @mvoid(*callback)(std::string, std::string)
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

// Everything above is properly a library function/macro.

@meta std::vector<std::string> function_names;

@mvoid declare_function(std::string name, std::string definition) {
  @meta printf("Injecting function \"%s\" = \"%s\"\n", name.c_str(), 
    definition.c_str());

  double @(name)(double x) {
    return @expression(definition);
  }

  @meta function_names.push_back(name);
}

@macro visit_json_files("json_data", visit_json_contents, declare_function);


int main() {
  double x = 15;

  @meta for(const std::string& name : function_names)
    printf("%s(%f) -> %f\n", @string(name), x, @(name)(x));

  return 0;
}
