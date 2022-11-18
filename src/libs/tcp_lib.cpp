#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include "tcp_lib.hpp"

int sock;
int sock_client;

namespace CLIENT
{
    void error(std::string msg)
    {
        std::cerr << msg << '\n';
        exit(EXIT_FAILURE);
    }

    void connect(int &sock, const char* addr, int port)
    {
        struct sockaddr_in server;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            error("socket");
        }

        server.sin_addr.s_addr = inet_addr(addr);
        server.sin_family = AF_INET;
        server.sin_port = port;

        if (connect(sock, (const struct sockaddr *)&server, sizeof(server)) < 0) {
            error("connect");
        }
    }

    void send_msg(char *n)
    {
        int size = send(sock, n, strlen(n), 0);
        if (size < 0) {
            error("send");
        }
    }

    void send_msg(const char *n)
    {
        int size = send(sock, n, strlen(n), 0);
        if (size < 0) {
            error("send");
        }
    }

    void send_msg(std::string n)
    {
        int size = send(sock, n.c_str(), strlen(n.c_str()), 0);
        if (size < 0) {
            error("send");
        }
    }

    std::string recv_msg()
    {
        char buf[RECV_BUFF_SIZE];
        memset(buf, 0, sizeof(buf));
        int size = recv(sock, buf, RECV_BUFF_SIZE, 0);
        if (size == -1) {
            error("size");
        }

        return std::string(buf);
    }
}

namespace SERVER
{
    void error(std::string msg)
    {
        std::cerr << msg << ": " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }

    int creat(int &sock, const char* addr, int port)
    {
        struct sockaddr_in server;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            error("socket");
        }

        server.sin_addr.s_addr = inet_addr(addr);
        server.sin_family = AF_INET;
        server.sin_port = port;

        int reuseaddr_on = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                       &reuseaddr_on, sizeof(reuseaddr_on)) < 0) {
            error("setsockopt");
        }
        if (bind(sock, (const struct sockaddr *)&server, sizeof(server)) < 0) {
            error("bind");
        }

        return sock;
    }

    void start(int sock)
    {
        if (listen(sock, 5) < 0) {
            error("listen");
        }
    }

    void connection_accept(int sock, int &sock_client)
    {
        sock_client = accept(sock, 0, 0);
        if (sock_client < 0) {
            error("accept");
        }
    }

    void send_msg(char *n)
    {
        int size = send(sock_client, n, strlen(n), 0);
        if (size < 0) {
            error("send");
        }
    }

    void send_msg(const char *n)
    {
        int size = send(sock_client, n, strlen(n), 0);
        if (size < 0) {
            error("send");
        }
    }

    void send_msg(std::string n)
    {
        int size = send(sock_client, n.c_str(), strlen(n.c_str()), 0);
        if (size < 0) {
            error("send");
        }
    }

    std::string recv_msg()
    {
        char buf[RECV_BUFF_SIZE];
        memset(buf, 0, sizeof(buf));
        int size = recv(sock_client, buf, RECV_BUFF_SIZE, 0);
        if (size == -1) {
            error("size");
        }

        return std::string(buf);
    }
}

void handle_shutdown(int sig)
{
    printf("\nShutdown\n");
    close(sock);
    close(sock_client);
    exit(EXIT_SUCCESS);
}

void error(std::string msg)
{
    std::cerr << msg << ": " << strerror(errno) << '\n';
    close(sock);
    close(sock_client);
    exit(EXIT_FAILURE);
}

void reaper(int sig)
{
    int status;
    while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0);
}