#include "sim.h"


static void draw_gen(const uint8_t * gen) {
    for (uint32_t y = 0; y < SIM_Y_SIZE; y++) {
        for (uint32_t x = 0; x < SIM_X_SIZE; x++) {
            sim_set_pixel(x, y, 0xFF000000 + 0xFF00 * gen[y * SIM_X_SIZE + x]);
        }
    }

    sim_flush();
}

static inline uint8_t get_gen_value(const uint8_t * gen, int32_t x, int32_t y) {
    x = (x % SIM_X_SIZE + SIM_X_SIZE) % SIM_X_SIZE;
    y = (y % SIM_Y_SIZE + SIM_Y_SIZE) % SIM_Y_SIZE;

    return gen[y * SIM_X_SIZE + x];
}

static uint8_t calc_gen(uint8_t * next_gen, const uint8_t * prev_gen) {
    uint8_t has_changes = 0;
    uint8_t has_living = 0;

	for (int32_t y = 0; y < SIM_Y_SIZE; ++y) {
	    for (int32_t x = 0; x < SIM_X_SIZE; ++x) {
            uint32_t neighbours = 0;

            for (int8_t dy = -1; dy <= 1; ++dy) {
                for (int8_t dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }

                    neighbours += get_gen_value(prev_gen, x + dx, y + dy);
                }
            }

            const uint8_t old_value = prev_gen[y * SIM_X_SIZE + x];
            const uint8_t next_value = old_value
                ? neighbours == 2 || neighbours == 3
                : neighbours == 3;

            if (old_value != next_value) {
                has_changes = 1;
            }

            if (next_value) {
                has_living = 1;
            }

            next_gen[y * SIM_X_SIZE + x] = next_value;
        }
    }

    return has_changes || has_living;
}

int main() {
    uint8_t gen1[SIM_Y_SIZE * SIM_X_SIZE] = { 0 };
    uint8_t gen2[SIM_Y_SIZE * SIM_X_SIZE] = { 0 };
    uint8_t * next_gen = gen1;
    uint8_t * prev_gen = gen2;

    for (uint32_t y = 0; y < SIM_Y_SIZE; y++) {
        for (uint32_t x = 0; x < SIM_X_SIZE; x++) {
            prev_gen[y * SIM_X_SIZE + x] = sim_rand() % 2;
        }
    }

    draw_gen(prev_gen);

    while (calc_gen(next_gen, prev_gen)) {
        draw_gen(next_gen);

        uint8_t * const tmp = prev_gen;
        prev_gen = next_gen;
        next_gen = tmp;
    }

    return 0;
}
