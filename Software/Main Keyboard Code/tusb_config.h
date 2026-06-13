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
