#pragma once

#include <ac_int.h>
#include "ArchitectureParams.h"

#pragma hls_design ccore
template <typename T>
T poly_approx(
    T x,
    const T maxes[NUM_MAXES],
    const T ranges[NUM_RANGES][NUM_COEFFS],
    const ac_int<1, false> clamp_min,
    const ac_int<1, false> clamp_max
) {
    const T min_bound = maxes[0];
    const T max_bound = maxes[NUM_MAXES - 1];
    if (clamp_min && x < min_bound)        x = min_bound;
    else if (clamp_max && max_bound < x)   x = max_bound;

    T c0, c1, c2, c3;
    if      (x <= maxes[0]) { c0=ranges[0][0]; c1=ranges[0][1]; c2=ranges[0][2]; c3=ranges[0][3]; }
    else if (x <= maxes[1]) { c0=ranges[1][0]; c1=ranges[1][1]; c2=ranges[1][2]; c3=ranges[1][3]; }
    else if (x <= maxes[2]) { c0=ranges[2][0]; c1=ranges[2][1]; c2=ranges[2][2]; c3=ranges[2][3]; }
    else if (x <= maxes[3]) { c0=ranges[3][0]; c1=ranges[3][1]; c2=ranges[3][2]; c3=ranges[3][3]; }
    else if (x <= maxes[4]) { c0=ranges[4][0]; c1=ranges[4][1]; c2=ranges[4][2]; c3=ranges[4][3]; }
    else if (x <= maxes[5]) { c0=ranges[5][0]; c1=ranges[5][1]; c2=ranges[5][2]; c3=ranges[5][3]; }
    else                    { c0=ranges[6][0]; c1=ranges[6][1]; c2=ranges[6][2]; c3=ranges[6][3]; }

    return c0 + x * (c1 + x * (c2 + x * c3));
}
