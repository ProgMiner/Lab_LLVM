#include "sim.h"

#include <stdbool.h>
#include <unistd.h>


// ms
#define STEP_DELAY 100


static void draw_gen(const bool * gen) {
    uint32_t x = 0;
    uint32_t y = 0;

    for (y = 0; y < SIM_Y_SIZE; y++) {
        for (x = 0; x < SIM_X_SIZE; x++) {
            sim_set_pixel(x, y, 0xFF000000 + 0xFF00 * gen[y * SIM_X_SIZE + x]);
        }
    }

    sim_flush();
}

static inline bool get_gen_value(const bool * gen, int32_t x, int32_t y) {
    x = (x % SIM_X_SIZE + SIM_X_SIZE) % SIM_X_SIZE;
    y = (y % SIM_Y_SIZE + SIM_Y_SIZE) % SIM_Y_SIZE;

    return gen[y * SIM_X_SIZE + x];
}

static bool calc_gen(bool * next_gen, const bool * prev_gen) {
    bool has_changes = false;
    bool has_living = false;

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

            const bool old_value = prev_gen[y * SIM_X_SIZE + x];
            const bool next_value = old_value
                ? neighbours == 2 || neighbours == 3
                : neighbours == 3;

            if (old_value != next_value) {
                has_changes = true;
            }

            if (next_value) {
                has_living = true;
            }

            next_gen[y * SIM_X_SIZE + x] = next_value;
        }
    }

    return has_changes || has_living;
}

int main() {
    bool gen1[SIM_Y_SIZE * SIM_X_SIZE] = { 0 };
    bool gen2[SIM_Y_SIZE * SIM_X_SIZE] = { 0 };
    bool * next_gen = gen1;
    bool * prev_gen = gen2;

    for (uint32_t y = 0; y < SIM_Y_SIZE; y++) {
        for (uint32_t x = 0; x < SIM_X_SIZE; x++) {
            prev_gen[y * SIM_X_SIZE + x] = sim_rand() % 2;
        }
    }

    draw_gen(prev_gen);

    while (calc_gen(next_gen, prev_gen)) {
        usleep(STEP_DELAY * 1000);

        draw_gen(next_gen);

        bool * const tmp = prev_gen;
        prev_gen = next_gen;
        next_gen = tmp;
    }

    return 0;
}
