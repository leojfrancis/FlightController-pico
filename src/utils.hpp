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

    long clamp(long val, long min, long max)
    {
        if (val < min)
            return min;
        else if (val > max)
            return max;
        return val;
    }

    long clamp_map(long x, long in_min, long in_max, long out_min, long out_max, long comp_min, long comp_max)
    {
        return clamp((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, comp_min, comp_max);
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