#include QMK_KEYBOARD_H

// ─── Layer indices ────────────────────────────────────────────────────────────
enum layers {
    _BASE = 0,
    _FN,
};

// ─── Custom keycodes ──────────────────────────────────────────────────────────
enum custom_keycodes {
    FN = SAFE_RANGE,
};

// ─── Keymap ───────────────────────────────────────────────────────────────────
// Matrix layout: 5 rows × 8 cols per half, halves side-by-side.
// Adjust keycodes to match your physical key placement.
//
// Left half              │ Right half
// Row 0:  Esc F1..F7     │ F8..F12 Ins Del Bksp
// Row 1:  ` 1..7         │ 8..0 - = \ Home PgUp
// Row 2:  Tab Q..T [ ]   │ Y..P Up End PgDn
// Row 3:  Caps A..G ' Ent│ H..L ; Left Down Right
// Row 4:  LSft Z..B LC LA│ N M , . / RSft RAlt RCtl
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT(
        // ── Left ──────────────────────────────────────────  ── Right ──────────────────────────────────────────
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,     KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_DEL,  KC_BSPC,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,      KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_HOME, KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_LBRC, KC_RBRC,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_UP,   KC_END,  KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_QUOT, KC_ENT,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_LEFT, KC_DOWN, KC_RGHT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LCTL, KC_LALT,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_RALT, KC_RCTL
    ),

    [_FN] = LAYOUT(
        // ── Left ──────────────────────────────────────────  ── Right ──────────────────────────────────────────
        QK_BOOT, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, KC_VOLU, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, KC_MPRV, KC_VOLD, KC_MNXT,
        _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, KC_MUTE, KC_MPLY
    ),
};
