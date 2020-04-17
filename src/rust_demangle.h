#ifndef BASE_RUST_DEMANGLE_H_
#define BASE_RUST_DEMANGLE_H_

extern "C" {

char* demangle_rust_symbol(const char* mangled);
void recycle_demangle_result(char* demangled);

};

#endif  // BASE_RUST_DEMANGLE_H_
