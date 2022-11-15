#include "operations.hpp"

bool is_prime(long int number) {
    if (number <= 1)
        return false;

    for (int i = 2; i <= sqrt(number); i++)
        if (number % i == 0)
            return false;

    return true;
}

long int generate_prime_number(long int limiter) {
    long int number = 0;
    if (limiter == -1)
        limiter = RAND_LIMIT;
    do {
        number = rand() % limiter;
    } while (!is_prime(number));
    return number;
}

long int fast_exp(long int base, long int exp, long int mod) {
    long int log = floor(log2(exp));
    long int row;
    long int prod = 1;
    for (int i = 0; i <= log; i++) {
        if (i == 0) {
            row = base % mod;
        } else {
            row = (row * row) % mod;
        }
        if ((exp >> i) & 1) {
            prod = (prod * row) % mod;
        }
    }
    return prod;
}