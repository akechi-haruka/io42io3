#pragma once

#include <windows.h>

#include <stdbool.h>
#include <stdint.h>

#include "io42io3/config.h"
#include "io42io3/io4.h"

int shared_get_io4_btn(int index);
void shared_poll(uint8_t *opbtn, uint8_t *gamebtn);
struct io42io3_config shared_get_config();
struct JVSUSBReportIn shared_get_report();
