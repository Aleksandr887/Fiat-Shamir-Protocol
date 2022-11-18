#include "operations.hpp"

bool is_prime(u_int64_t number)
{
    if (number <= 1) {
        return false;
    }

    for (u_int32_t i = 2; i <= sqrt(number); i++) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

u_int64_t generate_prime_number(u_int64_t limiter)
{
    u_int64_t number = 0;
    if (limiter == 0) {
        limiter = RAND_LIMIT;
    }
    do {
        number = rand() % limiter;
    } while (!is_prime(number));

    return number;
}

u_int64_t fast_exp(u_int64_t base, u_int64_t exp, u_int64_t mod)
{
    u_int64_t log = floor(log2(exp));
    u_int64_t row;
    u_int64_t prod = 1;
    for (u_int32_t i = 0; i <= log; i++) {
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