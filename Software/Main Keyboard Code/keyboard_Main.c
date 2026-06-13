/*
 * Copyright (C) 2026  Charles-Antoine Mailhot
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
 * Split Keyboard – PRIMARY (left) half
 * =====================================
 * • Scans its own 6×7 key matrix  (GP10–GP15 rows, GP0–GP6 cols)
 * • Receives right half key states from the secondary over UART0
 *     RX = GP17  (secondary TX = GP16, cross-wired)
 * • Sends USB HID keyboard reports via TinyUSB
 */

#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "bsp/board.h"
#include "tusb.h"

// ─── Pin / peripheral definitions ────────────────────────────────────────────

#define NUM_ROWS  6
#define NUM_COLS  7

static const uint ROW_PINS[NUM_ROWS] = {10, 11, 12, 13, 14, 15};
static const uint COL_PINS[NUM_COLS] = {0,  1,  2,  3,  4,  5,  6};

#define UART_INST    uart0
#define UART_RX_PIN  17
#define UART_BAUD    1000000

// ─── Packet protocol (secondary → primary) ───────────────────────────────────
// [0xAA][row0..row5][XOR_checksum][0x55]  = 9 bytes total
#define START_BYTE   0xAAu
#define END_BYTE     0x55u
#define PACKET_SIZE  (NUM_ROWS + 3)   // 9

// ─── HID Usage IDs ───────────────────────────────────────────────────────────
// Letters A–Z
#define HID_A  0x04u
#define HID_B  0x05u
#define HID_C  0x06u
#define HID_D  0x07u
#define HID_E  0x08u
#define HID_F  0x09u
#define HID_G  0x0Au
#define HID_H  0x0Bu
#define HID_I  0x0Cu
#define HID_J  0x0Du
#define HID_K  0x0Eu
#define HID_L  0x0Fu
#define HID_M  0x10u
#define HID_N  0x11u
#define HID_O  0x12u
#define HID_P  0x13u
#define HID_Q  0x14u
#define HID_R  0x15u
#define HID_S  0x16u
#define HID_T  0x17u
#define HID_U  0x18u
#define HID_V  0x19u
#define HID_W  0x1Au
#define HID_X  0x1Bu
#define HID_Y  0x1Cu
#define HID_Z  0x1Du
// Digits
#define HID_1  0x1Eu
#define HID_2  0x1Fu
#define HID_3  0x20u
#define HID_4  0x21u
#define HID_5  0x22u
#define HID_6  0x23u
#define HID_7  0x24u
#define HID_8  0x25u
#define HID_9  0x26u
#define HID_0  0x27u
// Control
#define HID_ENTER   0x28u
#define HID_ESC     0x29u
#define HID_BKSP    0x2Au
#define HID_TAB     0x2Bu
#define HID_SPACE   0x2Cu
// Punctuation
#define HID_MINUS   0x2Du
#define HID_EQUAL   0x2Eu
#define HID_LBRACKET 0x2Fu
#define HID_RBRACKET 0x30u
#define HID_BSLASH  0x31u
#define HID_SEMI    0x33u
#define HID_QUOTE   0x34u
#define HID_GRAVE   0x35u
#define HID_COMMA   0x36u
#define HID_DOT     0x37u
#define HID_SLASH   0x38u
#define HID_CAPS    0x39u
// Function keys
#define HID_F1  0x3Au
#define HID_F2  0x3Bu
#define HID_F3  0x3Cu
#define HID_F4  0x3Du
#define HID_F5  0x3Eu
#define HID_F6  0x3Fu
#define HID_F7  0x40u
#define HID_F8  0x41u
#define HID_F9  0x42u
#define HID_F10 0x43u
#define HID_F11 0x44u
#define HID_F12 0x45u
// Navigation
#define HID_DEL   0x4Cu
#define HID_RIGHT 0x4Fu
#define HID_LEFT  0x50u
#define HID_DOWN  0x51u
#define HID_UP    0x52u
// Modifiers (0xE0–0xE7) – handled separately in HID report modifier byte
#define HID_LCTRL  0xE0u
#define HID_LSHIFT 0xE1u
#define HID_LALT   0xE2u
#define HID_LGUI   0xE3u
#define HID_RCTRL  0xE4u
#define HID_RSHIFT 0xE5u
#define HID_RALT   0xE6u
// Sentinel – no key
#define HID_NONE   0x00u
// Media / Consumer Control (HID Usage Page 0x0C)
#define CONSUMER_PLAY_PAUSE   0x00CDu
#define CONSUMER_STOP         0x00B7u
#define CONSUMER_NEXT_TRACK   0x00B5u
#define CONSUMER_PREV_TRACK   0x00B6u
#define CONSUMER_FAST_FORWARD 0x00B3u
#define CONSUMER_REWIND       0x00B4u
#define CONSUMER_MUTE         0x00E2u
#define CONSUMER_VOL_UP       0x00E9u
#define CONSUMER_VOL_DOWN     0x00EAu

// ─── Keymaps ─────────────────────────────────────────────────────────────────
// Indexed [row][col].  Modifier keycodes (0xE0+) are extracted to the
// modifier byte; all others fill the 6-key slots.

static const uint8_t LEFT_KEYMAP[NUM_ROWS][NUM_COLS] = {
    // col: 0          1         2         3         4         5         6
    { HID_NONE,  HID_F1,   HID_F2,   HID_F3,   HID_F4,   HID_F5,        HID_F6   }, // row 0
    { HID_ESC,   HID_1,    HID_2,    HID_3,    HID_4,    HID_5,         HID_6    }, // row 1
    { HID_TAB,   HID_Q,    HID_W,    HID_E,    HID_R,    HID_T,         HID_GRAVE}, // row 2
    { HID_LSHIFT,HID_A,    HID_S,    HID_D,    HID_F,    HID_G,         HID_CAPS }, // row 3
    { HID_LCTRL, HID_Z,    HID_X,    HID_C,    HID_V,    HID_B,         HID_LALT }, // row 4
    { HID_NONE,  HID_LEFT, HID_SPACE,HID_DOWN, HID_UP,   HID_RIGHT,     HID_LGUI }, // row 5
};

static const uint8_t RIGHT_KEYMAP[NUM_ROWS][NUM_COLS] = {
    // col: 0          1         2         3         4         5         6
    { HID_NONE,  HID_F12,  HID_F11,  HID_F10,  HID_F9,   HID_F8,   HID_F7   }, // row 0
    { HID_BKSP,  HID_EQUAL,HID_MINUS,HID_0,    HID_9,    HID_8,    HID_7    }, // row 1
    { HID_ENTER, HID_P,    HID_O,    HID_I,    HID_U,    HID_Y,    HID_QUOTE}, // row 2
    { HID_RSHIFT,HID_BSLASH,HID_L,   HID_K,    HID_J,    HID_H,    HID_SEMI }, // row 3
    { HID_RCTRL, HID_DOT,  HID_COMMA,HID_M,    HID_N,    HID_B,    HID_SLASH}, // row 4
    { HID_NONE,  HID_RALT, HID_SPACE, HID_RBRACKET,HID_LBRACKET,HID_DEL, CONSUMER_PLAY_PAUSE}, // row 5
};

// ─── Debounce state ───────────────────────────────────────────────────────────
#define DEBOUNCE_CYCLES 20   // 20 × 500 µs = 10 ms – sufficient for mechanical switches
#define STARTUP_IGNORE_SCANS 200

static int8_t  raw_count[NUM_ROWS][NUM_COLS];
static bool    debounced[NUM_ROWS][NUM_COLS];
static bool    startup_reports_enabled = false;
static uint16_t startup_ignore_scans = 0;

// ─── Right-half state (from UART) ────────────────────────────────────────────
static bool right_state[NUM_ROWS][NUM_COLS];

// ─── UART RX ring buffer ──────────────────────────────────────────────────────
#define RX_BUF_SIZE 128

static uint8_t rx_buf[RX_BUF_SIZE];
static uint    rx_head = 0;   // write index
static uint    rx_tail = 0;   // read  index

static inline uint rx_len(void)  { return (rx_head - rx_tail) & (RX_BUF_SIZE - 1); }
static inline bool rx_empty(void){ return rx_head == rx_tail; }

static void uart_drain(void) {
    while (uart_is_readable(UART_INST) && rx_len() < RX_BUF_SIZE - 1) {
        rx_buf[rx_head & (RX_BUF_SIZE - 1)] = (uint8_t)uart_getc(UART_INST);
        rx_head++;
    }
}

static uint8_t rx_peek(uint offset) {
    return rx_buf[(rx_tail + offset) & (RX_BUF_SIZE - 1)];
}

static void rx_consume(uint n) {
    rx_tail += n;
}

// ─── Hardware initialisation ──────────────────────────────────────────────────

static void gpio_init_matrix(void) {
    // Columns: outputs, idle-high.
    // Set the output register HIGH *before* switching to output mode so
    // the pin never briefly glitches LOW and falsely asserts all rows.
    for (uint c = 0; c < NUM_COLS; c++) {
        gpio_init(COL_PINS[c]);
        gpio_put(COL_PINS[c], 1);
        gpio_set_dir(COL_PINS[c], GPIO_OUT);
    }
    // Rows: inputs. External 2 kΩ pull-ups hold the line HIGH; a keypress
    // shorts the row to the driven-LOW column.
    // Internal pull-ups (~50 kΩ) are also enabled so that a missing or
    // broken external pull-up cannot leave a row pin floating LOW and
    // cause phantom keypresses for every key in that row.
    for (uint r = 0; r < NUM_ROWS; r++) {
        gpio_init(ROW_PINS[r]);
        gpio_set_dir(ROW_PINS[r], GPIO_IN);
        gpio_pull_up(ROW_PINS[r]);
    }
}

static void uart_init_secondary(void) {
    uart_init(UART_INST, UART_BAUD);
    // Pull RX high before muxing to UART so the line idles correctly
    // even when the secondary half is not yet connected.
    gpio_pull_up(UART_RX_PIN);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_INST, false, false);
    uart_set_format(UART_INST, 8, 1, UART_PARITY_NONE);
}

// ─── Matrix scan & debounce ───────────────────────────────────────────────────

static void scan_and_debounce(void) {
    for (uint c = 0; c < NUM_COLS; c++) {
        gpio_put(COL_PINS[c], 0);
        sleep_us(5);
        for (uint r = 0; r < NUM_ROWS; r++) {
            bool pressed = !gpio_get(ROW_PINS[r]);
            if (pressed) {
                if (raw_count[r][c] < DEBOUNCE_CYCLES)
                    raw_count[r][c]++;
                if (raw_count[r][c] == DEBOUNCE_CYCLES)
                    debounced[r][c] = true;
            } else {
                if (raw_count[r][c] > 0)
                    raw_count[r][c]--;
                if (raw_count[r][c] == 0)
                    debounced[r][c] = false;
            }
        }
        gpio_put(COL_PINS[c], 1);
    }
}

// ─── UART packet parser ───────────────────────────────────────────────────────

static void parse_uart(void) {
    uart_drain();

    while (rx_len() >= PACKET_SIZE) {
        // Find START_BYTE
        if (rx_peek(0) != START_BYTE) {
            rx_consume(1);
            continue;
        }
        // Check END_BYTE
        if (rx_peek(PACKET_SIZE - 1) != END_BYTE) {
            rx_consume(1);   // bad frame; skip
            continue;
        }
        // Verify XOR checksum over the NUM_ROWS row bytes (offsets 1..6)
        uint8_t chk = 0;
        for (uint r = 0; r < NUM_ROWS; r++)
            chk ^= rx_peek(1 + r);
        if (chk == rx_peek(1 + NUM_ROWS)) {
            for (uint r = 0; r < NUM_ROWS; r++) {
                uint8_t row_byte = rx_peek(1 + r);
                for (uint c = 0; c < NUM_COLS; c++)
                    right_state[r][c] = (row_byte >> c) & 1u;
            }
        }
        rx_consume(PACKET_SIZE);
    }
}

// ─── HID report builder ──────────────────────────────────────────────────────

// Returns the modifier-byte bit for a modifier keycode, or 0 if not a modifier.
static inline uint8_t modifier_bit(uint8_t hid) {
    if (hid >= 0xE0u && hid <= 0xE7u)
        return (uint8_t)(1u << (hid - 0xE0u));
    return 0;
}

static void build_keyboard_report(uint8_t *modifier, uint8_t keycodes[6]) {
    *modifier = 0;
    memset(keycodes, 0, 6);

    uint key_count = 0;

    for (uint r = 0; r < NUM_ROWS; r++) {
        for (uint c = 0; c < NUM_COLS; c++) {
            uint8_t hid = HID_NONE;

            if (debounced[r][c])
                hid = LEFT_KEYMAP[r][c];
            else if (right_state[r][c])
                hid = RIGHT_KEYMAP[r][c];

            if (hid == HID_NONE) continue;

            uint8_t mod = modifier_bit(hid);
            if (mod) {
                *modifier |= mod;
            } else if (key_count < 6) {
                keycodes[key_count++] = hid;
            }
        }
    }
}

static void send_hid_report(void) {
    if (!tud_hid_ready()) return;

    uint8_t modifier;
    uint8_t keycodes[6];

    build_keyboard_report(&modifier, keycodes);
    tud_hid_keyboard_report(0, modifier, keycodes);
}

// ─── TinyUSB callbacks ────────────────────────────────────────────────────────

// Invoked when USB is mounted (host has enumerated the device).
// Reset all debounce state so any pre-enumeration transients are discarded
// and the first report sent to the host reflects only live key state.
void tud_mount_cb(void) {
    memset(raw_count,   0, sizeof(raw_count));
    memset(debounced,   0, sizeof(debounced));
    memset(right_state, 0, sizeof(right_state));
    startup_reports_enabled = false;
    startup_ignore_scans = 0;
}

// Invoked when GET_REPORT control request is received.
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                                hid_report_type_t report_type,
                                uint8_t *buffer, uint16_t reqlen) {
    (void)instance; (void)report_id;

    if (report_type != HID_REPORT_TYPE_INPUT || reqlen == 0)
        return 0;

    uint8_t modifier;
    uint8_t keycodes[6];
    uint8_t report[8] = {0};
    uint16_t report_len = reqlen < sizeof(report) ? reqlen : sizeof(report);

    if (startup_reports_enabled) {
        build_keyboard_report(&modifier, keycodes);
        report[0] = modifier;
        memcpy(&report[2], keycodes, sizeof(keycodes));
    }

    memcpy(buffer, report, report_len);
    return report_len;
}

// Invoked when SET_REPORT or data is received on OUT endpoint.
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                            hid_report_type_t report_type,
                            const uint8_t *buffer, uint16_t bufsize) {
    (void)instance; (void)report_id; (void)report_type;
    (void)buffer; (void)bufsize;
}

// ─── Main ─────────────────────────────────────────────────────────────────────

int main(void) {
    board_init();
    tusb_init();

    gpio_init_matrix();
    uart_init_secondary();

    memset(raw_count,   0, sizeof(raw_count));
    memset(debounced,   0, sizeof(debounced));
    memset(right_state, 0, sizeof(right_state));
    startup_reports_enabled = false;
    startup_ignore_scans = 0;

    // Previous HID state – used to detect changes
    uint8_t prev_modifier    = 0;
    uint8_t prev_keycodes[6] = {0};

    while (true) {
        tud_task();          // TinyUSB device task

        scan_and_debounce();
        parse_uart();

        // Build new report
        uint8_t modifier;
        uint8_t keycodes[6];

        build_keyboard_report(&modifier, keycodes);

        if (!startup_reports_enabled) {
            if (startup_ignore_scans < STARTUP_IGNORE_SCANS) {
                startup_ignore_scans++;
            } else {
                memset(raw_count,   0, sizeof(raw_count));
                memset(debounced,   0, sizeof(debounced));
                memset(right_state, 0, sizeof(right_state));
                startup_reports_enabled = true;
                prev_modifier = 0;
                memset(prev_keycodes, 0, sizeof(prev_keycodes));
                continue;
            }
        }

        // Send report only when state changes
        if (startup_reports_enabled && tud_hid_ready()) {
            if (modifier != prev_modifier ||
                memcmp(keycodes, prev_keycodes, 6) != 0)
            {
                tud_hid_keyboard_report(0, modifier, keycodes);
                prev_modifier = modifier;
                memcpy(prev_keycodes, keycodes, 6);
            }
        }

        sleep_us(500);   // ~500 µs scan interval
    }
}
