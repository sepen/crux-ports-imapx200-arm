/* Must come first: forces glibc to expose GNU/BSD extensions
 * (AT_NO_AUTOMOUNT, asprintf, mempcpy, etc.) in every header pulled in
 * after this file, including the ones psmisc's own sources include later. */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

/* compat-statx.h glibc 2.22 has neither struct statx nor its syscall/flags,
 * so provide the missing kernel ABI pieces (statx() added in Linux 4.11 /
 * glibc 2.28) as a compile-time shim for older systems like this one. */
#ifndef PSMISC_COMPAT_STATX_H
#define PSMISC_COMPAT_STATX_H

#include <sys/syscall.h>
#include <stdint.h>

#ifndef AT_STATX_SYNC_AS_STAT
#define AT_STATX_SYNC_AS_STAT   0x0000
#define AT_STATX_FORCE_SYNC     0x2000
#define AT_STATX_DONT_SYNC      0x4000
#endif

#ifndef STATX_TYPE
#define STATX_TYPE        0x00000001U
#define STATX_MODE        0x00000002U
#define STATX_NLINK       0x00000004U
#define STATX_UID         0x00000008U
#define STATX_GID         0x00000010U
#define STATX_ATIME       0x00000020U
#define STATX_MTIME       0x00000040U
#define STATX_CTIME       0x00000080U
#define STATX_INO         0x00000100U
#define STATX_SIZE        0x00000200U
#define STATX_BLOCKS      0x00000400U
#define STATX_BASIC_STATS 0x000007ffU
#define STATX_BTIME       0x00000800U
#define STATX_ALL         0x00000fffU
#endif

#ifndef SYS_statx
#define SYS_statx 291   /* ARM EABI (32-bit) */
#endif

#ifndef _STATX_H
struct statx_timestamp {
    int64_t  tv_sec;
    uint32_t tv_nsec;
    int32_t  __reserved;
};

struct statx {
    uint32_t stx_mask;
    uint32_t stx_blksize;
    uint64_t stx_attributes;
    uint32_t stx_nlink;
    uint32_t stx_uid;
    uint32_t stx_gid;
    uint16_t stx_mode;
    uint16_t __spare0[1];
    uint64_t stx_ino;
    uint64_t stx_size;
    uint64_t stx_blocks;
    uint64_t stx_attributes_mask;
    struct statx_timestamp stx_atime;
    struct statx_timestamp stx_btime;
    struct statx_timestamp stx_ctime;
    struct statx_timestamp stx_mtime;
    uint32_t stx_rdev_major;
    uint32_t stx_rdev_minor;
    uint32_t stx_dev_major;
    uint32_t stx_dev_minor;
    uint64_t __spare2[14];
};
#endif

#endif /* PSMISC_COMPAT_STATX_H */
