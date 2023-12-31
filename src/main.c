#include "sim.h"


#define GEN_IDX(__idx) ((__idx) / 32)
#define GEN_SIZE(__s) GEN_IDX((__s) + 31)


static inline uint32_t get_gen_value(const uint32_t * gen, int32_t x, int32_t y) {
    x = (x % SIM_X_SIZE + SIM_X_SIZE) % SIM_X_SIZE;
    y = (y % SIM_Y_SIZE + SIM_Y_SIZE) % SIM_Y_SIZE;

    const uint32_t idx = y * SIM_X_SIZE + x;
    return (gen[GEN_IDX(idx)] >> (idx % 32)) & 1;
}

static inline void set_gen_value(uint32_t * gen, int32_t x, int32_t y, uint32_t v) {
    x = (x % SIM_X_SIZE + SIM_X_SIZE) % SIM_X_SIZE;
    y = (y % SIM_Y_SIZE + SIM_Y_SIZE) % SIM_Y_SIZE;

    const uint32_t idx = y * SIM_X_SIZE + x;

    if (v) {
        gen[GEN_IDX(idx)] |= 1 << (idx % 32);
    } else {
        gen[GEN_IDX(idx)] &= ~(1 << (idx % 32));
    }
}

static void draw_gen(const uint32_t * gen) {
    for (uint32_t y = 0; y < SIM_Y_SIZE; y++) {
        for (uint32_t x = 0; x < SIM_X_SIZE; x++) {
            sim_set_pixel(x, y, 0xFF000000 + 0xFF00 * get_gen_value(gen, x, y));
        }
    }

    sim_flush();
}

static uint32_t calc_gen(uint32_t * next_gen, const uint32_t * prev_gen) {
    uint32_t has_changes = 0;
    uint32_t has_living = 0;

	for (int32_t y = 0; y < SIM_Y_SIZE; ++y) {
	    for (int32_t x = 0; x < SIM_X_SIZE; ++x) {
            uint32_t neighbours = 0;

            for (int32_t dy = -1; dy <= 1; ++dy) {
                for (int32_t dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }

                    neighbours += get_gen_value(prev_gen, x + dx, y + dy);
                }
            }

            const uint32_t old_value = get_gen_value(prev_gen, x, y);
            const uint32_t next_value = old_value
                ? neighbours == 2 || neighbours == 3
                : neighbours == 3;

            if (old_value != next_value) {
                has_changes = 1;
            }

            if (next_value) {
                has_living = 1;
            }

            set_gen_value(next_gen, x, y, next_value);
        }
    }

    return has_changes || has_living;
}

int main(void) {
    uint32_t gen1[GEN_SIZE(SIM_Y_SIZE * SIM_X_SIZE)];
    uint32_t gen2[GEN_SIZE(SIM_Y_SIZE * SIM_X_SIZE)];
    uint32_t * next_gen = gen1;
    uint32_t * prev_gen = gen2;

    for (uint32_t y = 0; y < SIM_Y_SIZE; y++) {
        for (uint32_t x = 0; x < SIM_X_SIZE; x++) {
            set_gen_value(prev_gen, x, y, sim_rand() % 2);
        }
    }

    draw_gen(prev_gen);

    while (calc_gen(next_gen, prev_gen)) {
        draw_gen(next_gen);

        uint32_t * const tmp = prev_gen;
        prev_gen = next_gen;
        next_gen = tmp;
    }

    return 0;
}
