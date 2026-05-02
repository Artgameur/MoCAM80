// mcuconf.h — RP2350 peripheral configuration for MoCAM80
// Activates UART0, which is mapped to GP16 (TX) and GP17 (RX).
#pragma once

#include_next <mcuconf.h>

// Enable UART0 — required for the full-duplex USART split transport.
#undef  RP_SERIAL_USE_UART0
#define RP_SERIAL_USE_UART0 TRUE
