#include QMK_KEYBOARD_H

#define _NUM_LAYER 1
#define _FL 2 // function layer, shortened for the keymaps section
#define _VIM_LAYER 3
#define _DY_LAYER 4

// Define Types
enum Keyboard_Mode {Regular_Mode, Num_Mode, Vim_Mode, Y_Mode, D_Mode} KB_mode = Regular_Mode;
typedef enum Keyboard_Mode Keyboard_Mode;

// Define Emoji Map
enum unicode_names {
    D_FC,
    PLEAD,
    IRONY,
    SNEK
};

const uint32_t PROGMEM unicode_map[] = {
    [D_FC]  = 0x1F978,  // Disguised Face Emoji
    [PLEAD]  = 0x1F97A,  // 🥺
    [IRONY] = 0x2E2E,  // ⸮
    [SNEK]  = 0x1F40D, // 🐍
};

// Global Vars
Keyboard_Mode dy_mode_prev_mode = Regular_Mode;

extern rgblight_config_t rgblight_config;
rgblight_config_t vim_mode_prev_rgb;
rgblight_config_t dy_mode_prev_rgb;
rgblight_config_t num_mode_prev_rgb;
rgblight_config_t jptr_mode_prev_rgb;

bool JPTR_Mode_On = false;

bool VIM_lshift = false;
bool VIM_rshift = false;
bool clipboard_holds_line = false;
uint8_t RGB_val_vim = 150;

bool vim_shift(void) {
    return VIM_lshift || VIM_rshift;
}

void reset_vim_vars(void) {
    VIM_lshift = false;
    VIM_rshift = false;
}

void regular_mode_on(void) {
    KB_mode = Regular_Mode;
    layer_off(_NUM_LAYER);
    layer_off(_FL);
    layer_off(_VIM_LAYER);
    reset_vim_vars();
}

void num_mode_on(void) {
    KB_mode = Num_Mode;
    layer_on(_NUM_LAYER);
    reset_vim_vars();
}

void vim_mode_on(void) {
    KB_mode = Vim_Mode;
    layer_on(_VIM_LAYER);
    reset_vim_vars();
}

void dy_vim_mode_on(Keyboard_Mode mode) {
    dy_mode_prev_mode = KB_mode;
    KB_mode = mode;
    layer_on(_DY_LAYER);
}

void dy_vim_mode_off(void) {
    KB_mode = dy_mode_prev_mode;
    layer_off(_DY_LAYER);
    // reset_oneshot_layer();
}

// saving RGB settings  ----------------------

void save_rgb(rgblight_config_t* config) {
    //save current rgb settings
    config->enable = rgblight_config.enable;
    config->mode   = rgblight_config.mode;
    config->hue    = rgblight_config.hue;
    config->sat    = rgblight_config.sat;
    config->val    = rgblight_config.val;
} 

// modifying RGB settings  ----------------------

void load_rgb(rgblight_config_t* config) {
    rgblight_mode(config->mode);
    rgblight_sethsv(config->hue, config->sat, config->val);
    if (config->enable) {
       rgblight_enable(); 
    } else {
       rgblight_disable(); 
    }
}

enum rgb_preset {
    WHITE_RGB,
    RED_RGB,
    DRACULA_RGB,
    CYAN_RGB,
    GOLD_RGB,
    TWK_RGB,
    VIM_RGB,
    VIM_D_RGB,
    VIM_Y_RGB,
};

void set_rgb_preset(enum rgb_preset preset) {
    switch (preset) {
    case WHITE_RGB:
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv(HSV_WHITE);
        rgblight_enable();
        break;
    case RED_RGB:
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv(HSV_RED);
        rgblight_enable();
        break;
    case DRACULA_RGB:
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        //rgblight_setrgb(189, 147, 249);
        rgblight_sethsv(187, 105, 249);
        rgblight_enable();
        break;
    case CYAN_RGB:
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv(HSV_CYAN);
        rgblight_enable();
        break;
    case GOLD_RGB:
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv(HSV_GOLDENROD);
        rgblight_enable();
        break;
    case TWK_RGB:
        rgblight_mode(RGBLIGHT_MODE_TWINKLE + 4);
        rgblight_sethsv(HSV_RED);
        rgblight_enable();
        break;
    case VIM_RGB:
        //load vim rgb settings
        rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL + 5);
        rgblight_sethsv(rgblight_config.hue, 255,
                        rgblight_config.val > RGB_val_vim ? rgblight_config.val : RGB_val_vim);
        rgblight_enable();
        break;
    case VIM_D_RGB:
        //load vim d (delete) mode rgb settings
        rgblight_mode(RGBLIGHT_MODE_SNAKE + 2);
        rgblight_sethsv(HSV_RED);
        rgblight_enable();
        break;
    case VIM_Y_RGB:
        //load vim y (yank) mode rgb settings
        rgblight_mode(RGBLIGHT_MODE_SNAKE + 3);
        rgblight_sethsv(HSV_YELLOW);
        rgblight_enable();
        break;
    }
}


// keycodes ---------------------------------------------------------

enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    VDKTP_R, // virtual deskptop right
    VDKTP_L, // virtual desktop left
    TAB_R,   // move to the tab to the right (for chrome)
    TAB_L,   // move to the tab to the left (for chrome)

    VIM_MD,  // enter vim mode 
    VIM_RST, // reset vim settings (for use only in the vim layer)
    REG_MD,  // enter insert mode (or just regular mode)
    NUM_MD,  // enter number mode
    NUM_MDT, // number mode toggle
    JPTR_MDT,// jupyter mode toggle, makes vim stuffs use 'enter' instead of shift 'enter'
    APPEND,  // append to insert mode (right arrow then regular mode)
    VIENTER, // vim enter (after pressing enter will go back to regular mode)

    M_UNDO,  // control + z
    NEXT_WD, // move to next word ('w' in vim)
    BACK_WD, // move back to the last word ('b' in vim)
    VI_RSHFT,// vim right shift
    VI_LSHFT,// vim left shift

    VIM_D,   // vim d command (activates the delete commands)
    VIM_Y,   // vim y command (activates the yank commands)
    DY_N_WD, // delete/yank next word ('dw' in vim) 
    DY_B_WD, // delete/yank back (last) word ('db' in vim)
    DY_END,  // delete/yank until the end of line
    DY_HOME, // delete/yank until the beginning of line
    D_LINE,  // delete current line
    Y_LINE,  // yank current line
    DY_UNSET,// Unset the DY mode (restore old rgb and mode values & OSL)

    NEW_LN,  // add a new line
    V_PASTE, // paste in vim 

    PRE_WHI, // turn on rgb preset
    PRE_DRA, // turn on rgb preset
    PRE_RED, // turn on rgb preset
    PRE_TWK, // turn on rgb preset
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
            save_rgb(&vim_mode_prev_rgb); // backup the current rgb settings
            set_rgb_preset(VIM_RGB); // set the vim rgb
        } else { // on release:
            vim_mode_on();
        }
        break;
    case VIM_RST:
        if (record->event.pressed) { // on press
            set_rgb_preset(VIM_RGB); // set the vim rgb
        } else { // on release:
            vim_mode_on();
        }
        break;
    case REG_MD:
        if (record->event.pressed) { // on press
            load_rgb(&vim_mode_prev_rgb);
        } else { // on release:
            //change layer
            regular_mode_on();
        }
        break;
    case NUM_MD:
        if (record->event.pressed) { // on press
            save_rgb(&vim_mode_prev_rgb); // backup the current rgb settings
            set_rgb_preset(CYAN_RGB); // set the num rgb
        } else { // on release:
            num_mode_on();
        }
        break;
    case NUM_MDT:
        if (record->event.pressed) { // on press
            if (KB_mode == Num_Mode) {
                load_rgb(&num_mode_prev_rgb);
            } else if (KB_mode == Regular_Mode) {
                save_rgb(&num_mode_prev_rgb); // backup the current rgb settings
                set_rgb_preset(CYAN_RGB); // set the num rgb
            }
        } else { // on release:
            if (KB_mode == Num_Mode) {
                regular_mode_on();
            } else if (KB_mode == Regular_Mode) {
                num_mode_on();
            }
        }
        break;
    case JPTR_MDT:
        if (record->event.pressed) { // on press
            if (!JPTR_Mode_On) { // regular mode
                save_rgb(&jptr_mode_prev_rgb); // backup the current rgb settings
                set_rgb_preset(GOLD_RGB); // set the num rgb
            } else { // jptr mode
                load_rgb(&jptr_mode_prev_rgb); // backup the current rgb settings
            }
        } else { // on release:
            JPTR_Mode_On = !JPTR_Mode_On;
        }
        break;
    case APPEND:
        if (record->event.pressed) { // on press
            load_rgb(&vim_mode_prev_rgb);
            if (vim_shift()) {
                SEND_STRING(SS_TAP(X_END));
            } else {
                SEND_STRING(SS_TAP(X_RGHT));
            }
        } else { // on release:
            //change layer
            regular_mode_on();
        }
        break;
    case VIENTER:
        if (record->event.pressed) { // on press
            load_rgb(&vim_mode_prev_rgb);
            SEND_STRING(SS_TAP(X_ENTER));
        } else { // on release:
            //change layer
            regular_mode_on();
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
    // basic vim commands ------------------------------------------------
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
    // vim delete/yank commands: -----------------------------------------
    case VIM_D:
        if (record->event.pressed) { // on press
            // rgb settings
            save_rgb(&dy_mode_prev_rgb);
            set_rgb_preset(VIM_D_RGB);

            // turn mode on
            dy_vim_mode_on(D_Mode);
        } else { // on release:
        }
        break;
    case VIM_Y:
        if (record->event.pressed) { // on press
            // rgb settings
            save_rgb(&dy_mode_prev_rgb);
            set_rgb_preset(VIM_Y_RGB);

            // turn mode on
            dy_vim_mode_on(Y_Mode);
        } else { // on release:
        }
        break;
    // vim delete/yank commands: -----------------------------------------
    case DY_N_WD:
        if (record->event.pressed) { // on press
            if (KB_mode  == D_Mode) {
                SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_RIGHT))) SS_LCTL("x"));
            } else if (KB_mode == Y_Mode) {
                SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_RIGHT))) SS_LCTL("c") SS_TAP(X_LEFT));
            }
            clipboard_holds_line = false;
            load_rgb(&dy_mode_prev_rgb);
        } else { // on release:
            dy_vim_mode_off();
        }
        break;
    case DY_B_WD:
        if (record->event.pressed) { // on press
            if (KB_mode  == D_Mode) {
                SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_LEFT))) SS_LCTL("x"));
            } else if (KB_mode == Y_Mode) {
                SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_LEFT))) SS_LCTL("c") SS_TAP(X_RIGHT));
            }
            clipboard_holds_line = false;
            load_rgb(&dy_mode_prev_rgb);
        } else { // on release:
            dy_vim_mode_off();
        }
        break;
    case DY_END:
        if (record->event.pressed) { // on press
            if (KB_mode  == D_Mode) {
                SEND_STRING(SS_LSFT(SS_TAP(X_END)) SS_LCTL("x"));
            } else if (KB_mode == Y_Mode) {
                SEND_STRING(SS_LSFT(SS_TAP(X_END)) SS_LCTL("c") SS_TAP(X_LEFT));
            }
            clipboard_holds_line = false;
            load_rgb(&dy_mode_prev_rgb);
        } else { // on release:
            dy_vim_mode_off();
        }
        break;
    case DY_HOME:
        if (record->event.pressed) { // on press
            if (KB_mode  == D_Mode) {
                SEND_STRING(SS_LSFT(SS_TAP(X_HOME)) SS_LCTL("x"));
            } else if (KB_mode == Y_Mode) {
                SEND_STRING(SS_LSFT(SS_TAP(X_HOME)) SS_LCTL("c") SS_TAP(X_RIGHT));
            }
            clipboard_holds_line = false;
            load_rgb(&dy_mode_prev_rgb);
        } else { // on release:
            dy_vim_mode_off();
        }
        break;
    case D_LINE:
        if (record->event.pressed) { // on press
            if (KB_mode  == D_Mode) {
                SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_END)) SS_LCTL("x") SS_LSFT(SS_TAP(X_DOWN) SS_TAP(X_HOME)) SS_TAP(X_BSPC));
                clipboard_holds_line = true;
            }
            load_rgb(&dy_mode_prev_rgb);
            dy_vim_mode_off();
        } else { // on release:
        }
        break;
    case Y_LINE:
        if (record->event.pressed) { // on press
            if (KB_mode  == Y_Mode) {
                SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_END)) SS_LCTL("c") SS_TAP(X_LEFT));
                clipboard_holds_line = true;
            }
            load_rgb(&dy_mode_prev_rgb);
            dy_vim_mode_off();
        } else { // on release:
        }
        break;
    case DY_UNSET:
        if (record->event.pressed) { // on press
            load_rgb(&dy_mode_prev_rgb);
        } else { // on release:
            dy_vim_mode_off();
        }
        break;
	// shift dependent commands -----------------------------------------------
    case NEW_LN:
        if (record->event.pressed) { // on press
            if (vim_shift()) {
                if (JPTR_Mode_On) {
                    SEND_STRING(SS_TAP(X_HOME) SS_TAP(X_ENTER) SS_TAP(X_UP));
                } else {
                    SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_ENTER)) SS_TAP(X_UP));
                }
            } else {
                if (JPTR_Mode_On) {
                    SEND_STRING(SS_TAP(X_END) SS_TAP(X_ENTER));
                } else {
                    SEND_STRING(SS_TAP(X_END) SS_LSFT(SS_TAP(X_ENTER)));
                }
            }
        } else { // on release:
        }
        break;
    case V_PASTE:
        if (record->event.pressed) { // on press
            if (clipboard_holds_line) {
                if (vim_shift()) {
                    if (JPTR_Mode_On) {
                        SEND_STRING(SS_TAP(X_HOME) SS_TAP(X_ENTER) SS_TAP(X_UP) SS_LCTL("v"));
                    } else {
                        SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_ENTER)) SS_TAP(X_UP) SS_LCTL("v"));
                    }
                } else {
                    if (JPTR_Mode_On) {
                        SEND_STRING(SS_TAP(X_END) SS_TAP(X_ENTER) SS_LCTL("v"));
                    } else {
                        SEND_STRING(SS_TAP(X_END) SS_LSFT(SS_TAP(X_ENTER)) SS_LCTL("v"));
                    }
                }
            } else { // regular paste
                if (vim_shift()) {
                    SEND_STRING(SS_LCTL("v"));
                } else {
                    SEND_STRING(SS_TAP(X_RIGHT) SS_LCTL("v"));
                }
            }
        } else { // on release:
        }
        break;
    case PRE_WHI:
        if (record->event.pressed) { // on press
            set_rgb_preset(WHITE_RGB);
        } else { // on release:
        }
        break;
    case PRE_DRA:
        if (record->event.pressed) { // on press
            set_rgb_preset(DRACULA_RGB);
        } else { // on release:
        }
        break;
    case PRE_RED:
        if (record->event.pressed) { // on press
            set_rgb_preset(RED_RGB);
        } else { // on release:
        }
        break;
    case PRE_TWK:
        if (record->event.pressed) { // on press
            set_rgb_preset(TWK_RGB);
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
    KC_LCTL,  KC_LGUI,  KC_LALT,                      KC_SPC,   KC_SPC,   KC_SPC,                       KC_RALT,  MO(_FL),  MO(_FL),  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),
  
  [1] = LAYOUT( // num layer
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_4,     KC_5,     KC_6,     _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_1,     KC_2,     KC_3,     _______,  _______,                      _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_0,     KC_0,     _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                      _______,  _______,  _______,                      _______,  _______,  _______,  _______,  _______,  _______
  ),
  
  [2] = LAYOUT( // function layer
    RESET,    PRE_TWK,  PRE_DRA,  PRE_RED,  PRE_WHI,  _______,  _______,  _______,  _______,  _______,  _______,  RGB_HUD,  RGB_HUI,  RGB_RMOD, RGB_MOD,  _______,
    _______,  X(D_FC),  X(PLEAD), X(IRONY), X(SNEK),  _______,  _______,  _______,  _______,  _______,  KC_HOME,  RGB_VAD,  RGB_VAI,  RGB_TOG,  RGB_TOG,  KC_SLEP,
    KC_ENT,   _______,  NEXT_WD,  KC_ENT,   _______,  _______,  VIM_Y,    M_UNDO,   KC_I,     NEW_LN,   V_PASTE,  RGB_SAD,  RGB_SAI,  JPTR_MDT,           KC_MNXT,
    VIM_MD,   KC_END,   _______,  VIM_D,    _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,                      JPTR_MDT, KC_MPRV,
    VI_LSHFT, _______,  KC_BSPC,  KC_DEL,   _______,  _______,  BACK_WD,  _______,  _______,  TAB_L,    TAB_R,    _______,  VI_RSHFT,           KC_PGUP,  KC_MUTE,
    _______,  _______,  _______,                      KC_ENT,   KC_ENT,   KC_ENT,                       NUM_MDT,  _______,  _______,  VDKTP_L,  KC_PGDN,  VDKTP_R
  ),

  [3] = LAYOUT( // vim mode layer
    REG_MD,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    REG_MD,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_HOME,  _______,  _______,  _______,  _______,  _______,
    REG_MD,   _______,  NEXT_WD,  _______,  _______,  _______,  VIM_Y,    M_UNDO,   REG_MD,   NEW_LN,   V_PASTE,  _______,  _______,  _______,            _______,
    VIM_RST,  APPEND,   _______,  VIM_D,    _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,                      VIENTER,  _______,
    VI_LSHFT, _______,  _______,  KC_DEL,   _______,  _______,  BACK_WD,  _______,  _______,  _______,  _______,  _______,  VI_RSHFT,           _______,  _______,
    _______,  _______,  _______,                      _______,  _______,  _______,                      _______,  _______,  _______,  _______,  _______,  _______
  ),

  [4] = LAYOUT( // delete / yank mode layer
    DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET,
    DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_END,   DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_HOME,  DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET,
    DY_UNSET, DY_UNSET, DY_N_WD,  DY_UNSET, DY_UNSET, DY_UNSET, Y_LINE,   DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET,           DY_UNSET,
    DY_UNSET, DY_UNSET, DY_UNSET, D_LINE,   DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET,                     DY_UNSET, DY_UNSET,
    XXXXXXX,  DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_B_WD,  DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, XXXXXXX,            DY_UNSET, DY_UNSET,
    DY_UNSET, DY_UNSET, DY_UNSET,                     DY_UNSET, DY_UNSET, DY_UNSET,                     DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET, DY_UNSET
  ),

};
