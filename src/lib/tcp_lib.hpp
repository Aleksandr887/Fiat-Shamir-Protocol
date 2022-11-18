#pragma once
#include <iostream>
#include <signal.h>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <sys/socket.h>

#define SEND_BUFF_SIZE 256
#define RECV_BUFF_SIZE 256

extern int sock;
extern int sock_client;

namespace CLIENT
{
    void error(std::string msg);
    void connect(int &sock, const char *addr, int port);
    void send_msg(char *n);
    void send_msg(const char *n);
    void send_msg(std::string n);
    template <typename T>
    void send_msg(T n)
    {
        int size = send(sock, &n, sizeof(n), 0);
        if (size < 0)
        {
            error("send");
        }
    }
    std::string recv_msg();
    template <typename T>
    void recv_msg(T &msg)
    {
        if (recv(sock, &msg, sizeof(msg), 0) < 0)
        {
            error("recv");
        }
    }
}

namespace SERVER
{
    void error(std::string msg);
    int creat(int &sock, const char* addr, int port);
    void start(int sock);
    void connection_accept(int sock, int &sock_client);
    void send_msg(char *n);
    void send_msg(const char *n);
    void send_msg(std::string n);
    template <typename T>
    void send_msg(T n)
    {
        int size = send(sock_client, &n, sizeof(n), 0);
        if (size < 0)
        {
            error("send");
        }
    }
    std::string recv_msg();
    template <typename T>
    void recv_msg(T &msg)
    {
        int size = recv(sock_client, &msg, sizeof(msg), 0);
        if (size == -1)
        {
            error("size");
        }
    }
}

void handle_shutdown(int sig);
void error(std::string msg);
void reaper(int sig);