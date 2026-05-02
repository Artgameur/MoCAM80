// halconf.h — ChibiOS HAL configuration for MoCAM80
// Enables the hardware UART driver needed for split serial transport.
#pragma once

#include_next <halconf.h>

// Enable the Serial (UART) driver used by the USART split transport.
#undef  HAL_USE_SERIAL
#define HAL_USE_SERIAL TRUE
