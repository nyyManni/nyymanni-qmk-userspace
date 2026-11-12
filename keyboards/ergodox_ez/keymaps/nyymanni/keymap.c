#include QMK_KEYBOARD_H
#include "version.h"

enum layers {
    BASE,     // default layer
    SYMB,     // symbols
    MOVEMENT, // arrow keys
    GAMING,   // left-hand gaming overrides
};

enum custom_keycodes {
    VRSN = SAFE_RANGE,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// Base typing layer. Symbols assume a US host layout.
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_GRV,         KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   TG(GAMING),
        KC_DEL,         KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   KC_HASH,
        LALT_T(KC_ESC), KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        KC_LGUI,        LSFT_T(KC_Z), KC_X,   KC_C,   KC_V,   KC_B,   KC_CIRC,
        MO(MOVEMENT),   MO(SYMB),     KC_LPRN,KC_RPRN,KC_LCTL,

                                              KC_VOLD,  KC_VOLU,
                                                              KC_HOME,
                                               KC_BSPC,KC_TAB,KC_END,
        // right hand
             TG(GAMING), KC_6,   KC_7,    KC_8,   KC_9,   KC_0,           KC_EQL,
             KC_ASTR,     KC_Y,   KC_U,    KC_I,   KC_O,   KC_P,           KC_BSLS,
                          KC_H,   KC_J,    KC_K,   KC_L,   KC_SCLN,        LALT_T(KC_QUOT),
             KC_DLR,      KC_N,   KC_M,    KC_COMM,KC_DOT, LSFT_T(KC_SLSH), LGUI_T(KC_MINS),
                        KC_RCTL,KC_LBRC,KC_RBRC,MO(SYMB),       MO(MOVEMENT),

             KC_MSTP,        KC_MPLY,
             KC_PGUP,
             KC_PGDN,KC_ENT, KC_SPC
    ),
// Symbols, function keys, keypad, and media track controls.
[SYMB] = LAYOUT_ergodox(
       // left hand
       VRSN,   KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  _______,
       _______,KC_EXLM,KC_AT,  KC_LCBR,KC_RCBR,KC_PIPE,_______,
       _______,KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
       _______,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_TILD,_______,
       _______,_______,_______,_______,_______,
                                       _______,_______,
                                               _______,
                               _______,_______,_______,
       // right hand
       _______, KC_F6,   KC_F7,   KC_F8,    KC_F9,    KC_F10,  KC_F11,
       _______, KC_UP,   KC_P7,   KC_P8,    KC_P9,    KC_PAST, KC_F12,
                KC_DOWN, KC_P4,   KC_P5,    KC_P6,    KC_PPLS, _______,
       _______, KC_AMPR, KC_P1,   KC_P2,    KC_P3,    KC_PSLS, _______,
                         _______, KC_PDOT,  KC_P0,    _______, _______,
       KC_MPRV, KC_MNXT,
       _______,
       _______, _______, _______
),
// Arrows and host-configured GUI+F10 / GUI+F15 shortcuts.
[MOVEMENT] = LAYOUT_ergodox(
       _______, _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______,
                                           _______, _______,
                                                    _______,
                                  _______, _______, _______,
    // right hand
       _______,  _______, _______, _______, _______, LGUI(KC_F10), LGUI(KC_F15), // Start screen saver
       _______,  _______, _______, _______, _______, _______, _______,
                 KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
       _______,  _______, _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______,
       _______, _______,
       _______,
       _______, _______, _______
),

// The left gaming toggle becomes 6; the transparent right toggle exits.
[GAMING] = LAYOUT_ergodox(
       _______, _______, _______, _______, _______, _______, KC_6,
       KC_LALT, _______, _______, _______, _______, _______, _______,
        KC_ESC, _______, _______, _______, _______, _______,
       KC_LSFT, KC_Z, _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______,
                                           _______, _______,
                                                    _______,
                          _______, _______, KC_ENT,
    // right hand
       _______,  _______, _______, _______, _______, _______, _______,
       _______,  _______, _______, _______, _______, _______, _______,
                 _______, _______, _______, _______, _______, _______,
       _______,  _______, _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______,
       _______, _______,
       _______,
       _______, _______, _______
                           )
};
// clang-format on

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_Z):
        case LSFT_T(KC_SLSH):
            return 200;
        default:
            return TAPPING_TERM;
    }
}

// A recent Z/slash tap must not turn the next Shift hold into auto-repeat.
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_Z):
        case LSFT_T(KC_SLSH):
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}

// Settle opposite-hand Shift chords on keydown, regardless of release order.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_Z):
        case LSFT_T(KC_SLSH):
            return true;
        default:
            return false;
    }
}

// ErgoDox EZ stores the left half in rows 0-6 and the right in rows 7-13.
char chordal_hold_handedness(keypos_t key) {
    return key.row < MATRIX_ROWS / 2 ? 'L' : 'R';
}

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    switch (tap_hold_keycode) {
        case LSFT_T(KC_Z):
        case LSFT_T(KC_SLSH):
            // Ctrl/Alt/GUI/Shift must not turn a pending Shift into Z or slash.
            if (IS_MODIFIER_KEYCODE(other_keycode)) {
                return true;
            }
            return get_chordal_hold_default(tap_hold_record, other_record);
        default:
            return true;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case VRSN:
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
                return false;
        }
    }
    return true;
}

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
    rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case 0:
#ifdef RGBLIGHT_COLOR_LAYER_0
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
            break;
        case 1:
            ergodox_right_led_1_on();
#ifdef RGBLIGHT_COLOR_LAYER_1
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
#endif
            break;
        case 2:
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_2
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
#endif
            break;
        case 3:
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_3
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
#endif
            break;
        case 4:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_4
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
#endif
            break;
        case 5:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_5
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
#endif
            break;
        case 6:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_6
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
#endif
            break;
        case 7:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_7
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
#endif
            break;
        default:
            break;
    }

    return state;
};
