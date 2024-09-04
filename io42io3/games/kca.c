#include "io42io3/util/dprintf.h"
#include "io42io3/config.h"
#include "io42io3/io42io3.h"

const int SPEED_STEPS = 6;
static uint16_t telegraph_speed = 0;
static uint8_t steering_value = 0;
static bool last_up_state = false;
static bool last_down_state = false;

void game_kca_config_load(
        struct kca_config *cfg,
        const char *filename){
    cfg->steering_speed = GetPrivateProfileIntA("kca", "steering_speed", 1, filename);
    cfg->steering_adc = GetPrivateProfileIntA("kca", "steering_adc", 0, filename);
    cfg->steering_invert = GetPrivateProfileIntA("kca", "steering_invert", 0, filename);
    cfg->steering_deadzone = GetPrivateProfileIntA("kca", "steering_deadzone", 20, filename);
    cfg->button_telegraph_up = GetPrivateProfileIntA("kca", "button_telegraph_up", 10, filename);
    cfg->button_telegraph_down = GetPrivateProfileIntA("kca", "button_telegraph_down", 11, filename);
}

uint16_t kantai_io_get_api_version(void)
{
    dprintf("IO42IO3: KANTAI COLLECTION ARCADE\n");
    return 0x0102;
}

void kantai_io_jvs_poll(uint8_t *opbtn_out, uint8_t *gamebtn_out)
{
    uint8_t opbtn = 0;
    uint8_t gamebtn = 0;

    shared_poll(&opbtn, &gamebtn);

    struct io42io3_config cfg = shared_get_config();
    struct JVSUSBReportIn report = shared_get_report();

    if (shared_get_io4_btn(cfg.kca.button_telegraph_up)) {
        if (!last_up_state && telegraph_speed < SPEED_STEPS){
            telegraph_speed++;
        }
        last_up_state = true;
    } else {
        last_up_state = false;
    }

    if (shared_get_io4_btn(cfg.kca.button_telegraph_down)) {
        if (!last_down_state && telegraph_speed > 0){
            telegraph_speed--;
        }
        last_down_state = true;
    } else {
        last_down_state = false;
    }

    int adc = report.adcs[cfg.kca.steering_adc];

    uint32_t adc_direction = 0;
    if (adc > USHRT_MAX / 2 + cfg.kca.steering_deadzone) {
        adc_direction = cfg.kca.steering_speed;
    } else if (adc < USHRT_MAX / 2 - cfg.kca.steering_deadzone) {
        adc_direction = -cfg.kca.steering_speed;
    }

    if (cfg.kca.steering_invert){
        adc_direction *= -1;
    }
    steering_value += adc_direction;

    *opbtn_out = opbtn;
    *gamebtn_out = gamebtn;

}

void kantai_io_jvs_read_steering(uint8_t *steering){
    if (steering == NULL) {
        return;
    }

    *steering = steering_value;
}

void kantai_io_jvs_read_telegraph(uint16_t *telegraph){
    if (telegraph == NULL) {
        return;
    }

    *telegraph = telegraph_speed * USHRT_MAX / SPEED_STEPS;
}
