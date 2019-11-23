# New examples for circle compiler
** https://www.circle-lang.org/ **

## [tuple1.cxx](tuple1.cxx)

```cpp
#include <iostream>

template<typename... types_t>
struct tuple_t {
  @meta for(int i = 0; i < sizeof...(types_t); ++i)
    types_t...[i] @(i);
};

template<typename... types_t>
tuple_t(types_t... args) -> tuple_t<types_t...>;

template<typename type_t>
void print_object(const type_t& obj) {
  @meta for(int i = 0; i < @member_count(type_t); ++i)
    std::cout<< 
      @member_decl_string(type_t, i)<< ": "<<
      @member_value(obj, i)<< "\n";
}

@meta print_object(tuple_t {
  5, "Hello tuple - loop style", 1.618
});

int main() {
  return 0;
}
```
```
$ circle tuple1.cxx
int _0: 5
const char* _1: Hello tuple - loop style
double _2: 1.618
```



## [tuple2.cxx](tuple2.cxx)

