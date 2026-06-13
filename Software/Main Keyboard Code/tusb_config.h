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

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// CFG_TUSB_MCU is supplied by the Pico SDK cmake target (tinyusb_device).
#ifndef CFG_TUSB_MCU
  #error CFG_TUSB_MCU must be defined
#endif

// Use USB as device
#define CFG_TUSB_RHPORT0_MODE   OPT_MODE_DEVICE

// Enable one HID interface
#define CFG_TUD_HID             1
#define CFG_TUD_HID_EP_BUFSIZE  16

#endif // _TUSB_CONFIG_H_
