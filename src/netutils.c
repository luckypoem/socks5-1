#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "netutils.h"

int create_v4_socket() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        return -1;
    }

    if (set_nonblocking(fd) < 0) {
        return -1;
    }

    if (set_reuseaddr(fd) < 0) {
        return -1;
    }

    if (set_nosigpipe(fd) < 0) {
        return -1;
    }

    return fd;
}

int create_v6_socket() {
    int fd = socket(AF_INET6, SOCK_STREAM, 0);

    if (fd < 0) {
        return -1;
    }

    if (set_nonblocking(fd) < 0) {
        return -1;
    }

    if (set_reuseaddr(fd) < 0) {
        return -1;
    }

    if (set_nosigpipe(fd) < 0) {
        return -1;
    }

    return fd;
}

int set_nonblocking(int fd) {
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }

    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int set_reuseaddr(int fd) {
    int opt = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
}

int set_nodelay(int fd) {
    int opt = 1;
    return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&opt, sizeof(opt));
}

int set_nosigpipe(int fd) {
#ifdef SO_NOSIGPIPE
    int opt = 1;
    return setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&opt, sizeof(opt));
#endif
    return 0;
}

// int set_nosigpipe(int fd) {
//     int opt = 1;
//     return setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&opt, sizeof(opt));
// }

int strtosockaddr(const char *src, void *addrptr) {
    int ret;

    ret = inet_pton(AF_INET, src, addrptr);
    if (0 < ret) {
        return ret;
    }

    ret = inet_pton(AF_INET6, src, addrptr);
    return ret;
}