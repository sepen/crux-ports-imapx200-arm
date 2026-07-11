/* compat-reallocarray.h — glibc 2.22 predates reallocarray() (added in
 * glibc 2.26), so <stdlib.h> never declares it. xorg-server already bundles
 * its own os/reallocarray.c fallback implementation and links it in when
 * HAVE_REALLOCARRAY is false, but nothing declares the prototype in this
 * translation unit — add just the declaration, not a new definition, so it
 * resolves against xorg-server's own bundled symbol at link time. */
#ifndef COMPAT_REALLOCARRAY_H
#define COMPAT_REALLOCARRAY_H
#include <stddef.h>
void *reallocarray(void *ptr, size_t nmemb, size_t size);
#endif
