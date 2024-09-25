#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "io42io3/config.h"

struct kemono_config {
};

void game_kemono_config_load(
        struct kemono_config *cfg,
        const char *filename);
