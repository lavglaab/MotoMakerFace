#include <pebble.h>
#include "layers.h"
#include "drawing_utils.h"
#include "settings.h"

void hands_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  const int16_t max_hand_length = PBL_IF_ROUND_ELSE(((bounds.size.w - 30) / 2), (bounds.size.w - 10) / 2);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  // calculate minute hand
  int32_t minute_angle = TRIG_MAX_ANGLE * t->tm_min / 60;

  // draw minute hand
  draw_hand(ctx, center, minute_angle, 10, max_hand_length, 5, settings.color_minute_hand, settings.color_background);


  // calculate hour hand
  int32_t hour_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);

  // draw hour hand
  draw_hand(ctx, center, hour_angle, 10, max_hand_length * 0.6, 5, settings.color_hour_hand, settings.color_background);

  // Draw the center dot
  graphics_context_set_fill_color(ctx, settings.color_dot);
  graphics_fill_circle(ctx, center, 3);
}

void seconds_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  // calculate hand
  int32_t angle = TRIG_MAX_ANGLE * t->tm_sec / 60;

  // draw hand
  draw_hand(ctx, center, angle, 10, bounds.size.h, 2, settings.color_second_hand, settings.color_background);

  // Draw the center dot
  graphics_context_set_fill_color(ctx, settings.color_dot);
  graphics_fill_circle(ctx, center, 3);
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
    draw_hand(ctx, center, angle, 0, bounds.size.h, 1, settings.color_minute_markers, settings.color_background);
  }

  // 3 6 9 o clock
  for (int i = 15; i < 60; i = i + 15) {
    // calculate ray
    int32_t angle = TRIG_MAX_ANGLE * i / 60;

    // draw ray
    draw_hand(ctx, center, angle, 0, bounds.size.h, 2, settings.color_hour_markers, settings.color_background);
  }

  // 12 o clock
  //int32_t angle = TRIG_MAX_ANGLE * 0 / 60;
  if (settings.enable_double_12) {

    // draw double 12
    draw_hand(ctx, (GPoint) {.x = center.x - 3, .y = center.y}, 90, 0, bounds.size.h, 2, settings.color_hour_markers, settings.color_background);
    draw_hand(ctx, (GPoint) {.x = center.x + 3, .y = center.y}, 90, 0, bounds.size.h, 2, settings.color_hour_markers, settings.color_background);
  } else {
    // draw single 12
    draw_hand(ctx, center, 90, 0, bounds.size.h, 2, settings.color_hour_markers, settings.color_background);
  }
  
  const int16_t mark_inset = PBL_IF_ROUND_ELSE(30, 26);

  // erase rays beyond inset
  graphics_context_set_fill_color(ctx, settings.color_background);
  PBL_IF_ROUND_ELSE( 
    graphics_fill_circle(ctx, center, (bounds.size.w / 2) - mark_inset),
    graphics_fill_rect(
      ctx,
      GRect(mark_inset, mark_inset, bounds.size.w - (2 *mark_inset), bounds.size.h - (2 * mark_inset)),
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
    graphics_draw_text(
      ctx,
      "pebble",
      fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
      GRect(0, 37, bounds.size.w, 24),
      GTextOverflowModeTrailingEllipsis,
      GTextAlignmentCenter,
      0
    );
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Background layer drawn");
}