#include <cmath>

#define RAND_LIMIT 10000
#define NUM_OF_CHECKS 20

bool is_prime(long int number);
long int generate_prime_number(long int limiter = -1);
long int fast_exp(long int base, long int exp, long int mod);