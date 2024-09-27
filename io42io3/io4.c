#include "io4.h"
#include "hidapi.h"
#include "io42io3/util/dprintf.h"

#include <windows.h>

#define MAX_STR 255
#define IO4_REPORT_LEN 63
#define IO4_IN_REPORT_ID 0x01

static hid_device *handle;

HRESULT io4_hid_init(){
    int res;
	wchar_t wstr[MAX_STR];

    dprintf("IO4: Opening device\n");

    res = hid_init();
    if (res){
        dprintf("IO4: ERROR: failed to initialize hidapi\n");
        return E_FAIL;
    }

	handle = hid_open(0x0ca3, 0x0021, NULL);
	if (handle == NULL){
        dprintf("IO4: ERROR: failed to open IO4 (is it connected?)\n");
        return E_FAIL;
	}

    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res){
        dprintf("IO4: ERROR: failed to retrieve board data (1)\n");
        return E_FAIL;
	}
	dwprintf(L"IO4: Manufacturer String: %s\n", wstr);

	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res){
        dprintf("IO4: ERROR: failed to retrieve board data (2)\n");
        return E_FAIL;
    }
	dwprintf(L"IO4: Product String: %s\n", wstr);

	dprintf("IO4: SUCCESS\n");
	return S_OK;
}

HRESULT write_hid_report(unsigned char cmd, void* data, int len){

    if (handle == NULL){
        return E_HANDLE;
    }

	unsigned char buf[len + 2];
	memset(buf, 0, len + 2);
    buf[0] = 0x10;
    buf[1] = cmd;
    memcpy(buf + 2, data, len);
    if (hid_write(handle, buf, len + 2) < 0){
        dprintf("IO4: write cmd %d failed\n", cmd);
        return E_FAIL;
    }
    return S_OK;
}

HRESULT io4_hid_poll(struct JVSUSBReportIn* report){

    if (handle == NULL){
        return E_HANDLE;
    }

    DWORD start = GetTickCount();

    unsigned char buf[IO4_REPORT_LEN + 1];
    int res = hid_read(handle, buf, IO4_REPORT_LEN + 1);
    if (res < 0){
        dprintf("IO4: poll failed\n");
        return E_FAIL;
    }
    if (res != IO4_REPORT_LEN + 1){
        dprintf("IO4: only read %d/%d bytes\n", res, IO4_REPORT_LEN + 1);
        return E_FAIL;
    }
    if (buf[0] != IO4_IN_REPORT_ID){
        dprintf("IO4: WARN: invalid report %d\n", buf[0]);
        return S_FALSE;
    }
    DWORD end = GetTickCount();
    if (end - start >= 20){
        dprintf("IO4: WARN: slow poll: %ld\n", end - start);
    }
    memcpy(report, buf + 1, IO4_REPORT_LEN);
    return S_OK;
}

HRESULT io4_clear_board_status(){
    char* buf[0x40] = {0};
    return write_hid_report(0x03, (void*)buf, 0x40);
}

HRESULT io4_set_gpio(struct JVSUSBReportGPIOOut report){
    return write_hid_report(0x04, (void*)&report, sizeof(report));
}