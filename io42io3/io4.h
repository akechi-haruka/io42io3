#pragma once

#define IO4_ADC_COUNT 8
#define IO4_SPINNER_COUNT 4
#define IO4_CHUTE_COUNT 2
#define IO4_BUTTON_COUNT 2
#define IO4_BIT_COUNT 16

struct JVSUSBReportIn {
    /// <summary>
    /// The current values of the board's analog devices.
    /// </summary>
    unsigned short adcs[IO4_ADC_COUNT];
    /// <summary>
    /// The current values of the board's spinners.
    /// </summary>
    unsigned short spinners[IO4_SPINNER_COUNT];
    /// <summary>
    /// The current values of the board's chutes.
    /// </summary>
    unsigned short chutes[IO4_CHUTE_COUNT];
    /// <summary>
    /// The current values of the board's buttons. Individual buttons are encoded as bits, so ex. the 4th button of player 2 would be tested with (buttons[1] >> 3 &amp; 1) != 0
    /// </summary>
    unsigned short buttons[IO4_BUTTON_COUNT];
    /// <summary>
    /// Unknown.
    /// </summary>
    char system_status;
    /// <summary>
    /// Unknown.
    /// </summary>
    char usb_status;
    /// <summary>
    /// Unknown.
    /// </summary>
    char unknown[29];
};

struct JVSUSBReportGPIOOut {
    unsigned char led[4];
    char unknown[58];
};

HRESULT io4_hid_init();

HRESULT io4_hid_poll(struct JVSUSBReportIn* report);

HRESULT io4_clear_board_status();

HRESULT io4_set_gpio(struct JVSUSBReportGPIOOut report);