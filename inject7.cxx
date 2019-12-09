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

@mvoid declare_enum(std::string name, std::string definition) {
  @meta printf("Injecting enum \"%s\"\n", name.c_str());

  // Make an enum declaration using a semicolon deliminated identifier.
  @(name);
}

enum class my_enum_t {
  @macro visit_json_files("json_data", visit_json_contents, declare_enum);
};

@meta printf("%d: %s\n", int..., @enum_names(my_enum_t)) ...;

int main() {
  return 0;
}