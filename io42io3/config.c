#include <windows.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "io42io3/config.h"

void io42io3_config_load(
        struct io42io3_config *cfg,
        const char *filename)
{
    assert(cfg != NULL);
    assert(filename != NULL);

    for (int b = 0; b < MAX_PLAYER_BUTTONS; b++){
        char bstr[16];
        sprintf(bstr, "button%d", b);
        cfg->inputs[b] = GetPrivateProfileIntA("player", bstr, -1, filename);
    }

    for (int b = 0; b < MAX_OPERATOR_BUTTONS; b++){
        char bstr[16];
        sprintf(bstr, "button%d", b);
        cfg->opinputs[b] = GetPrivateProfileIntA("system", bstr, -1, filename);
    }

    for (int b = 0; b < MAX_GPIO; b++){
        char bstr[16];
        sprintf(bstr, "led%d", b);
        cfg->gpio[b] = GetPrivateProfileIntA("led", bstr, -1, filename);
    }

    cfg->coin_keyboard_button = GetPrivateProfileIntA("coin", "keyboard", '1', filename);
    cfg->coin_chute = GetPrivateProfileIntA("coin", "chute", -1, filename);
}
