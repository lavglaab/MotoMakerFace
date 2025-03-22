#include <pebble.h>

void draw_line(GContext *ctx, GPoint start, GPoint end, int thickness, GColor color);
void draw_hand(GContext *ctx, GPoint start, int angle, int inset, int length, int thickness, GColor color, GColor shadow_color);