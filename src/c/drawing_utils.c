#include <pebble.h>
#include "drawing_utils.h"
#include "settings.h"

void draw_line(
  GContext *ctx,
  GPoint start,
  GPoint end,
  int thickness,
  GColor color
  ) {
    // Set stroke style
    graphics_context_set_stroke_color(ctx, color);
    graphics_context_set_stroke_width(ctx, thickness);

    // Draw stroke
    graphics_draw_line(ctx, start, end);
}

void draw_hand(
  GContext *ctx,
  GPoint start,
  int angle,
  int from_center,
  int length,
  int thickness,
  GColor color,
  GColor shadow_color
) {

  // Calculate where the start point of the hand goes
  GPoint hand_start = {
    .x = (int16_t)(sin_lookup(angle) * (int32_t)from_center / TRIG_MAX_RATIO) + start.x,
    .y = (int16_t)(-cos_lookup(angle) * (int32_t)from_center / TRIG_MAX_RATIO) + start.y,
  };

  // Calculate where the end point of the hand goes
  GPoint hand_end = {
    .x = (int16_t)(sin_lookup(angle) * (int32_t)length / TRIG_MAX_RATIO) + start.x,
    .y = (int16_t)(-cos_lookup(angle) * (int32_t)length / TRIG_MAX_RATIO) + start.y,
  };

  // Draw a shadow around the hand
  draw_line(ctx, hand_start, hand_end, thickness + 2, shadow_color);
  // Draw the actual hand
  draw_line(ctx, hand_start, hand_end, thickness, color);
}