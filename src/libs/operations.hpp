#include <cmath>

#define RAND_LIMIT 10000
#define NUM_OF_CHECKS 20

bool is_prime(u_int64_t number);
u_int64_t generate_prime_number(u_int64_t limiter = 0);
u_int64_t fast_exp(u_int64_t base, u_int64_t exp, u_int64_t mod);