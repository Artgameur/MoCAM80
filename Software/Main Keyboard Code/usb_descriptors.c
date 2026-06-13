/*
 * Copyright (C) 2026  Artgameur
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * USB descriptor tables for the split keyboard primary (left) half.
 * Provides a single USB HID keyboard interface using TinyUSB.
 */

#include "tusb.h"

// ─── HID Report Descriptor ────────────────────────────────────────────────────
// Standard boot-compatible keyboard with one report ID.

static const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD()
};

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    (void)instance;
    return hid_report_descriptor;
}

// ─── String descriptors ───────────────────────────────────────────────────────

enum {
    STRID_LANGID = 0,
    STRID_MANUFACTURER,
    STRID_PRODUCT,
    STRID_SERIAL,
};

static const char *string_desc_arr[] = {
    (const char[]){0x09, 0x04},  // [0] Language: English (0x0409)
    "DIY",                       // [1] Manufacturer
    "Split Keyboard",            // [2] Product
    "000001",                    // [3] Serial number
};

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;

    static uint16_t desc_str[32];
    uint8_t len;

    if (index == STRID_LANGID) {
        // Special case: language ID is returned as-is (2 bytes)
        memcpy(desc_str, string_desc_arr[0], 4);
        len = 1;
    } else {
        if (index >= (sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
            return NULL;
        const char *str = string_desc_arr[index];
        len = (uint8_t)strlen(str);
        if (len > 31) len = 31;
        for (uint8_t i = 0; i < len; i++)
            desc_str[1 + i] = str[i];
    }

    // First word: length (including header) + descriptor type (STRING = 0x03)
    desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * len + 2));
    return desc_str;
}

// ─── Device Descriptor ───────────────────────────────────────────────────────

static const tusb_desc_device_t device_descriptor = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = 0x1234,   // Change to your registered VID if needed
    .idProduct          = 0x5678,   // Change to your PID if needed
    .bcdDevice          = 0x0100,
    .iManufacturer      = STRID_MANUFACTURER,
    .iProduct           = STRID_PRODUCT,
    .iSerialNumber      = STRID_SERIAL,
    .bNumConfigurations = 0x01,
};

uint8_t const *tud_descriptor_device_cb(void) {
    return (uint8_t const *)&device_descriptor;
}

// ─── Configuration Descriptor ─────────────────────────────────────────────────

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define EPNUM_HID  0x81   // EP 1 IN

static const uint8_t config_descriptor[] = {
    // Configuration
    TUD_CONFIG_DESCRIPTOR(
        1,                  // config number
        1,                  // interface count
        0,                  // string index
        CONFIG_TOTAL_LEN,
        TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP,
        100                 // mA
    ),

    // HID Interface
    TUD_HID_DESCRIPTOR(
        0,                          // interface number
        0,                          // string index
        HID_ITF_PROTOCOL_KEYBOARD,  // boot protocol
        sizeof(hid_report_descriptor),
        EPNUM_HID,
        CFG_TUD_HID_EP_BUFSIZE,
        10                          // polling interval (ms)
    ),
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void)index;
    return config_descriptor;
}
