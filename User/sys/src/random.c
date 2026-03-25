#include "random.h"
#include "sys.h"

static uint32_t _xorshift32_s = 1;
static uint32_t xorshift32()
{
    uint32_t x = _xorshift32_s;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    _xorshift32_s = x;
    return x;
}

int randint(int a, int b)
{
    return a + xorshift32() % (b - a + 1);
}
