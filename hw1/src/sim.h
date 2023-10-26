#pragma once

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif


#define SIM_X_SIZE 256
#define SIM_Y_SIZE 128

#ifdef __cplusplus
#include "sim.hpp"
#endif


#ifdef __cplusplus
extern "C" {
#endif

extern void sim_set_pixel(uint32_t x, uint32_t y, uint32_t argb);
extern void sim_flush(void);
extern uint32_t sim_rand(void);

#ifdef __cplusplus
}
#endif

