#include <pebble.h>
#include "settings.h"
#include "drawing_utils.h"
#include "layers.h"


static Window *s_main_window;
static Layer *s_window_layer;

static Layer *s_background_layer;
static Layer *s_hands_layer;
static Layer *s_seconds_layer;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick handler fired");
  // Update minute/hour hands if a minute has passed
  if ((units_changed & MINUTE_UNIT) != 0) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "mark hands layer dirty");
    layer_mark_dirty(s_hands_layer);
  }

  // Update seconds hand if enabled
  if (settings.enable_second_hand) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "mark seconds layer dirty");
    layer_mark_dirty(s_seconds_layer);
  }
}

static void initialize_seconds() {
  tick_timer_service_unsubscribe();

  if (settings.enable_second_hand) {
    // Register with TickTimerService
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Subscribed to tick handler for seconds");

    // Show seconds layer
    layer_set_hidden(s_seconds_layer, false);
  } else {
    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Subscribed to tick handler for minutes");

    // Hide seconds layer
    layer_set_hidden(s_seconds_layer, true);
  }
}

// Update the display elements
void prv_update_display() {
  layer_mark_dirty(s_background_layer);
  layer_mark_dirty(s_hands_layer);
  layer_mark_dirty(s_seconds_layer);
  initialize_seconds();
}

static void bluetooth_callback(bool connected) {
  if (!connected && settings.enable_vibrate_on_disconnect) {
    // vibrate watch
    vibes_double_pulse();
  }
}

static void main_window_load(Window *window) {
  // Get information about the Window
  s_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(s_window_layer);

  // Add background layer
  s_background_layer = layer_create(bounds);
  layer_add_child(s_window_layer, s_background_layer);
  layer_set_update_proc(s_background_layer, background_layer_update_proc);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Background layer added");

  // Add hour and minute hands layer
  s_hands_layer = layer_create(bounds);
  layer_add_child(s_window_layer, s_hands_layer);
  layer_set_update_proc(s_hands_layer, hands_layer_update_proc);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Hands layer added");

  // Create seconds layer
  s_seconds_layer = layer_create(bounds);
  layer_set_update_proc(s_seconds_layer, seconds_layer_update_proc);
  layer_add_child(s_window_layer, s_seconds_layer);
  // Show or hide seconds layer, and
  // subscribe to correct tick timer service
  initialize_seconds();

  // Register for Bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  // Show the correct state of the BT connection from the start
  bluetooth_callback(connection_service_peek_pebble_app_connection());
}

static void main_window_unload(Window *window) {
  layer_destroy(s_background_layer);
  layer_destroy(s_hands_layer);
  layer_destroy(s_seconds_layer);
}

static void init() {
  prv_load_settings();

  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  // Create main Window element and assign to pointer
  s_main_window = window_create();

  prv_load_settings();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
