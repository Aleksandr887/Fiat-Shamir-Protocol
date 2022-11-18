#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <map>
#include <string>

#define NUM_OF_CHECKS 20
#define SEND_BUFF_SIZE 256
#define RECV_BUFF_SIZE 256

// int sock;
// int sock_client;

// void handle_shutdown(int sig)
// {
//     printf("\nShutdown\n");
//     close(sock);
//     close(sock_client);
//     exit(EXIT_SUCCESS);
// }

// void error(std::string msg)
// {
//     std::cerr << msg << ": " << strerror(errno) << '\n';
//     exit(EXIT_FAILURE);
// }

// void reaper(int sig)
// {
//     int status;
//     while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0);
// }
