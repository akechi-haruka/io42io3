#pragma once

#include <stddef.h>
#include <stdint.h>

#include <stdbool.h>

#include "io42io3/games/kca.h"
#include "io42io3/games/kemono.h"

#define MAX_PLAYER_BUTTONS 16
#define MAX_OPERATOR_BUTTONS 8
#define MAX_GPIO 32
#define MAX_ADCS 8

struct io42io3_config {
    uint8_t inputs[MAX_PLAYER_BUTTONS];
    uint8_t opinputs[MAX_OPERATOR_BUTTONS];
    uint8_t gpio[MAX_GPIO];

    int8_t adc2btn[MAX_PLAYER_BUTTONS];
    uint16_t adcmin[MAX_PLAYER_BUTTONS];
    uint16_t adcmax[MAX_PLAYER_BUTTONS];

    int coin_keyboard_button;
    int coin_chute;

    bool sleep;

    struct kca_config kca;
    struct kemono_config kemono;
};

void io42io3_config_load(
        struct io42io3_config *cfg,
        const char *filename);
