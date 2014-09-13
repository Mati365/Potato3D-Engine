#ifndef STOCKUTILS_HPP_
#define STOCKUTILS_HPP_
#include <string>

using c_str = const std::string&;
using GLuchar = unsigned char;

#define FORCE_INLINE __attribute__((always_inline))
#define REMOVE_FLAG(num, flag) ((num) & ~(flag))
#define IS_SET_FLAG(num, flag) ((num) & (flag))

#define ARRAY_LENGTH(type, array) (sizeof(array) / sizeof(type))
#define IS_IN_MAP(map, key) (map.find(key) != map.cend())

#define INSTANCE_OF(a, type) dynamic_cast<type*>(a)
#define TYPE_IMPORT(ns, type) using ns::type
#define NAMESPACE_IMPORT(ns) using namespace ns

#endif
