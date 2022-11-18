#include "operations.hpp"
#include "tcp_lib.hpp"
#include <map>

// namespace SERVER
// {
//     void error(std::string msg)
//     {
//         std::cerr << msg << ": " << strerror(errno) << '\n';
//         exit(EXIT_FAILURE);
//     }

//     int creat(int &sock, const char* addr, int port)
//     {
//         struct sockaddr_in server;
//         sock = socket(AF_INET, SOCK_STREAM, 0);
//         if (sock < 0)
//         {
//             error("socket");
//         }
//         // server.sin_addr.s_addr = inet_addr("127.0.0.1");
//         server.sin_addr.s_addr = inet_addr(addr);
//         server.sin_family = AF_INET;
//         // server.sin_port = 7777;
//         server.sin_port = port;
//         int reuseaddr_on = 1;
//         if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
//                        &reuseaddr_on, sizeof(reuseaddr_on)) < 0)
//         {
//             error("setsockopt");
//         }
//         if (bind(sock, (const struct sockaddr *)&server, sizeof(server)) < 0)
//         {
//             error("bind");
//         }
//         return sock;
//     }

//     void start(int sock)
//     {
//         if (listen(sock, 5) < 0)
//         {
//             error("listen");
//         }
//     }

//     void connection_accept(int sock, int &sock_client)
//     {
//         sock_client = accept(sock, 0, 0);
//         if (sock_client < 0)
//         {
//             error("accept");
//         }
//     }

//     void send_msg(char *n)
//     {
//         int size = send(sock_client, n, strlen(n), 0);
//         if (size < 0)
//         {
//             error("send");
//         }
//     }

//     void send_msg(const char *n)
//     {
//         int size = send(sock_client, n, strlen(n), 0);
//         if (size < 0)
//         {
//             error("send");
//         }
//     }

//     void send_msg(std::string n)
//     {
//         int size = send(sock_client, n.c_str(), strlen(n.c_str()), 0);
//         if (size < 0)
//         {
//             error("send");
//         }
//     }

//     template <typename T>
//     void send_msg(T n)
//     {
//         int size = send(sock_client, &n, sizeof(n), 0);
//         if (size < 0)
//         {
//             error("send");
//         }
//     }

//     std::string recv_msg()
//     {
//         char buf[RECV_BUFF_SIZE];
//         memset(buf, 0, sizeof(buf));
//         int size = recv(sock_client, buf, RECV_BUFF_SIZE, 0);
//         if (size == -1)
//         {
//             error("size");
//         }

//         return std::string(buf);
//     }

//     template <typename T>
//     void recv_msg(T &msg)
//     {
//         int size = recv(sock_client, &msg, sizeof(msg), 0);
//         if (size == -1)
//         {
//             error("size");
//         }
//     }
// }


// Класс, который хранит все вспомогательные элементы
class ServerBackend
{
private:
    long int n_; // p*q
    long int p_;
    long int q_;
    long int x_; // число, которое пользователь присылает в начале проверки
    long int y_; // число, также посылаемого пользователем, квадрат которого будет проверяться
    long int e_; // бит 0,1
    // Словарь [Логин] -> v(значение открытого ключа, который прислал пользователь)
    std::map<std::string, long int> open_keys;
    // long int s_;
    // long int v_;

public:
    ServerBackend()
    {
        srand(time(NULL));
        p_ = generate_prime_number();
        q_ = generate_prime_number();
        n_ = p_ * q_;
    }
    
    long int n() { return n_; }

    long int p() { return p_; }

    long int q() { return q_; }

    void set_x(long int x) { x_ = x; }

    void set_y(long int y) { y_ = y; }

    int get_random_e()
    {
        e_ = rand() % 2;
        return e_;
    }

    int e() { return e_; }

    int check(std::string login)
    {
        std::cout << "y^2: " << fast_exp(y_, 2, n_) << '\n';
        if (e_ == 0)
        {
            return (fast_exp(y_, 2, n_) == (x_));
        }
        return (fast_exp(y_, 2, n_) == ((x_ * open_keys[login]) % n_));
    }

    bool exists(std::string login)
    {
        return (open_keys.count(login) > 0);
    }

    void register_new_user(std::string login, long int v)
    {
        open_keys[login] = v;
        for (auto &item : open_keys)
        {
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
    long int n = data.n();
    std::string login;
    int action;
    SERVER::creat(sock, "127.0.0.1", 7777);
    SERVER::start(sock);

    while (1)
    {
        SERVER::connection_accept(sock, sock_client);
        SERVER::send_msg(n);
        std::cout << "n: " << n << '\n';
        SERVER::send_msg("enter '0' to register, '1' to log in: ");
        action = 0; // 0 - для регистрации, 1 - для входа(для зарегистрированных пользователей)
        SERVER::recv_msg(action);
        // Просим пользователя ввести логин. Он нужен как для регистрации, так и для авторизации
        SERVER::send_msg("enter your login: ");
        // Получаем логин от пользователя
        login = SERVER::recv_msg();

        if (action)
        {
            // Вход в систему
            if (!data.exists(login))
            {
                std::cout << "this user does not exist\n";
                SERVER::send_msg("this user does not exist");
                close(sock_client);
                continue;
            }
            SERVER::send_msg("OK");
            long int x = 0;
            long int y = 0;
            // Сама проверка
            for (int i = 0; i < NUM_OF_CHECKS; i++)
            {
                x = 0;
                SERVER::recv_msg(x);
                std::cout << "x: " << x << '\n';
                data.get_random_e();
                SERVER::send_msg(data.e());
                y = 0;
                SERVER::recv_msg(y);
                data.set_y(y);
                data.set_x(x);
                if (!data.check(login))
                {
                    SERVER::send_msg("access denied");
                    close(sock_client);
                    ag = 0;
                    break;
                }
                else
                {
                    if (i + 1 == NUM_OF_CHECKS)
                        break;
                    // Посылается сообщение "ОК", говорящее о том, чтобы пользователь продолжал проверку
                    SERVER::send_msg("OK");
                }
            }
            // Чтобы не использовать goto и выйти из вложенного цикла, используем переменную ag
            // Если ag(access granted) == 0, то мы хотим отбросить текущего пользователя
            // И ожидать работы с другими.
            if (ag == 0)
            {
                ag = 2;
                continue;
            }
            SERVER::send_msg("access granted");
            close(sock_client);
        }
        else
        {
            // Регистрация

            // Проверяем, не занят ли такой логин уже
            if (data.exists(login))
            {
                SERVER::send_msg("this login already taken");
                close(sock_client);
                continue;
            }
            SERVER::send_msg("getting your open key...");
            long int v = 0;
            SERVER::recv_msg(v);
            data.register_new_user(login, v);
            SERVER::send_msg("you're successfully registered!");
            close(sock_client);
        }
        close(sock_client);
    }

    return 0;
}