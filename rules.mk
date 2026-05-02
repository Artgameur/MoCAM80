# ─── MCU ──────────────────────────────────────────────────────────────────────
MCU = RP2350B
BOOTLOADER = rp2040

# ─── Split keyboard ───────────────────────────────────────────────────────────
SPLIT_KEYBOARD = yes

# ─── Serial transport (UART, full-duplex, pins GP16/GP17) ─────────────────────
SERIAL_DRIVER = usart

# ─── Optional features ────────────────────────────────────────────────────────
BOOTMAGIC_ENABLE = yes   # Hold top-left key on plug-in to enter bootloader
EXTRAKEY_ENABLE = yes    # Media & system keys
NKRO_ENABLE = yes        # Full N-key rollover
MOUSEKEY_ENABLE = no
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
