#include <pebble.h>

#define SETTINGS_KEY 8642

// This whole file taken from https://github.com/pebble-examples/clay-example/

// A structure containing our settings
typedef struct ClaySettings {
  bool enable_wordmark;
  char text_wordmark;
  bool enable_double_12;
  
  GColor color_background;
  GColor color_dot;
  GColor color_hour_hand;
  GColor color_minute_hand;
  GColor color_second_hand;
  GColor color_hour_markers;
  GColor color_minute_markers;
  GColor color_wordmark;

  bool enable_second_hand;
  bool enable_vibrate_on_disconnect;
} __attribute__((__packed__)) ClaySettings;


ClaySettings settings;

void prv_default_settings();
void prv_load_settings();
void prv_save_settings();
void prv_update_display();
void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
