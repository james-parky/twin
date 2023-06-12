#ifndef TWIN_WINDOW_H_
#define TWIN_WINDOW_H_

#include <stdarg.h>
#include <stdint.h>
#include <sys/ioctl.h>

struct twin_dim_t {
    uint16_t top, bot, left, right;
};
typedef struct twin_dim_t twin_dim_t;

enum twin_general_style {
    TWIN_GENERAL_THIN_SOLID,
    TWIN_GENERAL_BOLD_SOLID,
    TWIN_GENERAL_THIN_DASHED,
    TWIN_GENERAL_BOLD_DASHED,
    TWIN_GENERAL_THIN_DOTTED,
    TWIN_GENERAL_BOLD_DOTTED,
    TWIN_GENERAL_DOUBLE
};
typedef enum twin_general_style twin_general_style_t;

struct twin_style_t {
    char* horz;
    char* vert;
    char* tl;
    char* tr;
    char* bl;
    char* br;
    char* left_t;
    char* right_t;
    char* top_t;
    char* bot_t;
    char* cross;
};
typedef struct twin_style_t twin_style_t;

struct twin_window_t {
    char* title;
    char* contents;
    uint32_t contents_ptr;
    twin_dim_t* dim;
    char* colour;
    style_t* style;
    struct winsize current_terminal_sz;
    struct winsize original_terminal_sz;
    uint16_t z_index;
};
typedef struct twin_window_t twin_window_t;

struct twin_node_t {
    twin_window_t* window;
    struct twin_node_t* next;
};
typedef struct twin_node_t twin_node_t;

twin_window_t* twin_create_window (const uint16_t top, const uint16_t bot,
                                   const uint16_t left, const uint16_t right);

void twin_resize_window (const twin_window_t* window, const uint16_t top,
                         const uint16_t bot, const uint16_t left,
                         const uint16_t right);

void twin_set_z_index (twin_window_t* window, uint16_t z_index);
void twin_set_colour (twin_window_t* window, const char* colour);
void twin_set_title (twin_window_t* window, const char* title);
void twin_set_contents (twin_window_t* window, const char* contents);

void twin_set_general_style (twin_window_t* win, enum twin_general_style_t);
void twin_set_horizontal_style (twin_window_t* win, const char* horz_style);
void twin_set_vert_style (twin_window_t* win, const char* vert_style);
void twin_set_top_left_style (twin_window_t* win, const char* tl_style);
void twin_set_top_right_style (twin_window_t* win, const char* tr_style);
void twin_set_bottom_left_style (twin_window_t* win, const char*  bl_style);
void twin_set_bottom_right_style (twin_window_t* win, const char* br_style);
void twin_set_cross_style (twin_window_t* win, const char* cross_style);

uint8_t twin_draw_windows (uint8_t num, ...);
uint8_t twin_draw_all_windows();
void twin_draw_window (twin_window_t* window);
uint8_t twin_draw_windows_z (uint8_t num, ...);

void twin_init();

extern window_t* flex_window_head;
extern node_t* flex_windows;
extern node_t* flex_windows_tail;
extern uint8_t window_count;

#endif
