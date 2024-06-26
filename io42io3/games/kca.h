#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "io42io3/config.h"

struct kca_config {
    int steering_speed;
    int steering_adc;
    int steering_invert;
    int steering_deadzone;
    int button_telegraph_up;
    int button_telegraph_down;
};

void game_kca_config_load(
        struct kca_config *cfg,
        const char *filename);
