#include <pebble.h>
#include "layers.h"
#include "drawing_utils.h"
#include "settings.h"
#include "style.h"

void hands_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  int16_t max_hand_length = ((bounds.size.w - HAND_INSET) / 2);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  /*struct tm t;
  t.tm_hour = 10;
  t.tm_min = 10;
  t.tm_sec = 31; */

  // Calculate angles for hands
  int second_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
  int32_t minute_angle = TRIG_MAX_ANGLE * t->tm_min / 60;
  int32_t hour_angle = TRIG_MAX_ANGLE * t->tm_hour / 12;

  hour_angle += minute_angle / 12; // Adjust hour position for progress of minutes
  if (settings.enable_second_hand) { minute_angle += second_angle / 60; } // Adjust minute position for progress of seconds

  // draw minute hand
  draw_hand(ctx, center, minute_angle, HAND_WIDTH * 2, max_hand_length, HAND_WIDTH, settings.color_minute_hand, settings.color_background);
  
  // draw hour hand
  draw_hand(ctx, center, hour_angle, HAND_WIDTH * 2, max_hand_length * 0.6, HAND_WIDTH, settings.color_hour_hand, settings.color_background);

  // draw second hand
  if (settings.enable_second_hand) {
    draw_hand(ctx, center, second_angle, HAND_WIDTH * 2, bounds.size.h, SEC_WIDTH, settings.color_second_hand, settings.color_background);
  }
  // Draw the center dot
  graphics_context_set_fill_color(ctx, settings.color_dot);
  graphics_fill_circle(ctx, center, HAND_WIDTH / 2);
}

void background_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect unobstructed_bounds = layer_get_unobstructed_bounds(layer);
  GPoint center = grect_center_point(&unobstructed_bounds);

  graphics_context_set_fill_color(ctx, settings.color_background);
  graphics_fill_rect(ctx, unobstructed_bounds, 0, GCornerNone);

  // Hour markers
  for (int hour = 0; hour < 12; hour++) {
    int32_t angle = TRIG_MAX_ANGLE * hour / 12; //
    
    if (hour == 0 && settings.enable_double_12) {
      // Do double mark at the 12 Noon position
      draw_hand(ctx, (GPoint) {.x = center.x - HOUR_MARK_WIDTH, .y = center.y}, 90, 0, unobstructed_bounds.size.h, HOUR_MARK_WIDTH, settings.color_hour_markers, settings.color_background);
      draw_hand(ctx, (GPoint) {.x = center.x + HOUR_MARK_WIDTH, .y = center.y}, 90, 0, unobstructed_bounds.size.h, HOUR_MARK_WIDTH, settings.color_hour_markers, settings.color_background);
    } else if (hour % 3 == 0) {
      // We're at 0, 3, 6, or 9; do the thicker mark
      draw_hand(ctx, center, angle, 0, unobstructed_bounds.size.h, HOUR_MARK_WIDTH, settings.color_hour_markers, settings.color_background);
    } else {
      // Do the usual thin mark
      draw_hand(ctx, center, angle, 0, unobstructed_bounds.size.h, MINUTE_MARK_WIDTH, settings.color_minute_markers, settings.color_background);
    }
  }

  // erase rays beyond inset
  graphics_context_set_fill_color(ctx, settings.color_background);
  PBL_IF_ROUND_ELSE( 
    graphics_fill_circle(ctx, center, (unobstructed_bounds.size.w / 2) - TICK_INSETS),
    graphics_fill_rect(
      ctx,
      GRect(TICK_INSETS, TICK_INSETS, unobstructed_bounds.size.w - (2 *TICK_INSETS), unobstructed_bounds.size.h - (2 * TICK_INSETS)),
      0,
      GCornerNone
      )
  );

  if (
    unobstructed_bounds.size.h > 160 &&
    settings.enable_wordmark
  ) {
    // Draw wordmark
    graphics_context_set_text_color(ctx, settings.color_wordmark);
    GTextAttributes *s_attributes = graphics_text_attributes_create();
    // Enable circular text flow on round watches
    #if defined(PBL_ROUND)
      graphics_text_attributes_enable_screen_text_flow(s_attributes, 5);
    #endif
    graphics_draw_text(
      ctx,
      (watch_info_get_model() > WATCH_INFO_MODEL_PEBBLE_TIME_2) ? "core" : "pebble",
      fonts_get_system_font(WORDMARK_FONT),
      GRect(0, (TICK_INSETS * 1.5), unobstructed_bounds.size.w, 24),
      GTextOverflowModeTrailingEllipsis,
      GTextAlignmentCenter,
      s_attributes
    );
  }
}