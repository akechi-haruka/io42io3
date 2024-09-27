#include <windows.h>

#include <stdbool.h>
#include <stdint.h>
#include <winuser.h>

#include "io42io3/io42io3.h"
#include "io42io3/config.h"
#include "io42io3/io4.h"
#include "io42io3/util/dprintf.h"

#include <io42io3/games/kca.h>
#include <io42io3/games/kemono.h>

#include "subprojects/segapi/api/api.h"

#define MIN_API_VER 0x010101

struct io42io3_config cfg;
struct JVSUSBReportIn report;

static bool inited = false;
static int coin_counter = 0;
static bool coin_counter_pressed = false;
static uint32_t last_gpio = 0;

BOOL __attribute__((unused)) WINAPI
DllMain(__attribute__((unused)) HMODULE mod, DWORD cause, __attribute__((unused)) void *ctx) {

    if (cause != DLL_PROCESS_ATTACH) {
        return TRUE;
    }

    dprintf("IO42IO3: Initializing\n");

    if (api_get_version() < MIN_API_VER) {
        dprintf("IO42IO3: API dll is outdated! At least v.%x is required, DLL is v.%x", MIN_API_VER, api_get_version());
        return FALSE;
    }

    io42io3_config_load(&cfg, ".\\io42io3.ini");
    game_kca_config_load(&cfg.kca, ".\\io42io3.ini");
    game_kemono_config_load(&cfg.kemono, ".\\io42io3.ini");

    api_init(".\\io42io3.ini");

    dprintf("IO42IO3: Loaded\n");

    return TRUE;
}

struct io42io3_config shared_get_config() {
    return cfg;
}

struct JVSUSBReportIn shared_get_report() {
    return report;
}

DWORD WINAPI polling_thread(__attribute__((unused)) void *data) {
    dprintf("IO42IO3: I/O Polling Thread started\n");
    while (true) {

        if (FAILED(io4_hid_poll(&report))) {
            dprintf("IO42IO3: ERROR: I/O polling failed!\n");
            return 1;
        }

        if (cfg.sleep) {
            Sleep(1);
        }
    }

    dprintf("IO42IO3: I/O Polling Thread stopped\n");
    return 0;
}

HRESULT shared_init(void) {

    if (inited){
        dprintf("IO42IO3: already initialized!\n");
        return S_FALSE;
    }
    dprintf("IO42IO3: JVS Init called\n");

    HRESULT hr = io4_hid_init();

    if (FAILED(hr)) {
        return hr;
    }

    CreateThread(NULL, 0, polling_thread, NULL, 0, NULL);

    hr = io4_clear_board_status();
    if (FAILED(hr)) {
        return hr;
    }

    inited = true;

    struct JVSUSBReportGPIOOut clear_leds = {};
    return io4_set_gpio(clear_leds);
}

void shared_jvs_io3_read_coin_counter(uint16_t *out) {
    if (out == NULL) {
        return;
    }

    if (cfg.coin_chute > -1) {

        *out = report.chutes[cfg.coin_chute];

    } else {

        int api_credits = api_get_and_clear_credits();
        if (api_credits > 0) {
            coin_counter += api_credits;
        }

        if (GetAsyncKeyState(cfg.coin_keyboard_button) & 0x8000) {
            if (!coin_counter_pressed) {
                coin_counter_pressed = true;
                coin_counter++;
            }
        } else {
            coin_counter_pressed = false;
        }

        *out = coin_counter;

    }
}

inline int shared_get_io4_btn(int btn) {
    int p = btn / 16;
    int b = btn % 16;
    int v = ((report.buttons[p] >> b) & 1);
    return v != 0;
}

inline uint16_t shared_get_io4_adc(int adc) {
    return report.adcs[adc];
}

static int8_t button_test = -1;

void shared_poll(uint8_t *opbtn_out, uint8_t *gamebtn_out) {
    uint8_t opbtn;
    uint8_t gamebtn;

    opbtn = 0;
    gamebtn = 0;

    for (int i = 0; i < MAX_OPERATOR_BUTTONS; i++) {
        int btn = cfg.opinputs[i];
        if (btn > -1) {
            opbtn |= (shared_get_io4_btn(btn) << i);
        }
    }

    if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) {
        button_test++;
        dprintf("NOW TESTING IO3 OUTPUT %d\n", button_test);
        Sleep(3000);
    }
    if (button_test > -1) {
        gamebtn |= (1 << button_test);
    }

    for (int i = 0; i < MAX_PLAYER_BUTTONS; i++) {
        int8_t adc = cfg.adc2btn[i];
        if (adc > -1) {
            uint16_t val = shared_get_io4_adc(adc);
            if (val < cfg.adcmin[i] || val > cfg.adcmax[i]) {
                gamebtn |= (1 << i);
            }
        } else {
            int btn = cfg.inputs[i];
            if (btn > -1) {
                gamebtn |= (shared_get_io4_btn(btn) << i);
            }
        }
    }

    *opbtn_out = opbtn;
    *gamebtn_out = gamebtn;
}

void shared_write_gpio(uint32_t bytes) {
    //dprintf("IO42IO3: GPIO: %d\n", bytes);

    if (bytes == last_gpio) {
        return;
    }

    struct JVSUSBReportGPIOOut ledreport = {};
    memset(&ledreport, 0, sizeof(ledreport));

    for (int i = 0; i < MAX_GPIO; i++) {
        int gpio = cfg.gpio[i];
        if (gpio > -1) {
            int offset = gpio / 8;
            int index = gpio % 8;
            ledreport.led[offset] |= ((((bytes >> i) & 1) != 0) << index);
        }
    }

    last_gpio = bytes;
    io4_set_gpio(ledreport);
}

HRESULT shared_led_init(void) {
    return S_OK;
}

void shared_led_set_colors(uint8_t board, uint8_t *rgb) {

}