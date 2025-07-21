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
  // calculate minute hand
  int32_t minute_angle = TRIG_MAX_ANGLE * t->tm_min / 60;

  // draw minute hand
  draw_hand(ctx, center, minute_angle, HAND_WIDTH * 2, max_hand_length, HAND_WIDTH, settings.color_minute_hand, settings.color_background);


  // calculate hour hand
  int32_t hour_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);

  // draw hour hand
  draw_hand(ctx, center, hour_angle, HAND_WIDTH * 2, max_hand_length * 0.6, HAND_WIDTH, settings.color_hour_hand, settings.color_background);

  // Draw the center dot
  graphics_context_set_fill_color(ctx, settings.color_dot);
  graphics_fill_circle(ctx, center, HAND_WIDTH / 2);
}

void seconds_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  // calculate hand
  int32_t angle = TRIG_MAX_ANGLE * t->tm_sec / 60;

  // draw hand
  draw_hand(ctx, center, angle, HAND_WIDTH * 2, bounds.size.h, SEC_WIDTH, settings.color_second_hand, settings.color_background);

  // Draw the center dot
  graphics_context_set_fill_color(ctx, settings.color_dot);
  graphics_fill_circle(ctx, center, HAND_WIDTH / 2);
}

void background_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  graphics_context_set_fill_color(ctx, settings.color_background);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // add ticks

  // 5 minute marks
  for (int i = 5; i < 60; i = i + 5) {
    // calculate ray
    int32_t angle = TRIG_MAX_ANGLE * i / 60;

    // draw ray
    draw_hand(ctx, center, angle, 0, bounds.size.h, MINUTE_MARK_WIDTH, settings.color_minute_markers, settings.color_background);
  }

  // 3 6 9 o clock
  for (int i = 15; i < 60; i = i + 15) {
    // calculate ray
    int32_t angle = TRIG_MAX_ANGLE * i / 60;

    // draw ray
    draw_hand(ctx, center, angle, 0, bounds.size.h, HOUR_MARK_WIDTH, settings.color_hour_markers, settings.color_background);
  }

  // 12 o clock
  //int32_t angle = TRIG_MAX_ANGLE * 0 / 60;
  if (settings.enable_double_12) {

    // draw double 12
    draw_hand(ctx, (GPoint) {.x = center.x - HOUR_MARK_WIDTH, .y = center.y}, 90, 0, bounds.size.h, HOUR_MARK_WIDTH, settings.color_hour_markers, settings.color_background);
    draw_hand(ctx, (GPoint) {.x = center.x + HOUR_MARK_WIDTH, .y = center.y}, 90, 0, bounds.size.h, HOUR_MARK_WIDTH, settings.color_hour_markers, settings.color_background);
  } else {
    // draw single 12
    draw_hand(ctx, center, 90, 0, bounds.size.h, HOUR_MARK_WIDTH, settings.color_hour_markers, settings.color_background);
  }

  // erase rays beyond inset
  graphics_context_set_fill_color(ctx, settings.color_background);
  PBL_IF_ROUND_ELSE( 
    graphics_fill_circle(ctx, center, (bounds.size.w / 2) - TICK_INSETS),
    graphics_fill_rect(
      ctx,
      GRect(TICK_INSETS, TICK_INSETS, bounds.size.w - (2 *TICK_INSETS), bounds.size.h - (2 * TICK_INSETS)),
      0,
      GCornerNone
      )
  );

  if (
    layer_get_bounds(layer).size.h == bounds.size.h &&
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
      GRect(0, (TICK_INSETS * 1.5), bounds.size.w, 24),
      GTextOverflowModeTrailingEllipsis,
      GTextAlignmentCenter,
      s_attributes
    );
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Background layer drawn");
}