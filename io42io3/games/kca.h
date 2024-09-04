#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "io42io3/config.h"

struct kca_config {
    uint32_t steering_speed;
    uint8_t steering_adc;
    uint8_t steering_invert;
    uint32_t steering_deadzone;
    uint8_t button_telegraph_up;
    uint8_t button_telegraph_down;
};

void game_kca_config_load(
        struct kca_config *cfg,
        const char *filename);
