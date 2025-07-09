#pragma once

#include <ac_int.h>
#include "ArchitectureParams.h"

const VECTOR_DATATYPE EXP_MAXES[NUM_MAXES] = {-4.0, -2.0, -0.5, 0.0, 0.5, 2.0};
const VECTOR_DATATYPE EXP_RANGES[NUM_RANGES][NUM_COEFFS] = {
    {0.0, 0.0, 0.0, 0.0},
    {0.67650, 0.44727, 0.10577, 0.00878},
    {0.93237, 0.96342, 0.66150, 0.20769},
    {0.93237, 0.96342, 0.66150, 0.20769},
    {0.93237, 0.96342, 0.66150, 0.20769},
    {0.93237, 0.96342, 0.66150, 0.20769},
    {0.93237, 0.96342, 0.66150, 0.20769}
};
const ac_int<1, false> EXP_CLAMP_MIN = 1;
const ac_int<1, false> EXP_CLAMP_MAX = 1;


const VECTOR_DATATYPE GELU_MAXES[NUM_MAXES] = {-4.0, -2.0, -0.5, 0.0, 0.5, 2.0};
const VECTOR_DATATYPE GELU_RANGES[NUM_RANGES][NUM_COEFFS] = {
    {0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0},
    {0.0, 0.5, 0.4429289, 0.10261996},
    {0.0, 0.5, 0.40725988, 0.04765296},
    {0.0, 0.5, 0.19947114, 0.03324519},
    {0.0, 0.5, 0.19947114, 0.03324519},
    {0.0, 1.0, 0.0, 0.0}
};
const ac_int<1, false> GELU_CLAMP_MIN = 1;
const ac_int<1, false> GELU_CLAMP_MAX = 0;


const VECTOR_DATATYPE SILU_MAXES[NUM_MAXES] = {-4.0, -2.0, -0.5, 0.0, 2.0, 4.0};
const VECTOR_DATATYPE SILU_RANGES[NUM_RANGES][NUM_COEFFS] = {
    {0.0, 0.0, 0.0, 0.0},
    {0.0, 0.37413, 0.16931, 0.02016},
    {0.0, 0.51100, 0.28849, 0.04632},
    {0.0, 0.50044, 0.25535, 0.01894},
    {0.0, 0.49105, 0.28555, -0.04532},
    {0.0, 0.62587, 0.16931, -0.02016},
    {0.0, 1.0, 0.0, 0.0}
};
const ac_int<1, false> SILU_CLAMP_MIN = 1;
const ac_int<1, false> SILU_CLAMP_MAX = 0;


const VECTOR_DATATYPE HARDTANH_MAXES[NUM_MAXES] = {0.0, 6.0, 6.0, 6.0, 6.0, 6.0};
const VECTOR_DATATYPE HARDTANH_RANGES[NUM_RANGES][NUM_COEFFS] = {
    {0.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {6.0, 0.0, 0.0, 0.0}
};
const ac_int<1, false> HARDTANH_CLAMP_MIN = 1;
const ac_int<1, false> HARDTANH_CLAMP_MAX = 1;