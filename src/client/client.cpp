#include <fstream>
#include "tcp_lib.hpp"
#include "operations.hpp"

extern int sock;
extern int sock_client;

int main()
{
    srand(time(NULL));
    signal(SIGINT, handle_shutdown);

    char login[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];

    CLIENT::connect(sock, "127.0.0.1", 7777);
    uint64_t n = 0;
    uint64_t v = 0;
    uint64_t y = 0;
    uint64_t x = 0;
    uint64_t r = 0;
    uint16_t e = 0;
    uint64_t s = 0;
    uint16_t action;
    std::fstream pwd_file;

    CLIENT::recv_msg(n);
    std::cout << "n: " << n << '\n';
    std::cout << CLIENT::recv_msg() << '\n';
    std::cin >> action;
    if (action == 0 || action > 2) {
        std::cout << "Action must be '1' or '2'\n";
        goto exit;
    }

    CLIENT::send_msg(action);
    std::cout << CLIENT::recv_msg() << '\n';
    std::cin >> login;
    CLIENT::send_msg(login);
    strcpy(recv_buff, CLIENT::recv_msg().c_str());

    if (action == 1) {
        pwd_file.open(std::string(login) + "_passwd.txt", std::fstream::out | std::fstream::trunc);
        pwd_file.seekp(0);
        std::cout << recv_buff << '\n';
        if (recv_buff[0] == 'T') {
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
    } else if (action == 2) {
        if (recv_buff[0] == 'T') {
            std::cout << recv_buff << '\n';
            goto exit;
        }

        pwd_file.open(std::string(login) + "_passwd.txt");
        std::string str;
        pwd_file >> str;
        s = std::stol(str);
        v = fast_exp(s, 2, n);
        std::cout << "s: " << s << '\n';
        std::cout << "v: " << v << '\n';

        if (recv_buff[0] == 'O' && recv_buff[1] == 'K') {
            for (int i = 0; i < NUM_OF_CHECKS; i++) {
                r = rand() % (n - 1) + 1;
                x = fast_exp(r, 2, n);
                std::cout << "x: " << x << '\n';
                CLIENT::send_msg(x);
                CLIENT::recv_msg(e);
                std::cout << "e: " << e << '\n';
                if (e == 0) {
                    y = r;
                } else {
                    y = (r * s) % n;
                }
                CLIENT::send_msg(y);
                strcpy(recv_buff, CLIENT::recv_msg().c_str());
                if (recv_buff[0] != 'O') {
                    std::cout << recv_buff << '\n';
                    break;
                }
            }
        } else {
            std::cout << recv_buff << '\n';
        }
    }

exit:
    close(sock);
    pwd_file.close();
    return 0;
}