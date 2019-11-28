# Comments on the Circle interpreter

My goal with Circle was to support execution of any existing C++ code at compile time. Simply `#include` a library and use any types and functions during source translation. Use `-M` command-line arguments to include function definitions and object initializers that were compiled into shared-object libraries.

Any C++ compiler will have an interpreter for constant folding. C++11 and later requires more capable interpreters for constexpr evaluation. But an interpreter by itself doesn't allow you to execute very much. You're going to need foreign function calls for dynamic memory, i/o, exception handling, RTTI and everything else.

All C++ code is built on functions and objects that are externally defined in libraries like libc, libm, libstdc++ (::operator new, much of iostreams) and libc++abi (cxa_throw, the dynamic_cast algorithm). Interoperability requires the interpreter implement the full System V and Itanium C++ ABIs (or whatever the ABIs for your platform) down to the last particular, plus some creative engineering to make compile-time declarations appear like compiled code to your binary dependencies.

There are three general strategies used to run anything in the Circle interpreter:
1. Adopt C++ ABI data layout. All objects in the interpreter are laid out in compliance with the C++ ABI. This includes bit-field layout, virtual base class layout, and so on. We can pass objects by value, or pass by reference, to functions in compiled code.
1. FFI closures are generated whenever the address of a locally-defined function is required. The function can be called through this address inside the interpreter, or passed to compiled code and called from there. The closure's callback is implemented by the interpreter, which walks the AST to execute the function.
1. Real C++ exception handling is implemented in the interpreter. _throw-expression_ is implemented with a call to `__cxa_throw`, and catch clauses are implemented with RTTI checks on the caught exception, which is equivalent to the sequence emitted in compiled catch clauses.

## FFI closures

Foreign function/object linking is performed when trying to convert a function/object lvalue to an address (e.g. immediately preceding a call, or when decaying to a function pointer) only if a local definition for that function/object is not available. The declaration's name is mangled and this string is searched for in a list of pre-loaded dependencies (libc, libm, libpthread, libstdc++) and in user-specified dependencies (use the -M argument; I do this in the libregex.so and libapex.so demos).

Alternatively, if the function is defined locally, an FFI closure is allocated, which binds a callback function and a cookie to a callable address: this is the function address used within the compiler, and may be shared with compiled code.

[**qsort.cxx**](qsort.cxx)
```cpp
template<typename type_t>
int cmp_pred(const void* a, const void* b) {
  type_t x = *static_cast<const type_t*>(a);
  type_t y = *static_cast<const type_t*>(b);
  return x < y ? -1 : x > y ? +1 : 0;
}

template<typename type_t>
void sort_vec(std::vector<type_t>& vec) {
  qsort(vec.data(), vec.size(), sizeof(type_t), cmp_pred<type_t>);
}
```

qsort is externally defined in libc.so.6. When executed from the interpreter, Circle makes a foreign function call. An FFI closure is	allocated for the cmp_pred specialization. The compiled qsort code calls into this closure, returning control to the compiler, and the Circle interpreter executes cmp_pred by traversing its AST. The result object is returned via the closure, and execution resumes in libc, which will return control to the interpreter when finished sorting its argument.

Circle tries to elide calls through its own FFI closure--it keeps a table of all closures by address and the functions they map to. It first checks a function address with this table, and only if it's not in the table does it execute an FFI call (otherwise it interprets the function from its AST).

This table is required by constexpr/meta object portability: a _literal type_ may contain references or pointers to functions or to objects with static storage duration, as well as pointers to string literals. Because Circle uses System V/Itanium ABI object layout, constexpr/meta objects are just binary blobs. The interpreter needs to map function and object/subobject addresses to their corresponding declarations--it uses the closure map to do this for functions, and object map to do it for objects and a string literal map to do it for character pointers. 

## Virtual things

FFI closures are used heavily, especially when instantiating objects with vtables: each vfunc slot is set to a closure address. We can't emulate virtual function calls, because the object may be passed to a foreign function which makes virtual function calls on it, and expects a real callable pointer (iostreams does this). 

Additionally, the negative-address vtable slots pertaining to virtual inheritance must be set in the interpreter-generated vtables and recognized by the interpreter in binary-stored vtables, so that downcasts/sidecasts through virtual base classes are correctly executed (iostreams also does this).

Everyone is wowed by `@meta printf`, which is a trivial FFI invoke. Nobody is moved to tears by `@meta std::cout<< "Hello world\n";`, but I cried a lot trying to make it work. 

## Exception handling

Circle supports real C++ exceptions at compile time. They may be thrown from binary code and caught by meta code, or vice-versa.

[**throw.cxx**](throw.cxx)
```cpp
#include <stdexcept>

void throw_func(const char* msg) {
	printf("Throwing an exception with msg '%s'\n", msg);
	throw std::runtime_error(msg);
}
```
```
$ circle throw.cxx -o libthrow.so
$ nm libthrow.so | grep throw_func
0000000000000920 T _Z10throw_funcPKc
```

[**catch.cxx**](catch.cxx)
```cpp
#include <stdexcept>
#include <cstdio>

// Forward declare a function defined in libthrow.so. The function is
// invoked with a foreign function call. It throws a real C++ exception,
// which is caught by a direct or indirect meta try clause.
void throw_func(const char* msg);

// Catch an exception in a meta-try block.
@meta try {
	@meta throw_func("direct meta-try");

} catch(std::exception& e) {
	@meta printf("Caught exception '%s' in meta-try\n", e.what());
}

// Catch an exception in a normal function, called from a meta
// expression statement.
inline void try_test() {
	try {
		throw_func("indirect meta-try");

	} catch(std::exception& e) {
		printf("Caught exception '%s' in try_test\n", e.what());
	}
}

// Execute try_test at compile time.
@meta try_test();

int main() {
	return 0;
}
```
```
$ circle catch.cxx -M libthrow.so 
Throwing an exception with msg 'direct meta-try'
Caught exception 'direct meta-try' in meta-try
Throwing an exception with msg 'indirect meta-try'
Caught exception 'indirect meta-try' in try_test
```
Compile `throw.cxx` into an ordinary shared object library, `libthrow.so`. Compile `catch.cxx` into an executable and use -M to load `libthrow.so` as a compile-time resource. 

`throw_func` isn't declared in catch.cxx, so the interpreter is forced to perform a foreign function call. It searches for the mangled name `_Z10throw_funcPKc` and finds it in the shared object. Circle uses `dlsym` to extract the function address, and invokes it with the argument string. The shared object throws a C++ exception (using `__cxa_throw` internally, from libc++abi). The stack is unwound through the FFI mechanism. The Circle interpreter implements the set of meta catch clauses with a `catch(...)` mechanism: `__cxa_current_exception_type` retrieves the `std::type_info` of the thrown exception, and `std::type_info::__do_catch` matches the caught exception against each of the programmer-specified catch clause types. This exactly correlates to the catch implementation emitted by native code on Itanium ABI platforms.

## FFI for performance

When a function is locally defined when called, it is executed through the interpreter. This provides superior error reporting and robustness compared to binary execution, but at a tremendous performance cost. The developer can move the definitions of long-running functions from a header file into a .cpp file, and build those .cpp files into a binary dependency for the project.

Adding a C++ attribute on functions to prefer binary definitions over local definitions is an obvious enhancement. 