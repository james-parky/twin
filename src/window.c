#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "window.h"
#include "errorf.h"
#include "colours.h"


#define SET(style, attr, var) style->attr = malloc(strlen(var) + 1);\
                              strncpy(style->attr, var, strlen(var));\
                              style->attr[strlen(var) + 1] = '\0';

window_t* flex_window_head;
node_t* flex_windows;
node_t* flex_windows_tail;
size_t window_count;


node_t* new_node (window_t* window) {
    node_t* node = malloc(sizeof(node_t));
    node->window = window;
    node->next = NULL;
    return node;
}

node_t* insert_at_tail (node_t* tail, node_t* new) {
    tail->next = new;
    tail = new;
    return new;
}

void twin_init () {
    dim_t* dim = malloc(sizeof(dim_t));
    dim->top=0;
    dim->bot=0;
    dim->left=0;
    dim->right=0;
    flex_window_head = malloc(sizeof(window_t));
    flex_window_head->dim=dim;
    flex_window_head->contents_ptr=0;
    flex_window_head->z_index = -1;


        //create_window(0,0,0,0);
    flex_windows = new_node(flex_window_head);
    flex_windows_tail = flex_windows;
    window_count = 0;
}

window_t* create_window (size_t top, size_t bot, size_t left,
                               size_t right) {
    dim_t* dim = malloc(sizeof(dim_t));
    dim->top = top;
    dim->bot = bot;
    dim->left = left;
    dim->right = right;

    window_t* window = malloc(sizeof(window_t));
    window->dim = dim;
    window->contents = NULL;
    window->contents_ptr = 0;
    window->z_index = window_count++;
    set_general_style(window, GENERAL_THIN_SOLID);

    ioctl(0, TIOCGWINSZ, &window->current_terminal_sz);
    ioctl(0, TIOCGWINSZ, &window->original_terminal_sz);
    
    node_t* node = new_node(window);
    flex_windows_tail = insert_at_tail(flex_windows_tail, node);
    return window;
}

void resize_window (window_t* window, size_t _top, size_t _bot,
                        size_t _left, size_t _right) {
    window->dim->top = _top;
    window->dim->bot = _bot;
    window->dim->left = _left;
    window->dim->right = _right;
}


void set_colour (window_t* window, const char* colour) {
    window->colour = malloc(strlen(colour) + 1);
    strncpy(window->colour, colour, strlen(colour));
    window->colour[strlen(colour)] = '\0';
}

void set_title (window_t* window, const char* title) {
    window->title = malloc(strlen(title) + 1);
    strncpy(window->title, title, strlen(title));
    window->title[strlen(title)] = '\0';
}

void set_contents(window_t* window, char* contents) {
    size_t max_content_size = (window->dim->bot - window->dim->top)
        * (window->dim->right - window->dim->left);
    if (max_content_size < strlen(contents))
        contents_error(__FILE__, __LINE__);
    window->contents = malloc(strlen(contents) + 1);
    strncpy(window->contents, contents, strlen(contents));
    window->contents[strlen(contents)] = '\0';
} 

void set_z_index (window_t* window, size_t z_index) {
    if ((int) z_index < 0) z_index_error(__FILE__, __LINE__); 
    window->z_index = (int) z_index;
}

void _set_style (window_t* win, char* horz, char*vert, char* tl, char* tr,
                 char* bl, char* br, char* left_t, char* right_t, char* top_t,
                 char* bot_t, char* cross) {
    style_t* style = malloc(sizeof(style_t));
    SET(style, horz, horz);
    SET(style, vert, vert);
    SET(style, tl, tl);
    SET(style, tr, tr);
    SET(style, bl, bl);
    SET(style, br, br);
    SET(style, left_t, left_t);
    SET(style, right_t, right_t);
    SET(style, top_t, top_t);
    SET(style, bot_t, bot_t);
    SET(style, cross, cross);
    win->style = style;
}

void set_general_style (window_t* win, enum general_style general_style) {
    switch (general_style) {
        case GENERAL_THIN_SOLID:
            _set_style(win, HORZ_THIN_SOLID, VERT_THIN_SOLID, TL_THIN, TR_THIN,
                       BL_THIN, BR_THIN, LEFT_T_THIN, RIGHT_T_THIN, TOP_T_THIN,
                       BOT_T_THIN, CROSS_THIN);
            break;

        case GENERAL_BOLD_SOLID:
            _set_style(win, HORZ_BOLD_SOLID, VERT_BOLD_SOLID, TL_BOLD, TR_BOLD,
                       BL_BOLD, BR_BOLD, LEFT_T_BOLD, RIGHT_T_BOLD, TOP_T_BOLD,
                       BOT_T_BOLD, CROSS_BOLD);
            break;
        case GENERAL_THIN_DASHED:
            _set_style(win, HORZ_THIN_DASHED, VERT_THIN_DASHED, TL_BOLD,
                       TR_BOLD, BL_BOLD, BR_BOLD, LEFT_T_BOLD, RIGHT_T_BOLD,
                       TOP_T_BOLD, BOT_T_BOLD, CROSS_BOLD);
            break;

        case GENERAL_BOLD_DASHED:
            _set_style(win, HORZ_BOLD_DASHED, VERT_BOLD_DASHED, TL_BOLD,
                       TR_BOLD, BL_BOLD, BR_BOLD, LEFT_T_BOLD, RIGHT_T_BOLD,
                       TOP_T_BOLD, BOT_T_BOLD, CROSS_BOLD);
            break;
        case GENERAL_THIN_DOTTED:
            _set_style(win, HORZ_THIN_DOTTED, VERT_THIN_DOTTED, TL_BOLD,
                       TR_BOLD, BL_BOLD, BR_BOLD, LEFT_T_BOLD, RIGHT_T_BOLD,
                       TOP_T_BOLD, BOT_T_BOLD, CROSS_BOLD);
            break;

        case GENERAL_BOLD_DOTTED:
            _set_style(win, HORZ_BOLD_DOTTED, VERT_BOLD_DOTTED, TL_BOLD,
                       TR_BOLD, BL_BOLD, BR_BOLD, LEFT_T_BOLD, RIGHT_T_BOLD,
                       TOP_T_BOLD, BOT_T_BOLD, CROSS_BOLD);
            break;
        case GENERAL_DOUBLE:
            _set_style(win, HORZ_DOUBLE, VERT_DOUBLE, TL_DOUBLE, TR_DOUBLE,
                       BL_DOUBLE, BR_DOUBLE, LEFT_T_DOUBLE_BOTH,
                       RIGHT_T_DOUBLE_BOTH, TOP_T_DOUBLE_BOTH,
                       BOT_T_DOUBLE_BOTH, CROSS_DOUBLE_BOTH);
            break;
    }
}

void set_horizontal_style (window_t* win, char* horz) {
    SET(win->style, horz, horz);
}

void set_vert_style (window_t* win, char* vert) {
    SET(win->style, vert, vert);
}

void set_top_left_style (window_t* win, char* tl) {
    SET(win->style, tl, tl);
}

void set_top_right_style (window_t* win, char* tr) {
    SET(win->style, tr, tr);
}

void set_bottom_left_style (window_t* win, char* bl) {
    SET(win->style, bl, bl);
}

void set_bottom_right_style (window_t* win, char* br) {
    SET(win->style, br, br);
}

void set_cross_style (window_t* win, char* cross) {
    SET(win->style, cross, cross);
}

void print_border_section (char* colour, char* section) {
    printf("%s%s%s", colour ? colour : (const char*) RESET_COLOUR, section,
           RESET_COLOUR);
}

int overlap (window_t* prev_win, window_t* current_win) {
    
    int left_inbetween = current_win->dim->left >= prev_win->dim->left
        && current_win->dim->left <= prev_win->dim->right;
    int right_inbetween = current_win->dim->right >= prev_win->dim->left
        && current_win->dim->right <= prev_win->dim->right;
    int top_between = current_win->dim->top >= prev_win->dim->top
        && current_win->dim->top <= prev_win->dim->bot;
    int bot_between = current_win->dim->bot >= prev_win->dim->top
        && current_win->dim->bot <= prev_win->dim->bot;

    int tl_between = left_inbetween & top_between;
    int tr_between = right_inbetween & top_between;
    int bl_between = left_inbetween & bot_between;
    int br_between = right_inbetween & bot_between;

    return tl_between | tr_between | bl_between | br_between;
}

int windows_overlap (size_t num, va_list windows) {
    window_t* prev_win = va_arg(windows, window_t*);
    for (size_t i = 1; i < num; i++) {
        window_t* current_win = va_arg(windows, window_t*);
        if (overlap(prev_win, current_win)) return 1;
    }
    return 0;
}


void set_content (char** cur_content, window_t* window, char* content) {
    *cur_content = malloc(strlen(window->colour) + strlen(content)
                          + strlen(RESET_COLOUR) + 1);
    strncpy(*cur_content, window->colour, strlen(window->colour));
    strncpy(*cur_content + strlen(window->colour), content, strlen(content));
    strncpy(*cur_content + strlen(window->colour) + strlen(content),
            RESET_COLOUR, strlen(RESET_COLOUR));
    (*cur_content)[strlen(window->colour) + strlen(content)
                   + strlen(RESET_COLOUR)] = '\0';
}

void reset_content_ptrs (size_t num, va_list windows) {
    window_t* win;
    for (size_t i = 0; i < num; i++) {
        win = va_arg(windows, window_t*);
        win->contents_ptr = 0;
    }
}

int draw_windows (size_t num, ...) {
    va_list windows;
    va_start(windows, num);
    reset_content_ptrs(num, windows);
    va_end(windows);
    va_start(windows, num);

    struct winsize current_terminal_sz;
    ioctl(0, TIOCGWINSZ, &current_terminal_sz);

    for (size_t row = 0; row <= current_terminal_sz.ws_row; row++) {
        for (size_t col = 0; col < current_terminal_sz.ws_col; col++) {
            int max_z = -1;
            char* cur_content = NULL;
            int printed_title = 0;
            va_start(windows, num);
            for (size_t win = 0; win < num; win++) {
                window_t* window = va_arg(windows, window_t*);
                dim_t* dim = window->dim;
                int inside = (row >= dim->top) && (row <= dim->bot)
                    && (col >= dim->left) && (col <= dim->right);
                int ch = inside && window->contents &&
                    (window->contents_ptr < strlen(window->contents));

                if (inside && (window->z_index > max_z)) {
                    max_z = window->z_index;
                    if ((row > dim->top && row < dim->bot)
                        && (col == dim->left || col == dim->right)) {
                        set_content(&cur_content, window, window->style->vert);   
                    } else if ((row == dim->top || row == dim->bot)
                    && (col < dim->right && col > dim->left)) {
                        set_content(&cur_content, window, window->style->horz);    
                    }
                    else if (row == dim->top && col == dim->left) {
                        print_border_section(window->colour,
                                             window->style->tl);
                        if (window->title) {
                            print_border_section(window->colour,
                                                 window->title);
                            col += strlen(window->title);
                            printed_title = 1;
                        }
                    }
                    else if (row == dim->top && col == dim->right)
                        set_content(&cur_content, window, window->style->tr);
                    else if (row == dim->bot && col == dim->left)
                        set_content(&cur_content, window, window->style->bl);
                    else if (row == dim->bot && col == dim->right)
                        set_content(&cur_content, window, window->style->br);
                    else if (inside) {
                        cur_content = malloc(2);
                        cur_content[0] =
                            ch ? window->contents[window->contents_ptr++]
                               : ' ';
                        cur_content[1] = '\0';
                    }
                }
            }
            if (!printed_title) printf("%s", cur_content ? cur_content : " ");
        }
    }
    fflush(stdout);
    return 1;
}


int draw_all_windows () {
    struct winsize current_terminal_sz;
    ioctl(0, TIOCGWINSZ, &current_terminal_sz);
    node_t* temp = flex_windows->next;
    while (temp) {
        temp->window->contents_ptr = 0;
        temp = temp->next;
    }
    for (size_t row = 0; row <= current_terminal_sz.ws_row; row++) {
        for (size_t col = 0; col < current_terminal_sz.ws_col; col++) {
            int max_z = -1;
            char* cur_content = NULL;
            int printed_title = 0;
            node_t* temp = flex_windows->next;
            while (temp) {
                window_t* window = temp->window; 
                dim_t* dim = window->dim;
                int inside = (row >= dim->top) && (row <= dim->bot)
                    && (col >= dim->left) && (col <= dim->right);
                int ch = inside && window->contents &&
                    (window->contents_ptr < strlen(window->contents));

                if (inside && (window->z_index > max_z)) {
                    max_z = window->z_index;
                    if ((row > dim->top && row < dim->bot)
                        && (col == dim->left || col == dim->right)) {
                        set_content(&cur_content, window, window->style->vert);   
                    } else if ((row == dim->top || row == dim->bot)
                    && (col < dim->right && col > dim->left)) {
                        set_content(&cur_content, window, window->style->horz);    
                    }
                    else if (row == dim->top && col == dim->left) {
                        print_border_section(window->colour,
                                             window->style->tl);
                        if (window->title) {
                            print_border_section(window->colour,
                                                 window->title);
                            col += strlen(window->title);
                            printed_title = 1;
                        }
                    }
                    else if (row == dim->top && col == dim->right)
                        set_content(&cur_content, window, window->style->tr);
                    else if (row == dim->bot && col == dim->left)
                        set_content(&cur_content, window, window->style->bl);
                    else if (row == dim->bot && col == dim->right)
                        set_content(&cur_content, window, window->style->br);
                    else if (inside) {
                        cur_content = malloc(2);
                        cur_content[0] =
                            ch ? window->contents[window->contents_ptr++]
                               : ' ';
                        cur_content[1] = '\0';
                    }
                }
                temp = temp->next;
            }
            if (!printed_title) printf("%s", cur_content ? cur_content : " ");
        }
    }
    fflush(stdout);
    return 1;
}


void draw_window (window_t* window) { draw_windows(1, window); }
