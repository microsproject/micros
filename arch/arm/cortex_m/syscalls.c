/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <errno.h>
#include <reent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/*--------------------------------------------------------------------------------------------------------------------*/
extern char __heap_start__;
extern char __heap_end__;
static char* heap_cur = &__heap_start__;

extern void stdout_putc(char c); /* provided by soc/system.c */
/*--------------------------------------------------------------------------------------------------------------------*/
void* _sbrk(ptrdiff_t incr) {
    char* prev = heap_cur;
    char* next = heap_cur + incr;

    if (next > &__heap_end__) {
        errno = ENOMEM;
        return (void*)-1;
    }
    heap_cur = next;
    return prev;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _open(const char* path, int flags, int mode) {
    // open a file in MicrOS VFS
    (void)path;   // currently unused
    (void)flags;  // currently unused
    (void)mode;   // currently unused
    int fd = 0;
    if (fd < 0) {
        return -1;
    }
    return fd;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _close(int fd) {
    // close a file in MicrOS VFS
    (void)fd;  // currently unused
    int rc = 0;
    if (rc < 0) {
        return -1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
_ssize_t _read(int fd, void* buf, size_t n) {
    (void)fd;
    (void)buf;
    (void)n;
    // read from a file in MicrOS VFS
    ssize_t rc = 0;
    if (rc < 0) {
        return -1;
    }
    return rc;
}
/*--------------------------------------------------------------------------------------------------------------------*/
_ssize_t _write(int fd, const void* buf, size_t n) {
    // write to a file in MicrOS VFS
    (void)fd;
    const char* p = (const char*)buf;
    for (size_t i = 0; i < n; i++) {
        if (p[i] == '\n') {
            stdout_putc('\r');
        }
        stdout_putc(p[i]);
    }
    ssize_t rc = (ssize_t)n;
    if (rc < 0) {
        return -1;
    }
    return rc;
}
/*--------------------------------------------------------------------------------------------------------------------*/
_off_t _lseek(int fd, _off_t off, int whence) {
    (void)fd;
    (void)off;
    (void)whence;
    // seek in a file in MicrOS VFS
    off_t rc = 0;
    if (rc < 0) {
        return -1;
    }
    return rc;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _fstat(int fd, struct stat* st) {
    (void)fd;
    st->st_mode = S_IFCHR; /* character device */
    st->st_nlink = 1;
    st->st_size = 0;
    int rc = 0; /* implement minimal in VFS: set st_mode, st_size */
    if (rc < 0) {
        return -1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _isatty(int fd) {
    (void)fd;
    return 1; /* assume all fds are TTYs for simplicity */
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _unlink(const char* path) {
    (void)path;
    int rc = 0; /* implement in VFS */
    if (rc < 0) {
        return -1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _gettimeofday(struct timeval* tv, void* tz) {
    /* optional: wire to your tick counter */
    (void)tz;
    if (tv) {
        extern uint64_t arch_uptime_ticks(void);
        extern uint32_t arch_ticks_per_sec(void);
        uint64_t t = arch_uptime_ticks();
        tv->tv_sec = t / arch_ticks_per_sec();
        tv->tv_usec =
            (t % arch_ticks_per_sec()) * (1000000 / arch_ticks_per_sec());
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _getpid(void) {
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
