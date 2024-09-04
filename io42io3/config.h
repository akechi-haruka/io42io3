#pragma once

#include <stddef.h>
#include <stdint.h>

#include <stdbool.h>

#include "io42io3/games/kca.h"

#define MAX_PLAYER_BUTTONS 8
#define MAX_OPERATOR_BUTTONS 8
#define MAX_GPIO 32

struct io42io3_config {
    uint8_t inputs[MAX_PLAYER_BUTTONS];
    uint8_t opinputs[MAX_OPERATOR_BUTTONS];
    uint8_t gpio[MAX_GPIO];

    int coin_keyboard_button;
    int coin_chute;

    struct kca_config kca;
};

void io42io3_config_load(
        struct io42io3_config *cfg,
        const char *filename);
