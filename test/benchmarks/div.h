
#include <cstdint>
#include <tuple>

inline int clz(uint64_t x)
{
    return __builtin_clzl(x);
}

inline uint64_t umulh(uint64_t x, uint64_t y)
{
    unsigned __int128 p = static_cast<unsigned __int128>(x) * y;
    return static_cast<uint64_t>(p >> 64);
}

uint64_t soft_div_unr_unrolled(uint64_t x, uint64_t y)
{
    // decent start
    uint64_t z = uint64_t(1) << clz(y);

    // z recurrence, 6 iterations (TODO: make sure 6 is enough)
    uint64_t my = 0 - y;
    for (int i = 0; i < 6; ++i)
        z += umulh(z, my * z);

    // q estimate
    uint64_t q = umulh(x, z);
    uint64_t r = x - (y * q);

    // q refinement
    if (r >= y)
    {
        r -= y;
        q += 1;

        if (r >= y)
        {
            r -= y;
            q += 1;
        }
    }
    return q;
}

uint64_t soft_div_unr(uint64_t x, uint64_t y)
{
    // decent start
    uint64_t z = uint64_t(1) << clz(y);

    // z recurrence
    uint64_t my = 0 - y;
    for (int i = 0; i < 6; ++i)
    {
        uint64_t zd = umulh(z, my * z);
        if (zd == 0)
            break;
        z = z + zd;
    }

    // q estimate
    uint64_t q = umulh(x, z);
    uint64_t r = x - (y * q);
    // q refinement
    if (r >= y)
    {
        r = r - y;
        q = q + 1;
        if (r >= y)
        {
            r = r - y;
            q = q + 1;
        }
    }
    return q;
}
