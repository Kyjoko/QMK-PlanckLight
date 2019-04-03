#include "light.h"
#include "eeprom.h"

#define ALT_TAB LALT(KC_TAB)
#define CTRL_ALT_DEL LCTL(LALT(KC_DEL))

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum planck_layers {

  //Keymap Layers
  _DEFAULT, 
  _LOWER,
  _RAISE,
  _ADJUST,
  _HERO

  //LED Layer
  _GAMING
};

bool gamingMode = false;
bool darkMode = false;
bool cycleMode = false;
bool pressed0 = false;
bool pressed1 = false;

rgb_config_t rgb_matrix_config;
uint32_t g_tick;
uint8_t g_key_hit[DRIVER_LED_TOTAL];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	/* Defsult
	 * ,----------------------------------------------------------------------------------.
	 * | Esc  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
	 * |-----------------------------------------------------------------------------------|
	 * |  Tab |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |   \  |
	 * |-----------------------------------------|-----------------------------------------|
	 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
	 * |-----------------------------------------------------------------------------------|
	 * | Ctrl | GUI  | Alt  | Raise|Lower |    Space    |Raise | Alt  | GUI  |C-A-D | Ctrl |
	 * `----------------------------------------------------------------------------------'
	 */
	[_DEFAULT] = LAYOUT_planck_mit(
	  KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
	  KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_BSLS,
	  KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ENT,
	  KC_LCTL, KC_LGUI, KC_LALT, MO(_RAISE), MO(_LOWER), KC_SPC, MO(_RAISE), KC_RALT, KC_RGUI, CTRL_ALT_DEL, KC_RCTL
	),

	/* Lower
	 * ,-----------------------------------------------------------------------------------.
	 * |   ~   |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  | F7  | F8  |  F9  |  F10  |Clicky |
	 * |------------------------------------------------------------------------------------|
	 * | AltTab|  F11 |  F12 |  F13 |  F14 |  F15 | F16  |     | Ins | Home |PgUp   |       |
	 * |------------------------------------------|-----------------------------------------|
	 * |       |   F7 |  F8  |  F9  |  F10 |  F11 | F12  |     | Del | Enc  |PgDown |       |
	 * |----------------------------------------------------------------------------+-------|
	 * |       |      |      |      |      |             |     | Next| Vol- |Vol-   | Play  |
	 * `-----------------------------------------------------------------------------------'
	 */
	[_LOWER] = LAYOUT_planck_mit(
	  KC_TILD, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, CK_TOGG,
	  ALT_TAB, KC_F11, KC_F12, KC_F13, KC_F14, KC_F15, KC_F16, _______, KC_INS, KC_HOME, KC_PGUP, _______,
	  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, _______, KC_DEL, KC_END, KC_PGDN,  _______,
	  _______,  _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
	),

	/* Raise
	 * ,------------------------------------------------------------------------------------.
	 * |   `    |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |      |
	 * |-------------------------------------------------------------------------------------|
	 * |CAPSLOCK|      |      |      |      |   @  |  *   |  -   |  =   |  [   |  ]   |  \   |
	 * |-----------------------------------------|-------------------------------------------|
	 * |        |      |      |      |      |   ^  |  &   |  '   |  `   |  ;   |  /   |      |
	 * |-------------------------------------------------------------------------------------|
	 * |        |      |      |      |      |             |      |  <-  |  ^   |  v   | ->   |
	 * `------------------------------------------------------------------------------------'
	 */
	[_RAISE] = LAYOUT_planck_mit(
	  KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, _______,
	  KC_CAPS, _______, _______, _______, _______, KC_AT, KC_ASTR, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,
	  _______, _______, _______, _______, _______, KC_CIRC, KC_AMPR, KC_QUOT, KC_GRV, KC_SCLN, KC_SLSH, _______,
	  _______,  _______, _______, _______, _______, _______, _______, KC_LEFT, KC_UP, KC_DOWN, KC_RGHT
	),

	/* Lower - Raise
	 * ,------------------------------------------------------------------------------------------.
	 * |   `  |       |       |      |      |      |      |      |       |       | DEBUG  | RESET  |
	 * |-------------------------------------------------------------------------------------------|
	 * |  Del |RGB-TOG|RGB-MOD|RGB-H+|RGB H-|RGB S+|RGB S-|RGB V+|RGB V- |RGB SP+|RGB SP- |        |
	 * |-------------------------------------------|-----------------------------------------------|
	 * |      | MU-MOD| AU-ON |AU-OFF| MI-ON|MI-OFF|MU-ON |MU-OFF|MU-VOL-|MU-VOL+|TOG_RNBW|TOG_MOD-|
	 * |-------------------------------------------------------------------------------------------|
	 * |TOG_GH|       |       |      |      |             |      |  <-   |   ^   |   v    |  ->    |
	 * `------------------------------------------------------------------------------------------'
	 */

	[_ADJUST] = LAYOUT_planck_mit(
	  KC_GRV, _______,  _______, _______, _______, _______, _______, _______, _______, _______, DEBUG, RESET,
	  KC_DEL, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, RGB_SPI, RGB_SPD, _______,
	  _______, MU_MOD,  AU_ON,   AU_OFF, MI_ON,   MI_OFF, MU_ON,   MU_OFF, MUV_DE,  MUV_IN, M(0), M(1),
	  TG(_HERO),  _______, _______, _______, _______, KC_ACL0, _______,  KC_MS_LEFT, KC_MS_UP, KC_MS_DOWN, KC_MS_RIGHT
	),

	/* Guitar Hero
	 * ,----------------------------------------------------------------------------------------.
	 * |   `  |       |       |      |      |      |      |      |       |       |       |       |
	 * |-----------------------------------------------------------------------------------------|
	 * |      |       |       |      |      |      |      |      |       |       |       |       |
	 * |-------------------------------------------|---------------------------------------------|
	 * |      |       |       |      | ENTER|      |      |      |       |       |       |       |
	 * |-----------------------------------------------------------------------------------------|
	 * |  ESC |       |       |      | SHIFT|    ENTER    |  T   |   R   |   E   |   W   |   Q   |
	 * `----------------------------------------------------------------------------------------'
	 */

	[_HERO] = LAYOUT_planck_mit(
	  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	  _______, _______, _______, _______, KC_ENT, _______, _______, _______, _______, _______, _______, _______,
	  KC_ESC, _______, _______, _______, KC_RSFT, KC_ENT, KC_T, KC_R, KC_E, KC_W, KC_Q
	)

};

void matrix_init_user(void) {
  rgb_matrix_config.enable = 1;
  rgb_matrix_config.hue = -1;
  rgb_matrix_config.sat = -1;
  rgb_matrix_config.raw = eeprom_read_dword(EECONFIG_RGB_MATRIX);

}

const uint8_t PROGMEM layercolors[][2] = {
  [0] = {255,255}
};

/*int16_t h1 = 0;
    int16_t h2 = (0 + 180) % 255;
    int16_t deltaH = h2 - h1;

    // Take the shortest path between hues
    if ( deltaH > 127 )
    {
        deltaH -= 256;
    }
    else if ( deltaH < -127 )
    {
        deltaH += 256;
    }
    // Divide delta by 4, this gives the delta per row
    deltaH /= 4;

    int16_t s1 = rgb_matrix_config.sat;
    int16_t s2 = 0;
    int16_t deltaS = ( s2 - s1 ) / 4;

    HSV hsv = { .h = 0, .s = 255, .v = rgb_matrix_config.val };
    RGB rgb;
    Point point;
    for ( int i=0; i<DRIVER_LED_TOTAL; i++ )
    {
        // map_led_to_point( i, &point );
        point = g_rgb_leds[i].point;
        // The y range will be 0..64, map this to 0..4
        uint8_t y = (point.y>>4);
        // Relies on hue being 8-bit and wrapping
        hsv.h = 0 + ( deltaH * y );
        hsv.s = rgb_matrix_config.sat + ( deltaS * y );
        rgb = hsv_to_rgb( hsv );
*/


const uint16_t PROGMEM ledcolors[][DRIVER_LED_TOTAL][3] = {
  [_DEFAULT] = {
   {210,255,1}, {0, 0, 0}, {160, 255, 4}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {210,255,1}, 
    {210,255,1}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},{0, 0, 0}, {0, 0, 0}, {0, 0, 0},  {210,255,1}, 
    {210,255,1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {210,255,1}, 
    {210,255,1}, {210,255,1}, {210,255,1}, {179, 255, 2}, {140, 255, 3}, {210,255,1}, {210,255,1}, {210,255,1}, {179, 255, 2}, {210,255,1}, {210,255,1}, {0,0,255}, {210,255,1}
  },

  [_LOWER] = {
    {210,255,1}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3},{210,255,1},
    {179, 255, 2}, {140, 255, 3}, {140, 255, 3}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {179, 255, 2}, {179, 255, 2},{179, 255, 2}, {210,255,1},
    {210,255,1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {179, 255, 2}, {179, 255, 2},{179, 255, 2}, {210,255,1},
    {210,255,1}, {210,255,1}, {210,255,1}, {179, 255, 2}, {140, 255, 3}, {210,255,1}, {210,255,1}, {210,255,1}, {179, 255, 2}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}
  },

  [_RAISE] = {
    {210,255,1}, {179, 255, 2}, {179, 255, 2}, {179, 255, 2},{179, 255, 2}, {179, 255, 2}, {179, 255, 2}, {179, 255, 2}, {179, 255, 2},{179, 255, 2}, {179, 255, 2}, {210,255,1},
    {140, 255, 3}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3},
    {210,255,1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {140, 255, 3}, {210,255,1},
    {210,255,1}, {210,255,1}, {210,255,1}, {140, 255, 3}, {179, 255, 2}, {210,255,1}, {210,255,1}, {210,255,1}, {140, 255, 3}, {179, 255, 2}, {179, 255, 2},{179, 255, 2}, {179, 255, 2}
  },

  [_ADJUST] = {
    {210,255,1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {179, 255, 2},
    {210,255,1}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4},  {210,255,1},
    {210,255,1}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {140, 255, 3}, {140, 255, 3},
    {140, 255, 3}, {210,255,1}, {210,255,1}, {160, 255, 4}, {160, 255, 4}, {210,255,1}, {210,255,1}, {210,255,1}, {160, 255, 4}, {210,255,1}, {210,255,1}, {210,255,1}, {210,255,1}
  },

  [_HERO] = {
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {270, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {270, 255, 255}, {165, 0, 255}, {270, 0, 255}, {165, 0, 255}, {30, 255, 255}, {240, 255, 255}, {60, 255, 255}, {0, 255, 255}, {120, 255, 255}
  },

  [_GAMING] = {
    {179, 255, 2}, {0, 0, 0}, {160, 255, 4}, {0, 0, 0}, {179, 255, 2}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
    {0, 0, 0}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},{0, 0, 0}, {0, 0, 0}, {0, 0, 0},  {0, 0, 0}, 
    {179, 255, 2}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {179, 255, 2}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
    {179, 255, 2}, {0, 0, 0}, {0, 0, 0}, {179, 255, 2}, {140, 255, 3}, {160, 255, 4}, {160, 255, 4}, {160, 255, 4}, {179, 255, 2}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
  }
};

void set_leds_color( int layer) {

  rgb_matrix_config.raw = eeprom_read_dword(EECONFIG_RGB_MATRIX);

  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    HSV hsv;

    // if the brightness of the led is set to 0 in the map,
    // the led is controlled by the hue value of the board.
    // If the brightness is 1, the led is instead the
    // inverted color of the hue value.
    switch(pgm_read_byte(&ledcolors[layer][i][2])){
      case 0:
      {
        if((gamingMode || darkMode) && !pressed1)
        {
          //Modifiers affected by background color
          hsv.h = rgb_matrix_config.hue;
          hsv.s = rgb_matrix_config.sat;
          hsv.v = 0;
          
          break;
        } 
        else if (cycleMode)
        {
          uint8_t hue1 = ( g_tick/8 << rgb_matrix_config.speed ) & 0xFF;

          //User controlled hue of background
          hsv.h = (hue1)% 360;
          hsv.s = rgb_matrix_config.sat;
          hsv.v = rgb_matrix_config.val;

          break;
        }
        else if (!pressed1)
        {
          //User controlled hue of background
          hsv.h = rgb_matrix_config.hue;
          hsv.s = rgb_matrix_config.sat;
          hsv.v = rgb_matrix_config.val;

          break;
        }
      }
      case 1:
      {
        if(gamingMode)
        {
          //Modifiers affected by background color
          hsv.h = (rgb_matrix_config.hue + 110) % 360;
          hsv.s = rgb_matrix_config.sat;
          hsv.v = 0;
        } 
        else if (cycleMode)
        {
          uint8_t hue1 = ( g_tick/8 << rgb_matrix_config.speed ) & 0xFF;

          //Modifiers affected by background color
          hsv.h = (hue1+90) % 360;
          hsv.s = rgb_matrix_config.sat;
          hsv.v = rgb_matrix_config.val;
        }
        else
        { 
          //Modifiers affected by background color
          hsv.h = (rgb_matrix_config.hue + 110) % 360;
          hsv.s = rgb_matrix_config.sat;
          hsv.v = rgb_matrix_config.val;
        }

        break;
      }
      case 2:
      {
        //Buttons affected by background color
        hsv.h = (rgb_matrix_config.hue + 80) % 360;
        hsv.s = rgb_matrix_config.sat;
        hsv.v = rgb_matrix_config.val;

        break;
      } 
      case 3:
      {
        //Buttons affected by background color
        hsv.h = (rgb_matrix_config.hue + 50) % 360;
        hsv.s = rgb_matrix_config.sat;
        hsv.v = rgb_matrix_config.val;

        break;
      }
      case 4:
      {
        //Buttons affected by background color
        hsv.h = (rgb_matrix_config.hue + 20) % 360;
        hsv.s = rgb_matrix_config.sat;
        hsv.v = rgb_matrix_config.val;

        break;
      } 
      default: 
      {
        //Unaffected bound buttons
        hsv.h = pgm_read_byte(&ledcolors[layer][i][0]);
        hsv.s = pgm_read_byte(&ledcolors[layer][i][1]);
        hsv.v = rgb_matrix_config.val;

        break;
      }
    }

    RGB rgb = hsv_to_rgb( hsv );
    rgb_matrix_set_color( i, rgb.r, rgb.g, rgb.b );
  }
}

void set_layer_color( uint8_t layer ) {
  HSV hsv = { .h = pgm_read_byte(&layercolors[layer][0]), .s = pgm_read_byte(&layercolors[layer][1]), .v = rgb_matrix_config.val};
  RGB rgb = hsv_to_rgb( hsv );
  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    rgb_matrix_set_color( i, rgb.r, rgb.g, rgb.b );
  }
}

void rgb_matrix_indicators_user(void) {
  uint32_t mode = rgblight_get_mode();
  // assign colors if the matrix is on and the current mode
  // is SOLID COLORS => No animations running
  if(rgb_matrix_config.enable == 1 && mode == 1) {
    uint8_t layer = biton32(layer_state);
    switch (layer) {
      case _DEFAULT:
        if(gamingMode == true)
        {
        set_leds_color(_GAMING);
        }
        else
        {
        set_leds_color(_DEFAULT);
        }
        break;
      case _LOWER:
        set_leds_color(_LOWER);
        break;
      case _RAISE:
        set_leds_color(_RAISE);
        break;
      case _ADJUST:
        set_leds_color(_ADJUST);
        break;
      case _HERO:
        set_leds_color(_HERO);
        break;
    }
  }
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
  switch(id) {
    if (record->event.pressed) {
      case 0:
        if (cycleMode && !pressed0)
        {
          cycleMode = false;
          pressed0 = true;
        }
        else if (!cycleMode && pressed0)
        {
          cycleMode = false;
          pressed0 = false;
        }
        else if (!cycleMode && !pressed0)
        {
          cycleMode = true;
          pressed0 = true;
        }
        else if (cycleMode && pressed0)
        {
          cycleMode = true;
          pressed0 = false;
        }
        break;
      case 1:
        if (!darkMode && !gamingMode && !pressed1)
        {
          darkMode = false;
          gamingMode = false;
          pressed1 = true;
        }
        else if (!darkMode && !gamingMode && pressed1)
        {
          darkMode = false;
          gamingMode = true;
          pressed1 = false;
        }
        else if (!darkMode && gamingMode && !pressed1)
        {
          darkMode = false;
          gamingMode = true;
          pressed1 = true;
        }
        else if (!darkMode && gamingMode && pressed1)
        {
          darkMode = true;
          gamingMode = false;
          pressed1 = false;
        }
        else if (darkMode && !gamingMode && !pressed1)
        {
          darkMode = true;
          gamingMode = true;
          pressed1 = true;
        }
        else if (darkMode && gamingMode && pressed1)
        {
          darkMode = false;
          gamingMode = false;
          pressed1 = false;
        }
        

        break;
    }
  }
  return MACRO_NONE;
};

uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}


/*#ifdef AUDIO_ENABLE
  float 1song[][2]  = SONG(MARIO_MUSHROOM);
  float 2song[][2]  = SONG(SONIC_RING);
  float 3song[][2]  = SONG(OLD_SPICE);
  float 4song[][2]  = SONG(MEGALOVANIA);

  bool has_layer_changed = true;

  void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);
    static uint8_t old_layer = 0;

    if (old_layer != layer) {
      has_layer_changed = true;
      old_layer = layer;
    }

    if (has_layer_changed) {
      has_layer_changed = false;

      switch (layer) {
        case 1:
          PLAY_SONG(1song);
        break;
        case 2:
          PLAY_SONG(2song);
        break;
        case 3:
          PLAY_SONG(4song);
        break;
        case 4:
          PLAY_SONG(3song);
        break;
      }
    }
  };
#endif*/

void matrix_scan_user(void) {
}

//bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//	return true;
//}

void led_set_user(uint8_t usb_led) {

	if (usb_led & (1 << USB_LED_NUM_LOCK)) {
		
	} else {
		
	}

	if (usb_led & (1 << USB_LED_CAPS_LOCK)) {
		
	} else {
		
	}

	if (usb_led & (1 << USB_LED_SCROLL_LOCK)) {
		
	} else {
		
	}

	if (usb_led & (1 << USB_LED_COMPOSE)) {
		
	} else {
		
	}

	if (usb_led & (1 << USB_LED_KANA)) {
		
	} else {
		
	}

}