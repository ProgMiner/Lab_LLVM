#include "sim.h"

#include <random>


static SimContext ctx;

void sim_set_pixel(uint32_t x, uint32_t y, uint32_t argb) {
    ctx.set_pixel(x, y, argb);
}

void sim_flush() {
    ctx.flush();
}

uint32_t sim_rand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint32_t> distrib(0, 6);

    return distrib(gen);
}
