#include <pebble.h>
#include "settings.h"

// A struct for our specific settings (see main.h)
ClaySettings settings;

// Initialize the default settings
void prv_default_settings() {
  settings.enable_wordmark = true;
  // const char *default = "pebble";
  // memset(settings.text_wordmark, 0, sizeof settings.text_wordmark);
  // strncpy(settings.text_wordmark, default, sizeof settings.text_wordmark-1);
  settings.enable_double_12 = true;
  
  settings.color_background = GColorBlack;
  settings.color_dot = GColorWhite;
  settings.color_hour_hand = GColorWhite;
  settings.color_minute_hand = GColorWhite;
  settings.color_second_hand = PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
  settings.color_hour_markers = GColorLightGray;
  settings.color_minute_markers = PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite);
  settings.color_wordmark = PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite);

  settings.enable_second_hand = true;
  settings.enable_vibrate_on_disconnect = true;
}

// Read settings from persistent storage
void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}

// Handle the response from AppMessage
void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Widgets
  Tuple *enable_wordmark_t = dict_find(iter, MESSAGE_KEY_enableWordmark);
  if (enable_wordmark_t) {
    settings.enable_wordmark = enable_wordmark_t->value->int32 == 1;
  }
  // Tuple *text_wordmark_t = dict_find(iter, MESSAGE_KEY_textWordmark);
  // if (text_wordmark_t) {
  //   settings.text_wordmark = (char) text_wordmark_t->value->cstring;
  // }
  Tuple *enable_double_12_t = dict_find(iter, MESSAGE_KEY_enableDouble12);
  if (enable_double_12_t) {
    settings.enable_double_12 = enable_double_12_t->value->int32 == 1;
  }
  
  // Colors
  Tuple *color_background_t = dict_find(iter, MESSAGE_KEY_colorBackground);
  if (color_background_t) {
    settings.color_background = GColorFromHEX(color_background_t->value->int32);
  }
  Tuple *color_dot_t = dict_find(iter, MESSAGE_KEY_colorDot);
  if (color_dot_t) {
    settings.color_dot = GColorFromHEX(color_dot_t->value->int32);
  }
  Tuple *color_hour_markers_t = dict_find(iter, MESSAGE_KEY_colorHourMarkers);
  if (color_hour_markers_t) {
    settings.color_hour_markers = GColorFromHEX(color_hour_markers_t->value->int32);
  }
  Tuple *color_minute_markers_t = dict_find(iter, MESSAGE_KEY_colorMinuteMarkers);
  if (color_minute_markers_t) {
    settings.color_minute_markers = GColorFromHEX(color_minute_markers_t->value->int32);
  }
  Tuple *color_hour_hand_t = dict_find(iter, MESSAGE_KEY_colorHourHand);
  if (color_hour_hand_t) {
    settings.color_hour_hand = GColorFromHEX(color_hour_hand_t->value->int32);
  }
  Tuple *color_minute_hand_t = dict_find(iter, MESSAGE_KEY_colorMinuteHand);
  if (color_minute_hand_t) {
    settings.color_minute_hand = GColorFromHEX(color_minute_hand_t->value->int32);
  }
  Tuple *color_second_hand_t = dict_find(iter, MESSAGE_KEY_colorSecondHand);
  if (color_second_hand_t) {
    settings.color_second_hand = GColorFromHEX(color_second_hand_t->value->int32);
  }
  Tuple *color_logo_t = dict_find(iter, MESSAGE_KEY_colorWordmark);
  if (color_logo_t) {
    settings.color_wordmark = GColorFromHEX(color_logo_t->value->int32);
  }

  // Features
  Tuple *enable_second_hand_t = dict_find(iter, MESSAGE_KEY_enableSecondHand);
  if (enable_second_hand_t) {
    settings.enable_second_hand = enable_second_hand_t->value->int32 == 1;
  }
  Tuple *enable_vibrate_on_disconnect_t = dict_find(iter, MESSAGE_KEY_enableVibrateOnDisconnect);
  if (enable_vibrate_on_disconnect_t) {
    settings.enable_vibrate_on_disconnect = enable_vibrate_on_disconnect_t->value->int32 == 1;
  }

  // Save the new settings to persistent storage
  prv_save_settings();
}