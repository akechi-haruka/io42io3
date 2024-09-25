#include "io42io3/util/dprintf.h"
#include "io42io3/config.h"
#include "io42io3/io42io3.h"

#include "io42io3/games/kemono.h"

void game_kemono_config_load(
        struct kemono_config *cfg,
        const char *filename){
}

uint16_t kemono_io_get_api_version(void)
{
    dprintf("IO42IO3: KEMONO FRIENDS\n");
    return 0x0100;
}
