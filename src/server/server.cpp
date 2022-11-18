#include "operations.hpp"
#include "tcp_lib.hpp"
#include <map>

class ServerBackend
{
private:
    uint64_t n_;
    uint64_t p_;
    uint64_t q_;
    uint64_t x_;
    uint64_t y_;
    uint16_t e_;
    std::map<std::string, uint64_t> open_keys;

public:
    ServerBackend()
    {
        srand(time(NULL));
        p_ = generate_prime_number();
        q_ = generate_prime_number();
        n_ = p_ * q_;
    }

    uint64_t n() { return n_; }

    uint64_t p() { return p_; }

    uint64_t q() { return q_; }

    void set_x(uint64_t x) { x_ = x; }

    void set_y(uint64_t y) { y_ = y; }

    int get_random_e()
    {
        e_ = rand() % 2;
        return e_;
    }

    uint16_t e() { return e_; }

    int check(std::string login)
    {
        std::cout << "y^2: " << fast_exp(y_, 2, n_) << '\n';
        if (e_ == 0) {
            return (fast_exp(y_, 2, n_) == (x_));
        }

        return (fast_exp(y_, 2, n_) == ((x_ * open_keys[login]) % n_));
    }

    bool exists(std::string login)
    {
        return (open_keys.count(login) > 0);
    }

    void register_new_user(std::string login, uint64_t v)
    {
        open_keys[login] = v;
        for (auto &item : open_keys) {
            std::cout << item.first << "->" << item.second << "\n";
        }
    }
};

extern int sock;
extern int sock_client;

int main()
{
    signal(SIGINT, handle_shutdown);
    signal(SIGCHLD, reaper);

    ServerBackend data;
    char ag = 2;
    uint64_t n = data.n();
    std::string login;
    uint16_t action = 0;
    SERVER::creat(sock, "127.0.0.1", 7777);
    SERVER::start(sock);

    while (1) {
        SERVER::connection_accept(sock, sock_client);
        SERVER::send_msg(n);
        action = 0;
        std::cout << "n: " << n << '\n';
        SERVER::send_msg("Enter '1' to register, '2' to log in: ");

        SERVER::recv_msg(action);
        if (action == 0) {
            std::cout << "User disconnected\n";
            continue;
        }

        std::cout << "action = " << action << "\n";
        SERVER::send_msg("Enter your login: ");
        login = SERVER::recv_msg();

        if (action == 2) {
            if (!data.exists(login)) {
                std::cout << "This user does not exist\n";
                SERVER::send_msg("This user does not exist");
                close(sock_client);
                continue;
            }
            SERVER::send_msg("OK");
            uint64_t x = 0;
            uint64_t y = 0;

            for (int i = 0; i < NUM_OF_CHECKS; i++) {
                x = 0;
                SERVER::recv_msg(x);
                std::cout << "x: " << x << '\n';
                data.get_random_e();
                SERVER::send_msg(data.e());
                y = 0;
                SERVER::recv_msg(y);
                data.set_y(y);
                data.set_x(x);

                if (!data.check(login)) {
                    SERVER::send_msg("Access denied");
                    close(sock_client);
                    ag = 0;
                    break;
                } else {
                    if (i + 1 == NUM_OF_CHECKS)
                        break;
                    SERVER::send_msg("OK");
                }
            }
            if (ag == 0) {
                ag = 2;
                continue;
            }
            SERVER::send_msg("Access granted");
            close(sock_client);
        } else {
            if (data.exists(login)) {
                SERVER::send_msg("This login already taken");
                close(sock_client);
                continue;
            }

            SERVER::send_msg("Getting your open key...");
            uint64_t v = 0;
            SERVER::recv_msg(v);
            data.register_new_user(login, v);
            SERVER::send_msg("You're successfully registered!");
            close(sock_client);
        }
        close(sock_client);
    }

    return 0;
}