/* compat-glibc.c — this glibc (2.22) predates memfd_create (2.27) and
 * close_range (5.9 kernel / glibc 2.34), which glycin's Rust code calls
 * via FFI expecting the system libc to provide them. Implement them here
 * via direct syscalls, and link this into anything that needs them. */
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>

#ifndef SYS_memfd_create
#define SYS_memfd_create 385   /* ARM EABI; verify against your kernel headers */
#endif

int memfd_create(const char *name, unsigned int flags) {
    return syscall(SYS_memfd_create, name, flags);
}

#ifndef SYS_close_range
#define SYS_close_range 436   /* ARM EABI; verify against your kernel headers */
#endif

int close_range(unsigned int first, unsigned int last, unsigned int flags) {
    return syscall(SYS_close_range, first, last, flags);
}
