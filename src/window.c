#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "window.h"
#include "errorf.h"
#include "colours.h"

window_t* create_window (size_t top, size_t bot, size_t left,
                               size_t right) {
    dim_t* dim = malloc(sizeof(dim_t));
    dim->top = top;
    dim->bot = bot;
    dim->left = left;
    dim->right = right;

    window_t* window = malloc(sizeof(window_t));
    window->dim = dim;
    window->contents_ptr = 0;

    ioctl(0, TIOCGWINSZ, &window->current_terminal_sz);
    ioctl(0, TIOCGWINSZ, &window->original_terminal_sz);
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
    int max_content_size = (window->dim->bot - window->dim->top)
        * (window->dim->right - window->dim->left);
    if (max_content_size < (size_t) strlen(contents)) contents_error();
    window->contents = malloc(strlen(contents) + 1);
    window->contents[strlen(contents)] = '\0';
    strncpy(window->contents, contents, strlen(contents));
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

int draw_windows (size_t num, ...) {
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);
    va_list windows;
    size_t count = 0;
    va_start(windows, num);
    window_t* original_window;
    for (size_t i = 0; i < num; i++) {
        original_window = va_arg(windows, window_t*);
        count++;
    }
    if (count != num) {
        printf("bad args");
        exit(EXIT_FAILURE);
    }
    va_end(windows);
    va_start(windows, num);
    if (windows_overlap(num, windows)) return 0;
    struct winsize current_terminal_sz = original_window->current_terminal_sz; 
    for (size_t row = 0; row <= current_terminal_sz.ws_row; row++) {
        for (size_t col = 0; col < current_terminal_sz.ws_col; col++) {
            va_start(windows, num);
            int is_vert = 0, is_tl = 0, is_tr = 0, is_bl = 0;
            int is_br = 0, is_hor = 0, is_content = 0;

            for (size_t i = 0; i < num; i++){
                window_t* window = va_arg(windows, window_t*);
                dim_t* dim = window->dim;
                int tl = (row == dim->top && col == dim->left);
                int tr = (row == dim->top && col == dim->right);
                int bl = (row == dim->bot && col == dim->left);
                int br = (row == dim->bot && col == dim->right);
                int vert = ((row > dim->top && row < dim->bot)
                    && (col == dim->left || col == dim->right));
                int hor = ((row == dim->top || row == dim->bot)
                    && (col < dim->right && col > dim->left));
                int content = (row > window->dim->top)
                    && (row < window->dim->bot) && (col > window->dim->left)
                    && (col < window->dim->right) && (window->contents)
                    && (window->contents_ptr < strlen(window->contents));

                if (vert) print_border_section(window->colour, VERTICAL);
                else if (tl) {
                    print_border_section(window->colour, TOP_LEFT_CORNER);
                    if (window->title) {
                        print_border_section(window->colour, window->title);
                        col+= strlen(window->title);
                    }
                }
                else if (bl) print_border_section(window->colour,
                                                  BOTTOM_LEFT_CORNER);
                else if (br) print_border_section(window->colour,
                                                  BOTTOM_RIGHT_CORNER);
                else if (tr) print_border_section(window->colour,
                                                  TOP_RIGHT_CORNER);
                else if (hor) print_border_section(window->colour,
                                                   HORIZONTAL);
                else if (content)
                    printf("%c", window->contents[window->contents_ptr++]);

                is_vert |= vert;
                is_tl |= tl;
                is_tr |= tr;
                is_br |= br;
                is_bl |= bl;
                is_hor |= hor;
                is_content |= content;
            }
            va_end(windows);
            if (!(is_vert | is_tl | is_tr | is_bl | is_br | is_hor
                | is_content)) printf(" ");
        }
    }
    fflush(stdout);
    return 1;
}


void draw_window (window_t* window) {
    //size_t original_col = window->current_terminal_sz.ws_col;
    //window->current_terminal_sz.ws_col +=(window->current_terminal_sz.ws_col & 1);
    if (window->colour) printf("%s", window->colour);
    for (size_t row = 0; row <= window->current_terminal_sz.ws_row; row++) {
        if (row < window->dim->top) {
            for (size_t col = 0; col < window->current_terminal_sz.ws_col; col++)
                printf(" ");
        }
        else if (row == window->dim->top) {
            for (size_t col = 0; col <= window->current_terminal_sz.ws_col; col++) {
                if (col < window->dim->left) printf(" ");
                else if (col == window->dim->left) {
                    printf("%s", TOP_LEFT_CORNER);
                    printf("%s", window->title);
                    col += strlen(window->title);
                }
                else if (col > window->dim->left && col < window->dim->right - 1) printf("%s", HORIZONTAL);
                else if (col == window->dim->right) printf("%s", TOP_RIGHT_CORNER);
                else if (col > window->dim->right) printf(" ");
            }
        }
        else if (row > window->dim->top && row < window->dim->bot) {
            for (size_t col = 0; col <= window->current_terminal_sz.ws_col; col++) {
                if (col < window->dim->left) printf(" ");
                else if (col == window->dim->left) printf("%s", VERTICAL);
                else if (col > window->dim->left && col < window->dim->right - 1) {
                   printf(" "); 
                }
                else if (col == window->dim->right) printf("%s", VERTICAL);
                else if (col > window->dim->right) printf(" ");
            }
        }
        else if (row == window->dim->bot) {
            for (size_t col = 0; col <= window->current_terminal_sz.ws_col ; col++) {
                if (col < window->dim->left) printf(" ");
                else if (col == window->dim->left) printf("%s", BOTTOM_LEFT_CORNER);
                else if (col > window->dim->left && col < window->dim->right - 1) printf("%s", HORIZONTAL);
                else if (col == window->dim->right) printf("%s", BOTTOM_RIGHT_CORNER);
                else if (col > window->dim->right) printf(" ");
            }
        }
        else if (row > window->dim->bot) printf("\n");
/*        
else if (row > window->dim->bot) {
            for (size_t col = 0; col < window->current_terminal_sz.ws_col-1;col++) printf(" ");
        }*/
     }
    printf("%s", RESET_COLOUR);
    fflush(stdout);

    //window->current_terminal_sz.ws_col = original_col;
}



