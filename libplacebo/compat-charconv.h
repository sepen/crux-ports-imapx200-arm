/* compat-charconv.h — GCC 7's libstdc++ has no <charconv> at all, so the
 * name std::to_chars/std::from_chars doesn't exist, and decltype(...) on
 * a nonexistent name is a hard compile error, NOT a SFINAE-friendly
 * substitution failure. Declare dummy overloads that only match an unused
 * placeholder type, so the *name* exists (avoiding the hard error) while
 * substitution against real argument types (char*, int, float, ...)
 * still correctly fails, letting has_std_to_chars/has_std_from_chars
 * evaluate to false as intended. */
#ifndef COMPAT_CHARCONV_H
#define COMPAT_CHARCONV_H

namespace std {

struct __compat_charconv_unused_placeholder {};

inline void to_chars(__compat_charconv_unused_placeholder,
                      __compat_charconv_unused_placeholder,
                      __compat_charconv_unused_placeholder) {}

inline void from_chars(__compat_charconv_unused_placeholder,
                        __compat_charconv_unused_placeholder,
                        __compat_charconv_unused_placeholder&) {}

}  // namespace std

#endif
