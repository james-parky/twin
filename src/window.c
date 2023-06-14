/******************************************************************************
* FILENAME :        window.c                                                  *
*                                                                             *
* DESCRIPTION :                                                               *
*       Implementation of all user-accessible functions for drawing creating, *
*       styling, and drawing windows using twin.                              *
*                                                                             *
* NOTES :                                                                     *
*       See https://github.com/james-parky/twin for more detail.              *
*                                                                             *
*       Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>  *
*       Everyone is permitted to copy and distribute verbatim copies of this  *
*       licensed document, but changing it is not allowed.                    *
*                                                                             *
* AUTHOR :    James David Parkington        START DATE :    04 May 23         *
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "twin/window.h"
#include "twin/errorf.h"
#include "twin/colours.h"

#define SET(style, attr, var) style->attr = malloc(strlen(var) + 1);\
                              strncpy(style->attr, var, strlen(var));\
                              style->attr[strlen(var) + 1] = '\0';


/******************************************************************************
* globals:                                                                    *
*       Globals to represent a linked list of all active windows, and the     *
*       current total window count. After a window is created it is added to  *
*       the linked list automatically and cannot be removed as of yet.        *
******************************************************************************/
twin_window_t* flex_window_head;
_node_t* flex_windows;
_node_t* flex_windows_tail;
uint8_t window_count;

/******************************************************************************
* _new_node:                                                                  *
*       A private function for creating a new linked list node containing a   *
*       twin window pointer. This function should NOT be used in user code.   *
*                                                                             *
* @param window: The window of the new linked list node.                      *
******************************************************************************/
_node_t* new_node (twin_window_t* const window) {
    _node_t* node = malloc(sizeof(_node_t));
    node->window = window;
    node->next = NULL;
    return node;
}

/******************************************************************************
* _insert_at_tail:                                                            *
*       A private function for inserting a linked list node at a the tail of  *
*       a linked list. This function should NOT be used in user code.         *
*                                                                             *
* @param tail: The node representing the tail of the linked list being used.  *
* @param new: The node the be added to the linked list to become the tail.    *
******************************************************************************/
_node_t* insert_at_tail (_node_t* tail, _node_t* const new) {
    tail->next = new;
    tail = new;
    return new;
}

/******************************************************************************
* twin_init:                                                                  *
*       Initialise the twin library by setting global linked list value, and  *
*       total window count.                                                   *
******************************************************************************/
void twin_init () {
    if (flex_window_head) twin_double_init_error(__FILE__, __LINE__);

    twin_dim_t* dim = malloc(sizeof(twin_dim_t));
    dim->top = 0;
    dim->bot = 0;
    dim->left = 0;
    dim->right = 0;

    flex_window_head = malloc(sizeof(twin_window_t));
    flex_window_head->dim = dim;
    flex_window_head->contents_ptr = 0;
    flex_window_head->z_index = 0;

    flex_windows = new_node(flex_window_head);
    flex_windows_tail = flex_windows;
    window_count = 1;
}

/******************************************************************************
* twin_init:                                                                  *
*       Creates and returns a new twin window with the given dimensions. Adds *
*       the new window to the global linked list of active windows.           *
*                                                                             *
* @param top: The y coordinate of the top of the window.                      *
* @param bot: The y coordinate of the bottom of the window.                   *
* @param left: The x coordinate of the left of the window.                    *
* @param right: The x coordinate of the right of the window.                  *
*                                                                             *
* @return twin_window_t*: A pointer to the created window.                    *
******************************************************************************/
twin_window_t* twin_create_window (const uint16_t top, const uint16_t bot,
                                   const uint16_t left, const uint16_t right) {
    twin_dim_t* dim = malloc(sizeof(twin_dim_t));
    dim->top = top;
    dim->bot = bot;
    dim->left = left;
    dim->right = right;

    twin_window_t* window = malloc(sizeof(twin_window_t));
    window->dim = dim;
    window->contents = NULL;
    window->contents_ptr = 0;
    window->z_index = window_count++;
    twin_set_general_style(window, TWIN_GENERAL_THIN_SOLID);
    ioctl(0, TIOCGWINSZ, &window->current_terminal_sz);
    ioctl(0, TIOCGWINSZ, &window->original_terminal_sz);

    _node_t* node = new_node(window);
    flex_windows_tail = insert_at_tail(flex_windows_tail, node);
    return window;
}

/******************************************************************************
* twin_resize_window:                                                         *
*       Resizes the supplied window to the supplied new dimensions.           *
*                                                                             *
* @param window: The window to be resized.                                    *
* @param top: The new top y coordinate.                                       *
* @param bot: The new bottom y coordinate.                                    *
* @param left: The new left x coordinate.                                     *
* @param right: The new right x coordinate.                                   *
******************************************************************************/
void twin_resize_window (twin_window_t* const window, const uint16_t top,
                         const uint16_t bot, const uint16_t left,
                         const uint16_t right) {
    window->dim->top = top;
    window->dim->bot = bot;
    window->dim->left = left;
    window->dim->right = right;
}

/******************************************************************************
* twin_set_colour:                                                            *
*       Set the colour of the supplied window to one of the sixteen colours   *
*       defined in twin/colours.h.                                            *
*                                                                             *
* @param window: The window to have its colour changed.                       *
* @param colour: The new colour for the supplied window.                      *
******************************************************************************/
void twin_set_colour (twin_window_t* const window, const char* colour) {
    window->colour = malloc(strlen(colour) + 1);
    strncpy(window->colour, colour, strlen(colour));
    window->colour[strlen(colour)] = '\0';
}

/******************************************************************************
* twin_set_title:                                                             *
*       Set the title of the supplied window that will be drawn in the top    *
*       left of the window border on draw.                                    *
*                                                                             *
* @param window: The window to have its title changed.                        *
* @param title: The new title for the supplied window.                        *
******************************************************************************/
void twin_set_title (twin_window_t* const window, const char* title) {
    window->title = malloc(strlen(title) + 1);
    strncpy(window->title, title, strlen(title));
    window->title[strlen(title)] = '\0';
}

/******************************************************************************
* twin_set_contents:                                                          *
*       Set the contents of the supplied window to some string, provided that *
*       the length of the supplied string is not larger than the total        *
*       interior size of the supplied window.                                 *
*                                                                             *
* @param window: The window to have its content set.                          *
* @param contents: The new contents of the supplied window.                   *
******************************************************************************/
void twin_set_contents(twin_window_t* const window, const char* contents) {
    uint32_t max_content_size = (window->dim->bot - window->dim->top)
        * (window->dim->right - window->dim->left);
    if (max_content_size < strlen(contents))
        twin_contents_error(__FILE__, __LINE__);

    window->contents = malloc(strlen(contents) + 1);
    strncpy(window->contents, contents, strlen(contents));
    window->contents[strlen(contents)] = '\0';
} 

/******************************************************************************
* twin_set_z_index:                                                           *
*       Set the z-index of the supplied window to the supplied z-index. This  *
*       is used when drawing windows to decide which windows are drawn on top *
*       of another.                                                           *
*                                                                             *
* @param window: The window to be updated.                                    *
* @param z_index: The new z-index of the window.                              *
******************************************************************************/
void twin_set_z_index (twin_window_t* const window, const uint16_t z_index) {
    if (z_index < 1) twin_z_index_error(__FILE__, __LINE__); 
    window->z_index = z_index;
}

/******************************************************************************
* twin_set_style:                                                             *
*       Set the style of the supplied window using all border style strings   *
*       supplied. Implements a preprocessor directive macro for setting the   *
*       style since it involves many repeated lines of string manipulations.  *
*                                                                             *
* @param window: The window to have its style set.                            *
* @param horz: The horziontal border style.                                   *
* @param vert: The vertical border style.                                     *
* @param tl: The top-left corner border style.                                *
* @param tr: The top-right corner border style.                               *
* @param bl: The bottom-left corner border style.                             *
* @param br: The bottom-right corner border style.                            *
* @param left_t: The left-sided t-junction border style.                      *
* @param right_t: The right-sided t-junction border style.                    *
* @param top_t: The top-sided t-junction border style.                        *
* @param bot_t: The bottom-sided t-junction border style.                     *
* @param cross: The cross-junction border style.                              *
******************************************************************************/
void set_style (twin_window_t* const window, const char* horz,
                const char* vert, const char* tl, const char* tr,
                const char* bl, const char* br, const char* left_t,
                const char* right_t, const char* top_t, const char* bot_t,
                const char* cross) {
    twin_style_t* style = malloc(sizeof(twin_style_t));
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
    window->style = style;
}

/******************************************************************************
* twin_set_general_style:                                                     *
*       Set all supplied window styling options based on the supplied general *
*       styling enum (above).                                                 *
*                                                                             *
* @param window: The window to have its style set.                            *
* @param general_style: The general styling to be applied to the window.      *
******************************************************************************/
void twin_set_general_style (twin_window_t* const window,
                             const twin_general_style_t general_style) {
    switch (general_style) {
        case TWIN_GENERAL_THIN_SOLID:
            set_style(window, TWIN_HORZ_THIN_SOLID, TWIN_VERT_THIN_SOLID,
                      TWIN_TL_THIN, TWIN_TR_THIN, TWIN_BL_THIN, TWIN_BR_THIN,
                      TWIN_LEFT_T_THIN, TWIN_RIGHT_T_THIN, TWIN_TOP_T_THIN,
                      TWIN_BOT_T_THIN, TWIN_CROSS_THIN);
            break;

        case TWIN_GENERAL_BOLD_SOLID:
            set_style(window, TWIN_HORZ_BOLD_SOLID, TWIN_VERT_BOLD_SOLID,
                      TWIN_TL_BOLD, TWIN_TR_BOLD, TWIN_BL_BOLD, TWIN_BR_BOLD,
                      TWIN_LEFT_T_BOLD, TWIN_RIGHT_T_BOLD, TWIN_TOP_T_BOLD,
                      TWIN_BOT_T_BOLD, TWIN_CROSS_BOLD);
            break;
        case TWIN_GENERAL_THIN_DASHED:
            set_style(window, TWIN_HORZ_THIN_DASHED, TWIN_VERT_THIN_DASHED,
                      TWIN_TL_BOLD, TWIN_TR_BOLD, TWIN_BL_BOLD, TWIN_BR_BOLD,
                      TWIN_LEFT_T_BOLD, TWIN_RIGHT_T_BOLD, TWIN_TOP_T_BOLD,
                      TWIN_BOT_T_BOLD, TWIN_CROSS_BOLD);
            break;

        case TWIN_GENERAL_BOLD_DASHED:
            set_style(window, TWIN_HORZ_BOLD_DASHED, TWIN_VERT_BOLD_DASHED,
                      TWIN_TL_BOLD, TWIN_TR_BOLD, TWIN_BL_BOLD, TWIN_BR_BOLD,
                      TWIN_LEFT_T_BOLD, TWIN_RIGHT_T_BOLD, TWIN_TOP_T_BOLD,
                      TWIN_BOT_T_BOLD, TWIN_CROSS_BOLD);
            break;
        case TWIN_GENERAL_THIN_DOTTED:
            set_style(window, TWIN_HORZ_THIN_DOTTED, TWIN_VERT_THIN_DOTTED,
                      TWIN_TL_BOLD, TWIN_TR_BOLD, TWIN_BL_BOLD, TWIN_BR_BOLD,
                      TWIN_LEFT_T_BOLD, TWIN_RIGHT_T_BOLD, TWIN_TOP_T_BOLD,
                      TWIN_BOT_T_BOLD, TWIN_CROSS_BOLD);
            break;

        case TWIN_GENERAL_BOLD_DOTTED:
            set_style(window, TWIN_HORZ_BOLD_DOTTED, TWIN_VERT_BOLD_DOTTED,
                      TWIN_TL_BOLD, TWIN_TR_BOLD, TWIN_BL_BOLD, TWIN_BR_BOLD,
                      TWIN_LEFT_T_BOLD, TWIN_RIGHT_T_BOLD, TWIN_TOP_T_BOLD,
                      TWIN_BOT_T_BOLD, TWIN_CROSS_BOLD);
            break;
        case TWIN_GENERAL_DOUBLE:
            set_style(window, TWIN_HORZ_DOUBLE, TWIN_VERT_DOUBLE,
                      TWIN_TL_DOUBLE, TWIN_TR_DOUBLE, TWIN_BL_DOUBLE,
                      TWIN_BR_DOUBLE, TWIN_LEFT_T_DOUBLE_BOTH,
                      TWIN_RIGHT_T_DOUBLE_BOTH, TWIN_TOP_T_DOUBLE_BOTH,
                      TWIN_BOT_T_DOUBLE_BOTH, TWIN_CROSS_DOUBLE_BOTH);
            break;
    }
}

/******************************************************************************
* twin_set_horizontal_style:                                                  *
*       Set the supplied windows horizontal style.                            *
*                                                                             *
* @param window: The window to have its horizontal style changed.             *
* @param horz_style: The new horizontal style from twin/colours.h.            *
******************************************************************************/
void twin_set_horizontal_style (twin_window_t* const win, const char* horz) {
    SET(win->style, horz, horz);
}

/******************************************************************************
* twin_set_vertical_style:                                                    *
*       Set the supplied windows veritcal style.                              *
*                                                                             *
* @param window: The window to have its vertical style changed.               *
* @param vert_style: The new vertical style from twin/colours.h.              *
******************************************************************************/
void twin_set_vert_style (twin_window_t* const win, const char* vert) {
    SET(win->style, vert, vert);
}

/******************************************************************************
* twin_set_top_left_style:                                                    *
*       Set the supplied windows top-left corner style.                       *
*                                                                             *
* @param window: The window to have its top-left corner style changed.        *
* @param tl_style: The new top-left corner style from twin/colours.h.         *
******************************************************************************/
void twin_set_top_left_style (twin_window_t* const win, const char* tl) {
    SET(win->style, tl, tl);
}

/******************************************************************************
* twin_set_top_right_style_style:                                             *
*       Set the supplied windows top-right corner style.                      *
*                                                                             *
* @param window: The window to have its top-right corner style changed.       *
* @param tr_style: The new top-right corner style from twin/colours.h.        *
******************************************************************************/
void twin_set_top_right_style (twin_window_t* const win, const char* tr) {
    SET(win->style, tr, tr);
}

/******************************************************************************
* twin_set_bottom_left_style:                                                 *
*       Set the supplied windows bottom-left corner style.                    *
*                                                                             *
* @param window: The window to have its bottom-left corner style changed.     *
* @param bl_style: The new bottom-left corner style from twin/colours.h.      *
******************************************************************************/
void twin_set_bottom_left_style (twin_window_t* const win, const char* bl) {
    SET(win->style, bl, bl);
}

/******************************************************************************
* twin_set_bottom_left_style:                                                 *
*       Set the supplied windows bottom-left corner style.                    *
*                                                                             *
* @param window: The window to have its bottom-left corner style changed.     *
* @param bl_style: The new bottom-left corner style from twin/colours.h.      *
******************************************************************************/
void twin_set_bottom_right_style (twin_window_t* const win, const char* br) {
    SET(win->style, br, br);
}

/******************************************************************************
* twin_set_cross_style:                                                       *
*       Set the supplied windows cross style.                                 *
*                                                                             *
* @param window: The window to have its cross style changed.                  *
* @param cross_style: The new cross style from twin/colours.h.                *
******************************************************************************/
void twin_set_cross_style (twin_window_t* const win, const char* cross) {
    SET(win->style, cross, cross);
}

/******************************************************************************
* print_border_section:                                                       *
*       Print the supplied border section in the supplied colour. Resets the  *
*       colour afterwards.                                                    *
*                                                                             *
* @param colour: The colour to print the supplied border section.             *
* @param section: The section to be printed to the screen.                    *
******************************************************************************/
void print_border_section (const char* colour, const char* section) {
    printf("%s%s%s", colour ? colour : (const char*) TWIN_RESET_COLOUR,
           section, TWIN_RESET_COLOUR);
}

/******************************************************************************
* set_content:                                                                *
*       Copy the supplied content to the current terminal cell's content      *
*       buffer, including the coloured text ANSI control code for the window  *
*       to which the content belongs. Rests the colour afterwards.            *
*                                                                             *
* @param cur_content: A pointer to the terminal cells content buffer.         *
* @param window: The window to which the supplied content belongs.            *
* @param content: The content that will be written to the terminal cell.      *
******************************************************************************/
void set_content (char** cur_content, const twin_window_t* const window,
                       const char* const content) {
    *cur_content = malloc(strlen(window->colour) + strlen(content)
                          + strlen(TWIN_RESET_COLOUR) + 1);
    strncpy(*cur_content, window->colour, strlen(window->colour));
    strncpy(*cur_content + strlen(window->colour), content, strlen(content));
    strncpy(*cur_content + strlen(window->colour) + strlen(content),
            TWIN_RESET_COLOUR, strlen(TWIN_RESET_COLOUR));
    (*cur_content)[strlen(window->colour) + strlen(content)
                   + strlen(TWIN_RESET_COLOUR)] = '\0';
}

/******************************************************************************
* reset_content_ptrs:                                                         *
*       Reset the contents buffer pointers for all windows that supplied      *
*       variadically to twin_draw_windows().                                  *
*                                                                             *
* @param num: The number of windows that were supplied to twin_draw_windows().*
* @param windows: The variadic list containing the supplied windows.          *
******************************************************************************/
void reset_content_ptrs (int num, va_list windows) {
    twin_window_t* window;
    for (uint16_t i = 0; i < num; i++) {
        window = va_arg(windows, twin_window_t*);
        window->contents_ptr = 0;
    }
}

/******************************************************************************
* twin_draw_windows:                                                          *
*       A variadic function for drawing a set number of supplied twin         *
*       windows.                                                              *
*                                                                             *
* @param num: The number of twin windows being supplied.                      *
* @param ...: A variadic list of twin windows of type twin_window_t*.         *
*                                                                             *
* @return bool: Whether the windows were drawn successfully.                  *
******************************************************************************/
bool twin_draw_windows (int num, ...) {
    va_list windows;
    va_start(windows, num);
    reset_content_ptrs(num, windows);
    va_end(windows);
    va_start(windows, num);

    struct winsize current_terminal_sz;
    ioctl(0, TIOCGWINSZ, &current_terminal_sz);

    for (uint16_t row = 0; row <= current_terminal_sz.ws_row; row++) {
        for (uint16_t col = 0; col < current_terminal_sz.ws_col; col++) {
            uint16_t max_z = 0;
            char* cur_content = NULL;
            bool printed_title = false;
            va_start(windows, num);

            for (uint8_t win = 0; win < num; win++) {
                twin_window_t* window = va_arg(windows, twin_window_t*);
                twin_dim_t* dim = window->dim;

                bool inside = (row >= dim->top) && (row <= dim->bot)
                    && (col >= dim->left) && (col <= dim->right);
                bool ch = inside && window->contents &&
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
                            printed_title = true;
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
    return true;
}

/******************************************************************************
* twin_draw_all_windows:                                                      *
*       Draw all active windows in the global linked list of twin windows.    *
*                                                                             *
* @return bool: Whether the windows were drawn successfully or not.           *
******************************************************************************/
bool twin_draw_all_windows () {
    struct winsize current_terminal_sz;
    ioctl(0, TIOCGWINSZ, &current_terminal_sz);
    _node_t* temp = flex_windows->next;
    while (temp) {
        temp->window->contents_ptr = 0;
        temp = temp->next;
    }
    for (uint16_t row = 0; row <= current_terminal_sz.ws_row; row++) {
        for (uint16_t col = 0; col < current_terminal_sz.ws_col; col++) {
            uint16_t max_z = 0;
            char* cur_content = NULL;
            bool printed_title = false;
            _node_t* temp = flex_windows->next;

            while (temp) {
                twin_window_t* window = temp->window; 
                twin_dim_t* dim = window->dim;
                bool inside = (row >= dim->top) && (row <= dim->bot)
                    && (col >= dim->left) && (col <= dim->right);
                bool ch = inside && window->contents &&
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
                            printed_title = true;
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
    return true;
}

/******************************************************************************
* twin_draw_window:                                                           *
*       Draw a single twin window.                                            *
*                                                                             *
* @param window: The single window to be drawn.                               *
*                                                                             *
* @return bool: Whether the window was successfully drawn or not.             *
******************************************************************************/
bool twin_draw_window (const twin_window_t* const window) {
    return twin_draw_windows(1, window);
}
