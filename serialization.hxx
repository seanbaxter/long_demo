#pragma once
#include <sstream>
#include <type_traits>
#include <map>
#include <vector>
#include <array>
#include <string>
#include <optional>

template<typename type_t>
const char* enum_to_name(type_t e) {
  switch(e) {
    @meta for enum(type_t e2 : type_t) {
      case e2:
        return @enum_name(e2);
    }

    default:
      return nullptr;
  }
}

template<typename type_t>
void stream(std::ostream& os, const type_t& obj, int indent = 0) {

  os<< @type_string(type_t)<< " ";

  if constexpr(std::is_enum<type_t>::value) {
    os<< '\"';
    if(const char* name = enum_to_name<type_t>(obj))
      // Write the enumerator name if the value maps to an enumerator.
      os<< name;
    else
      // Otherwise cast the enum to its underlying type and write that.
      os<< (typename std::underlying_type<type_t>::type)obj;
    os<< '\"';

  } else if constexpr(@is_class_template(type_t, std::basic_string)) {
    // Carve out an exception for strings. Put the text of the string
    // in quotes. We could go further and add character escapes back in.
    os<< '\"'<< obj<< '\"';

  } else if constexpr(@is_class_template(type_t, std::vector) ||
    @is_class_template(type_t, std::array)) {
    // Special treatment for std::vector. Output each element in a comma-
    // separated list in brackets.
    os<< "[";
    bool insert_comma = false;
    for(const auto& x : obj) {
      // Move to the next line and indent.
      if(insert_comma)
        os<< ',';
      os<< "\n"<< std::string(2 * (indent + 1), ' ');
      
      // Stream the element.
      stream(os, x, indent + 1);

      // On the next go-around, insert a comma before the newline.
      insert_comma = true;
    }
    os<< "\n"<< std::string(2 * indent, ' ')<< "]";

  } else if constexpr(@is_class_template(type_t, std::map)) {
    // Special treatment for std::map.
    os<< "{";
    bool insert_comma = false;
    for(const auto& x : obj) {
      if(insert_comma)
        os<< ",";
      os<< "\n"<< std::string(2 * (indent + 1), ' ');

      // Stream key : value, where the key and value are done recursively.
      stream(os, x.first, indent + 1);
      os<< " : ";
      stream(os, x.second, indent + 1);

      // On the next go-around, insert a comma before the newline.
      insert_comma = true;
    }  
    os<< "\n"<< std::string(2 * indent, ' ')<< "}";

  } else if constexpr(@is_class_template(type_t, std::optional)) {
    // For an optional member, either stream the value or stream "null".
    if(obj)
      stream(os, *obj, indent);
    else
      os<< "null";

  } else if constexpr(std::is_class<type_t>::value) {
    // For any other class, treat with circle's introspection.
    os<< "{";
    bool insert_comma = false;
    @meta for(size_t i = 0; i < @member_count(type_t); ++i) {
      if(insert_comma) 
        os<< ",";
      os<< "\n"<< std::string(2 * (indent + 1), ' ');

      // Stream the name of the member. The type will be prefixed before the
      // value.
      os<< @member_name(type_t, i)<< " : ";

      // Stream the value of the member.
      stream(os, @member_value(obj, i), indent + 1);

      // On the next go-around, insert a comma before the newline.
      insert_comma = true;
    }
    os<< "\n"<< std::string(2 * indent, ' ')<< "}";

  } else {
    // For any non-class type, use the iostream overloads.
    os<< '\"'<< obj<< '\"';
  }
}

template<typename type_t>
void stream_simple(std::ostream& os, const type_t& obj) {

  if constexpr(std::is_enum<type_t>::value) {
    if(const char* name = enum_to_name<type_t>(obj))
      // Write the enumerator name if the value maps to an enumerator.
      os<< name;
    else
      // Otherwise cast the enum to its underlying type and write that.
      os<< (typename std::underlying_type<type_t>::type)obj;

  } else if constexpr(@is_class_template(type_t, std::basic_string)) {
    // Carve out an exception for strings. Put the text of the string
    // in quotes. We could go further and add character escapes back in.
    os<< obj;

  } else if constexpr(@is_class_template(type_t, std::vector) ||
    @is_class_template(type_t, std::array)) {
    // Special treatment for std::vector. Output each element in a comma-
    // separated list in brackets.
    os<< "[";
    bool insert_comma = false;
    for(const auto& x : obj) {
      // Move to the next line and indent.
      if(insert_comma)
        os<< ',';
      os<< " ";
      
      // Stream the element.
      stream_simple(os, x);

      // On the next go-around, insert a comma before the newline.
      insert_comma = true;
    }
    os<< " ]";

  } else if constexpr(@is_class_template(type_t, std::map)) {
    // Special treatment for std::map.
    os<< "{";
    bool insert_comma = false;
    for(const auto& x : obj) {
      if(insert_comma)
        os<< ",";
      os<< " ";

      // Stream key : value, where the key and value are done recursively.
      stream_simple(os, x.first);
      os<< " : ";
      stream_simple(os, x.second);

      // On the next go-around, insert a comma before the newline.
      insert_comma = true;
    }  
    os<< " }";

  } else if constexpr(@is_class_template(type_t, std::optional)) {
    // For an optional member, either stream the value or stream "null".
    if(obj)
      stream_simple(os, *obj);
    else
      os<< "null";

  } else if constexpr(std::is_class<type_t>::value) {
    // For any other class, treat with circle's introspection.
    os<< "{";
    bool insert_comma = false;
    @meta for(size_t i = 0; i < @member_count(type_t); ++i) {
      if(insert_comma) 
        os<< ",";
      os<< " ";

      // Stream the name of the member. The type will be prefixed before the
      // value.
      os<< @member_name(type_t, i)<< " : ";

      // Stream the value of the member.
      stream_simple(os, @member_value(obj, i));

      // On the next go-around, insert a comma before the newline.
      insert_comma = true;
    }
    os<< " }";

  } else {
    // For any non-class type, use the iostream overloads.
    os<< obj;
  }
}


template<typename type_t> 
std::string stream(const type_t& obj, int indent = 0) {
  std::ostringstream oss;
  stream(oss, obj, indent);
  return oss.str();
}

template<typename type_t> 
std::string stream_simple(const type_t& obj) {
  std::ostringstream oss;
  stream_simple(oss, obj);
  return oss.str();
}
