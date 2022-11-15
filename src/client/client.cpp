#include "client.hpp"

namespace CLIENT
{
    void error(std::string msg)
    {
        std::cerr << msg << '\n';
        exit(EXIT_FAILURE);
    }

    void connect(int &sock)
    {
        struct sockaddr_in server;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            error("socket");
        }

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = 7777;

        if (connect(sock, (const struct sockaddr *)&server, sizeof(server)) < 0)
        {
            error("connect");
        }
    }

    void send_msg(char *n)
    {
        int size = send(sock, n, strlen(n), 0);
        if (size < 0)
        {
            error("send");
        }
    }

    void send_msg(const char *n)
    {
        int size = send(sock, n, strlen(n), 0);
        if (size < 0)
        {
            error("send");
        }
    }

    void send_msg(std::string n)
    {
        int size = send(sock, n.c_str(), strlen(n.c_str()), 0);
        if (size < 0)
        {
            error("send");
        }
    }

    template <typename T>
    void send_msg(T n)
    {
        int size = send(sock, &n, sizeof(n), 0);
        if (size < 0)
        {
            error("send");
        }
    }

    std::string recv_msg()
    {
        char buf[RECV_BUFF_SIZE];
        memset(buf, 0, sizeof(buf));
        int size = recv(sock, buf, RECV_BUFF_SIZE, 0);
        if (size == -1)
        {
            error("size");
        }

        return std::string(buf);
    }

    template <typename T>
    void recv_msg(T &msg)
    {
        if (recv(sock, &msg, sizeof(msg), 0) < 0)
        {
            error("recv");
        }
    }
}

int main()
{
    srand(time(NULL));
    signal(SIGINT, handle_shutdown);

    char send_buff[SEND_BUFF_SIZE];
    char login[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];

    CLIENT::connect(sock);
    long int n = 0;
    long int v = 0;
    long int y = 0;
    long int x = 0;
    long int r = 0;
    int e = 0;
    long int s = 0;
    int action = 0;
    std::fstream pwd_file;

    CLIENT::recv_msg(n);
    std::cout << "n: " << n << '\n';
    std::cout << CLIENT::recv_msg() << '\n';
    std::cin >> action;
    CLIENT::send_msg(action);
    std::cout << CLIENT::recv_msg() << '\n';
    std::cin >> login;
    CLIENT::send_msg(login);
    strcpy(recv_buff, CLIENT::recv_msg().c_str());
    if (action == 0)
    {
        pwd_file.open(std::string(login) + "_passwd.txt", std::fstream::out | std::fstream::trunc);
        pwd_file.seekp(0);
        std::cout << recv_buff << '\n';
        if (recv_buff[0] == 'T')
        {
            goto exit;
        }
        s = generate_prime_number(n - 1);
        std::string str = std::to_string(s);
        pwd_file << str;
        v = fast_exp(s, 2, n);
        std::cout << "s: " << s << '\n';
        std::cout << "v: " << v << "" << '\n';
        CLIENT::send_msg(v);
        memset(recv_buff, 0, sizeof(recv_buff));
        std::cout << CLIENT::recv_msg() << '\n';
    }
    else
    {
        pwd_file.open(std::string(login) + "_passwd.txt");
        std::string str;
        pwd_file >> str;
        s = std::stol(str);
        v = fast_exp(s, 2, n);
        std::cout << "s: " << s << '\n';
        std::cout << "v: " << v << '\n';
        if (recv_buff[0] == 'O' && recv_buff[1] == 'K')
        {
            for (int i = 0; i < NUM_OF_CHECKS; i++)
            {
                r = rand() % (n - 1) + 1;
                x = fast_exp(r, 2, n);
                std::cout << "x: " << x << '\n';
                CLIENT::send_msg(x);
                CLIENT::recv_msg(e);
                std::cout << "e: " << e << '\n';
                if (e == 0)
                {
                    y = r;
                }
                else
                {
                    y = (r * s) % n;
                }
                CLIENT::send_msg(y);
                strcpy(recv_buff, CLIENT::recv_msg().c_str());
                if (recv_buff[0] != 'O')
                {
                    std::cout << recv_buff << '\n';
                    break;
                }
            }
        }
        else
        {
            std::cout << recv_buff << '\n';
        }
    }
exit:
    close(sock);
    pwd_file.close();
    return 0;
}