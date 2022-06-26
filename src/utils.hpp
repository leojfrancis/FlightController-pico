#ifndef _UTILS_H
#define _UTILS_H

#ifdef __cplusplus
extern "C"
{
#endif

    long map(long x, long in_min, long in_max, long out_min, long out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    long maximum(long x, long prev = 0)
    {
        if (x > prev)
            return x;
        return prev;
    }

    long minimum(long x, long prev = 0)
    {
        if (x < prev)
            return x;
        return prev;
    }

#ifdef __cplusplus
}
#endif
#endif