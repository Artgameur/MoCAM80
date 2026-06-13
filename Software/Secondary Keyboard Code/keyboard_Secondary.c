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

#include <stdbool.h>
#include <stdint.h>

#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

#define NUM_ROWS 6
#define NUM_COLS 7
#define UART_ID uart0
#define UART_TX_PIN 16
#define UART_RX_PIN 17
#define UART_BAUD_RATE 1000000
#define START_BYTE 0xAA
#define END_BYTE 0x55
#define DEBOUNCE_CYCLES 5
#define PACKET_SIZE (NUM_ROWS + 3)

static const uint8_t row_pins[NUM_ROWS] = {10, 11, 12, 13, 14, 15};
static const uint8_t col_pins[NUM_COLS] = {0, 1, 2, 3, 4, 5, 6};

static uint8_t raw_count[NUM_ROWS][NUM_COLS];
static bool debounced[NUM_ROWS][NUM_COLS];

static void matrix_init(void) {
    for (uint8_t col = 0; col < NUM_COLS; ++col) {
        gpio_init(col_pins[col]);
        gpio_put(col_pins[col], true);
        gpio_set_dir(col_pins[col], GPIO_OUT);
    }

    for (uint8_t row = 0; row < NUM_ROWS; ++row) {
        gpio_init(row_pins[row]);
        gpio_set_dir(row_pins[row], GPIO_IN);
        gpio_disable_pulls(row_pins[row]);
    }
}

static void uart_setup(void) {
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_fifo_enabled(UART_ID, true);
}

static void scan_raw(bool state[NUM_ROWS][NUM_COLS]) {
    for (uint8_t col = 0; col < NUM_COLS; ++col) {
        gpio_put(col_pins[col], false);
        sleep_us(5);

        for (uint8_t row = 0; row < NUM_ROWS; ++row) {
            state[row][col] = !gpio_get(row_pins[row]);
        }

        gpio_put(col_pins[col], true);
    }
}

static bool debounce_matrix(const bool raw[NUM_ROWS][NUM_COLS]) {
    bool changed = false;

    for (uint8_t row = 0; row < NUM_ROWS; ++row) {
        for (uint8_t col = 0; col < NUM_COLS; ++col) {
            if (raw[row][col]) {
                if (raw_count[row][col] < DEBOUNCE_CYCLES) {
                    ++raw_count[row][col];
                }
                if (raw_count[row][col] == DEBOUNCE_CYCLES && !debounced[row][col]) {
                    debounced[row][col] = true;
                    changed = true;
                }
            } else {
                if (raw_count[row][col] > 0) {
                    --raw_count[row][col];
                }
                if (raw_count[row][col] == 0 && debounced[row][col]) {
                    debounced[row][col] = false;
                    changed = true;
                }
            }
        }
    }

    return changed;
}

static void build_packet(uint8_t packet[PACKET_SIZE]) {
    uint8_t checksum = 0;

    packet[0] = START_BYTE;
    for (uint8_t row = 0; row < NUM_ROWS; ++row) {
        uint8_t row_bits = 0;
        for (uint8_t col = 0; col < NUM_COLS; ++col) {
            if (debounced[row][col]) {
                row_bits |= (uint8_t)(1u << col);
            }
        }
        packet[row + 1] = row_bits;
        checksum ^= row_bits;
    }
    packet[NUM_ROWS + 1] = checksum;
    packet[NUM_ROWS + 2] = END_BYTE;
}

int main(void) {
    bool last_packet_valid = false;
    uint8_t packet[PACKET_SIZE] = {0};
    bool raw_state[NUM_ROWS][NUM_COLS] = {{false}};

    matrix_init();
    uart_setup();

    while (true) {
        scan_raw(raw_state);

        if (debounce_matrix(raw_state) || !last_packet_valid) {
            build_packet(packet);
            uart_write_blocking(UART_ID, packet, PACKET_SIZE);
            last_packet_valid = true;
        }

        sleep_ms(1);
    }
}
