#pragma once

// ─── Matrix dimensions ───────────────────────────────────────────────────────
// 5 rows per half × 2 halves = 10 total rows; 8 cols per half.
#define MATRIX_ROWS 10
#define MATRIX_COLS 8

// ─── Split keyboard ───────────────────────────────────────────────────────────
// GP15 is used as the left/right hand side selector pin.
// Connect GP15 to GND on the left half; leave floating (or pull-high) on right.
#define SPLIT_HAND_PIN GP15

// ─── Serial / UART transport ─────────────────────────────────────────────────
// Full-duplex UART over GP16 (TX) and GP17 (RX).
// GP16 → UART0 TX   |   GP17 → UART0 RX
// Wire: left GP16 ──► right GP17
//       left GP17 ◄── right GP16
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP16
#define SERIAL_USART_RX_PIN GP17
#define SERIAL_USART_SPEED 921600  // High baud rate for low-latency split comms

// ─── USB connection ──────────────────────────────────────────────────────────
// Uncomment one of the options below if you cannot use SPLIT_HAND_PIN:
//   #define MASTER_LEFT   // left half is always the USB master
//   #define MASTER_RIGHT  // right half is always the USB master
