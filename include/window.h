#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdarg.h>
#include <sys/ioctl.h>

struct dim_t {
    size_t top, bot, left, right;
};
typedef struct dim_t dim_t;

// HORIZONTALS
enum horz_style {
    HORZ_THIN_SOLID, 
    HORZ_BOLD_SOLID,
    HORZ_THIN_DASHED, 
    HORZ_BOLD_DASHED, 
    HORZ_THIN_DOTTED, 
    HORZ_BOLD_DOTTED, 
    HORZ_DOUBLE
};

// VERTICALS
enum vert_style {
    VERT_THIN_SOLID,
    VERT_BOLD_SOLID,
    VERT_THIN_DASHED,
    VERT_BOLD_DASHED,
    VERT_THIN_DOTTED,
    VERT_BOLD_DOTTED,
    VERT_DOUBLE
};

// TL
enum tl_style {
    TL_THIN,
    TL_TOP_HEAVY,
    TL_SIDE_HEAVY,
    TL_BOLD,
    TL_TOP_DOUBLE,
    TL_SIDE_DOUBLE,
    TL_DOUBLE,
    TL_CURVED
};

// TR
enum tr_style {
    TR_THIN,
    TR_TOP_HEAVY,
    TR_SIDE_HEAVY,
    TR_BOLD,
    TR_TOP_DOUBLE,
    TR_SIDE_DOUBLE,
    TR_DOUBLE,
    TR_CURVED
};

// BL
enum bl_style {
    BL_THIN,
    BL_TOP_HEAVY,
    BL_SIDE_HEAVY,
    BL_BOLD,
    BL_TOP_DOUBLE,
    BL_SIDE_DOUBLE,
    BL_DOUBLE,
    BL_CURVED
};

// BR
enum br_style {
    BR_THIN,
    BR_TOP_HEAVY,
    BR_SIDE_HEAVY,
    BR_BOLD,
    BR_TOP_DOUBLE,
    BR_SIDE_DOUBLE,
    BR_DOUBLE,
    BR_CURVED
};

// LEFT T
enum left_t_style {
    LEFT_T_THIN,
    LEFT_T_MID_HEAVY,
    LEFT_T_TOP_HEAVY,
    LEFT_T_BOT_HEAVY,
    LEFT_T_TOP_BOT_HEAVY,
    LEFT_T_TOP_MID_HEAVY,
    LEFT_T_BOT_MID_HEAVY,
    LEFT_T_BOLD,
    LEFT_T_DOUBLE_HORZ,
    LEFT_T_DOUBLE_VERT,
    LEFT_T_DOUBLE_BOTH
};

// RIGHT T
enum right_t_style {
    RIGHT_T_THIN,
    RIGHT_T_MID_HEAVY,
    RIGHT_T_TOP_HEAVY,
    RIGHT_T_BOT_HEAVY,
    RIGHT_T_TOP_BOT_HEAVY,
    RIGHT_T_TOP_MID_HEAVY,
    RIGHT_T_BOT_MID_HEAVY,
    RIGHT_T_BOLD,
    RIGHT_T_DOUBLE_HORZ,
    RIGHT_T_DOUBLE_VERT,
    RIGHT_T_DOUBLE_BOTH
};
// TOP T
enum top_t_style {
    TOP_T_THIN,
    TOP_T_MID_HEAVY,
    TOP_T_TOP_HEAVY,
    TOP_T_BOT_HEAVY,
    TOP_T_TOP_BOT_HEAVY,
    TOP_T_TOP_MID_HEAVY,
    TOP_T_BOT_MID_HEAVY,
    TOP_T_BOLD,
    TOP_T_DOUBLE_HORZ,
    TOP_T_DOUBLE_VERT,
    TOP_T_DOUBLE_BOTH
};

// BOT T
enum bot_t_style {
    BOT_T_THIN,
    BOT_T_MID_HEAVY,
    BOT_T_TOP_HEAVY,
    BOT_T_BOT_HEAVY,
    BOT_T_TOP_BOT_HEAVY,
    BOT_T_TOP_MID_HEAVY,
    BOT_T_BOT_MID_HEAVY,
    BOT_T_BOLD,
    BOT_T_DOUBLE_HORZ,
    BOT_T_DOUBLE_VERT,
    BOT_T_DOUBLE_BOTH
};

// CROSS
enum cross_style {
    CROSS_THIN,
    CROSS_LEFT,
    CROSS_RIGHT,
    CROSS_MID,
    CROSS_TOP,
    CROSS_BOT,
    CROSS_VERT,
    CROSS_LEFT_TOP,
    CROSS_RIGHT_TOP,
    CROSS_LEFT_BOT,
    CROSS_RIGHT_BOT,
    CROSS_MID_TOP,
    CROSS_MID_BOT,
    CROSS_LEFT_VERT,
    CROSS_RIGHT_VERT,
    CROSS_BOLD,
    CROSS_DOUBLE_HORZ,
    CROSS_DOUBLE_VERT,
    CROSS_DOUBLE_BOTH
};

enum general_style {
    GENERAL_THIN_SOLID,
    GENERAL_BOLD_SOLID,
    GENERAL_THIN_DASHED,
    GENERAL_BOLD_DASHED,
    GENERAL_THIN_DOTTED,
    GENERAL_BOLD_DOTTED,
    GENERAL_DOUBLE
};

struct style_t {
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
typedef struct style_t style_t;

struct window_t {
    char* title;
    char* contents;
    size_t contents_ptr;
    dim_t* dim;
    char* colour;
    style_t* style;
    struct winsize current_terminal_sz;
    struct winsize original_terminal_sz;
    int z_index;
};
typedef struct window_t window_t;

struct node_t {
    window_t* window;
    struct node_t* next;
};
typedef struct node_t node_t;

node_t* new_node (window_t* window);

node_t* insert_at_tail (node_t* tail, node_t* new);

window_t* create_window (size_t top, size_t bot, size_t left,
                               size_t right);

void resize_window (window_t* window, size_t _top, size_t _bot,
                        size_t _left, size_t _right);

void set_z_index (window_t* window, size_t z_index);

void set_colour (window_t* window, const char* colour);

void set_title (window_t* window, const char* title);

void set_contents (window_t* window, char* contents);

void set_general_style (window_t* win, enum general_style);

void set_horizontal_style (window_t* win, char* horz_style);
void set_vert_style (window_t* win, char* vert_style);
void set_top_left_style (window_t* win, char* tl_style);
void set_top_right_style (window_t* win, char* tr_style);
void set_bottom_left_style (window_t* win, char*  bl_style);
void set_bottom_right_style (window_t* win, char* br_style);
void set_cross_style (window_t* win, char* cross_style);

int draw_windows (size_t num, ...);
int draw_all_windows();
void draw_window (window_t* window);
int draw_windows_z (size_t num, ...);
void twin_init();

extern window_t* flex_window_head;
extern node_t* flex_windows;
extern node_t* flex_windows_tail;
extern size_t window_count;

#endif
