#include QMK_KEYBOARD_H

extern rgblight_config_t rgblight_config;
bool VIM_d, VIM_y, VIM_lshift, VIM_rshift = false;
uint8_t RGB_enable;
uint8_t RGB_mode;
uint8_t RGB_hue;
uint8_t RGB_sat;
uint8_t RGB_val;
uint8_t RGB_val_vim = 150;

bool vim_shift(void) {
    return VIM_lshift || VIM_rshift;
}

void restore_original_rgb(void) {
    //load regular rgb settings
    rgblight_mode(RGB_mode);
    rgblight_sethsv(RGB_hue, RGB_sat, RGB_val);
    if (RGB_enable) {
       rgblight_enable(); 
    } else {
       rgblight_disable(); 
    }
}

enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    VDKTP_R, // virtual deskptop right
    VDKTP_L, // virtual desktop left
    TAB_R,   // move to the tab to the right (for chrome)
    TAB_L,   // move to the tab to the left (for chrome)

    VIM_MD,  // enter vim mode // layer 3
    REG_MD,  // enter insert mode (or just regular mode) // layer 1
    APPEND,  // append to insert mode (right arrow then regular mode)
    VIENTER, // vim enter (after pressing enter will go back to regular mode)

    M_UNDO,  // control + z
    NEXT_WD, // move to next word ('w' in vim)
    BACK_WD, // move back to the last word ('b' in vim)
    VI_RSHFT,// vim right shift
    VI_LSHFT,// vim left shift

    DY_N_WD, // delete/yank next word ('dw' in vim) 
    DY_B_WD, // delete/yank back (last) word ('db' in vim)
    DY_END,  // delete/yank until the end of line
    DY_HOME, // delete/yank until the beginning of line
    D_LINE,  // delete current line
    Y_LINE,  // yank current line

    NEW_LN,  // add a new line
    V_PASTE, // paste in vim 
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case QMKBEST:
        if (record->event.pressed) { // on press
            SEND_STRING("QMK is the best thing ever!");
        } else { // on release:
        }
        break;
    case VDKTP_R:
        if (record->event.pressed) { // on press
            SEND_STRING(SS_LCTL(SS_LGUI(SS_TAP(X_RGHT))));
        } else { // on release:
        }
        break;
    case VDKTP_L:
        if (record->event.pressed) { // on press
            SEND_STRING(SS_LCTL(SS_LGUI(SS_TAP(X_LEFT))));
        } else { // on release:
        }
        break;
    case TAB_R:
        if (record->event.pressed) { // on press
            SEND_STRING(SS_LCTL(SS_TAP(X_TAB)));
        } else { // on release:
        }
        break;
    case TAB_L:
        if (record->event.pressed) { // on press
            SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_TAB))));
        } else { // on release:
        }
        break;
    case M_UNDO:
        if (record->event.pressed) { // on press
            SEND_STRING(SS_LCTL("z"));
        } else { // on release:
        }
        break;
	// vim/regular modes ------------------------------------------------
    case VIM_MD:
        if (record->event.pressed) { // on press
            //save current rgb settings
			RGB_enable = rgblight_config.enable;
			RGB_mode = rgblight_config.mode;
			RGB_hue = rgblight_config.hue;
			RGB_sat = rgblight_config.sat;
			RGB_val = rgblight_config.val;

            //load vim rgb settings
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL + 5);
            rgblight_sethsv(RGB_hue, 255, 
                            RGB_val > RGB_val_vim ? RGB_val : RGB_val_vim);
            rgblight_enable();
        } else { // on release:
            //change layer
            layer_on(2);
        }
        break;
    case REG_MD:
        if (record->event.pressed) { // on press
            restore_original_rgb();
        } else { // on release:
            //change layer
            layer_off(2);
        }
        break;
    case APPEND:
        if (record->event.pressed) { // on press
            restore_original_rgb();
            if (vim_shift()) {
                SEND_STRING(SS_TAP(X_END));
            } else {
                SEND_STRING(SS_TAP(X_RGHT));
            }
        } else { // on release:
            //change layer
            layer_off(2);
        }
        break;
    case VIENTER:
        if (record->event.pressed) { // on press
            restore_original_rgb();
            SEND_STRING(SS_TAP(X_ENTER));
        } else { // on release:
            //change layer
            layer_off(2);
        }
        break;
    case VI_RSHFT:
        if (record->event.pressed) { // on press
            VIM_rshift = true;
        } else { // on release:
            VIM_rshift = false;
        }
        break;
    case VI_LSHFT:
        if (record->event.pressed) { // on press
            VIM_lshift = true;
        } else { // on release:
            VIM_lshift = false;
        }
        break;
    // vim commands ------------------------------------------------
    case NEXT_WD:
        if (record->event.pressed) { // on press
            SEND_STRING(SS_LCTL(SS_TAP(X_RGHT)));
        } else { // on release:
        }
        break;
    case BACK_WD:
        if (record->event.pressed) { // on press
            SEND_STRING(SS_LCTL(SS_TAP(X_LEFT)));
        } else { // on release:
        }
        break;

    case NEW_LN:
        if (record->event.pressed) { // on press
            if (vim_shift()) {
                SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_ENT)) SS_TAP(X_UP));
            } else {
                SEND_STRING(SS_TAP(X_END) SS_LSFT(SS_TAP(X_ENT)));
            }
        } else { // on release:
        }
        break;
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [0] = LAYOUT(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_END,   KC_INS,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_BSPC,  KC_DEL,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_VOLU,
    KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,                      KC_ENT,   KC_VOLD,
    KC_LSFT,  KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,            KC_UP,    KC_MPLY,
    KC_LCTL,  KC_LGUI,  KC_LALT,                      KC_SPC,   KC_SPC,   KC_SPC,                       KC_RALT,  MO(1),    MO(1),    KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [1] = LAYOUT(
    RESET,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RGB_HUD,  RGB_HUI,  RGB_RMOD, RGB_MOD,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_HOME,  RGB_VAD,  RGB_VAI,  RGB_TOG,  RGB_TOG,  KC_SLEP,
    _______,  _______,  NEXT_WD,  _______,  _______,  _______,  _______,  M_UNDO,   _______,  NEW_LN,   _______,  RGB_SAD,  RGB_SAI,  _______,            KC_MNXT,
    VIM_MD,   KC_END,   _______,  _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,                      _______,  KC_MPRV,
    _______,  _______,  _______,  KC_DEL,   _______,  _______,  BACK_WD,  _______,  _______,  TAB_L,    TAB_R,    _______,  _______,            KC_PGDN,  _______,
    _______,  _______,  _______,                      _______,  _______,  _______,                      _______,  _______,  _______,  VDKTP_L,  KC_PGUP,  VDKTP_R
  ),

  [2] = LAYOUT( // vim layer
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_HOME,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  NEXT_WD,  _______,  _______,  _______,  _______,  M_UNDO,   REG_MD,   NEW_LN,   _______,  _______,  _______,  _______,            _______,
    REG_MD,   APPEND,   _______,  _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,                      VIENTER,  _______,
    VI_LSHFT, _______,  _______,  KC_DEL,   _______,  _______,  BACK_WD,  _______,  _______,  _______,  _______,  _______,  VI_RSHFT,           _______,  _______,
    _______,  _______,  _______,                      _______,  _______,  _______,                      _______,  _______,  _______,  _______,  _______,  _______
  ),

  [3] = LAYOUT(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                      _______,  _______,  _______,                      _______,  _______,  _______,  _______,  _______,  _______
  ),

};
