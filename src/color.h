#ifndef COLOR_HEADER
#define COLOR_HEADER

#include <ncurses.h>
#include <stdbool.h>

#define SET_COLOR(c) (attr_set(0, (c), 0))
#define WSET_COLOR(w, c) (wattr_set((w), 0, (c), 0))
#define UNSET_COLOR(c) (SET_COLOR(0))
#define WUNSET_COLOR(w, c) (WSET_COLOR(w, 0))
#define PAIR_COLOR(f, g) (((f) << 8) + (g))

typedef struct color {
    int r, g, b;
    int cnt;
} color;

void reset_colors();
void init_color_pairs();
int get_color_id(int r, int g, int b);
void unget_color_id(int i);

#endif