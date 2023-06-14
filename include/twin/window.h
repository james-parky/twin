/******************************************************************************
* FILENAME :        twin/window.h                                             *
*                                                                             *
* DESCRIPTION :                                                               *
*       All user-accessible functions for drawing creating, styling, and      *
*       drawing windows using twin. Includes structs required for windows,    *
*       dimensions, and globals for the draw_all_windows() function.          *
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
#ifndef __TWIN_WINDOW_H__
#define __TWIN_WINDOW_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>

/******************************************************************************
* twin_dim_t:                                                                 *
*       A struct for holding the top, bottom, left, and right coordinates of  *
*       a twin window. All values are stored as unsigned short.               * 
******************************************************************************/
struct twin_dim_t {
    uint16_t top, bot, left, right;
};
typedef struct twin_dim_t twin_dim_t;

/******************************************************************************
* twin_general_style_t:                                                       *
*       An enum for representing general styles for twin window borders. For  *
*       example, passing TWIN_GENERAL_THIN_SOLID to twin_set_general_style()  *
*       will set all of the supplied window's border styles to the            *
*       corresponding thin, solid variant.                                    *
******************************************************************************/
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

/******************************************************************************
* twin_style_t:                                                               *
*      A struct for holding the styling of a twin window. Each possible       *
*      border component has its own styling. These are stored as char* and    *
*      represent the ASCII border strings defined with compiler preprocessor  *
*      directives in twin/colours.h.                                          *
******************************************************************************/
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

/******************************************************************************
* twin_window_t:                                                              *
*       A struct for representing a twin window. Each window contains members *
*       for its dimensions, styling, content buffer, and z-index. These are   *
*       all default-valued and changable through respective twin_set_xxx()    *
*       functions. The z-index represents the three-dimensional height of the *
*       window on the screen, much like how z-index is implemented in css.    *
******************************************************************************/
struct twin_window_t {
    char* title;
    char* contents;
    uint32_t contents_ptr;
    twin_dim_t* dim;
    char* colour;
    twin_style_t* style;
    struct winsize current_terminal_sz;
    struct winsize original_terminal_sz;
    uint16_t z_index;
};
typedef struct twin_window_t twin_window_t;

/******************************************************************************
* node_t:                                                                     *
*       A private struct for representing the complete set of active twin     *
*       windows using a linked list. The user should NOT be using _node_t     *
*       variables in their code.                                              *
******************************************************************************/
struct _node_t {
    twin_window_t* window;
    struct _node_t* next;
};
typedef struct _node_t _node_t;

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
                                   const uint16_t left, const uint16_t right);

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
                         const uint16_t right);

/******************************************************************************
* twin_set_z_index:                                                           *
*       Set the z-index of the supplied window to the supplied z-index. This  *
*       is used when drawing windows to decide which windows are drawn on top *
*       of another.                                                           *
*                                                                             *
* @param window: The window to be updated.                                    *
* @param z_index: The new z-index of the window.                              *
******************************************************************************/
void twin_set_z_index (twin_window_t* const window, const uint16_t z_index);

/******************************************************************************
* twin_set_colour:                                                            *
*       Set the colour of the supplied window to one of the sixteen colours   *
*       defined in twin/colours.h.                                            *
*                                                                             *
* @param window: The window to have its colour changed.                       *
* @param colour: The new colour for the supplied window.                      *
******************************************************************************/
void twin_set_colour (twin_window_t* const window, const char* colour);

/******************************************************************************
* twin_set_title:                                                             *
*       Set the title of the supplied window that will be drawn in the top    *
*       left of the window border on draw.                                    *
*                                                                             *
* @param window: The window to have its title changed.                        *
* @param title: The new title for the supplied window.                        *
******************************************************************************/
void twin_set_title (twin_window_t* const window, const char* title);

/******************************************************************************
* twin_set_contents:                                                          *
*       Set the contents of the supplied window to some string, provided that *
*       the length of the supplied string is not larger than the total        *
*       interior size of the supplied window.                                 *
*                                                                             *
* @param window: The window to have its content set.                          *
* @param contents: The new contents of the supplied window.                   *
******************************************************************************/
void twin_set_contents (twin_window_t* const window, const char* contents);

/******************************************************************************
* twin_set_general_style:                                                     *
*       Set all supplied window styling options based on the supplied general *
*       styling enum (above).                                                 *
*                                                                             *
* @param window: The window to have its style set.                            *
* @param general_style: The general styling to be applied to the window.      *
******************************************************************************/
void twin_set_general_style (twin_window_t* const window,
                             const twin_general_style_t general_style);

/******************************************************************************
* twin_set_horizontal_style:                                                  *
*       Set the supplied windows horizontal style.                            *
*                                                                             *
* @param window: The window to have its horizontal style changed.             *
* @param horz_style: The new horizontal style from twin/colours.h.            *
******************************************************************************/
void twin_set_horizontal_style (twin_window_t* const window,
                                const char* horz_style);

/******************************************************************************
* twin_set_vertical_style:                                                    *
*       Set the supplied windows veritcal style.                              *
*                                                                             *
* @param window: The window to have its vertical style changed.               *
* @param vert_style: The new vertical style from twin/colours.h.              *
******************************************************************************/
void twin_set_vertical_style (twin_window_t* const window,
                              const char* vert_style);

/******************************************************************************
* twin_set_top_left_style:                                                    *
*       Set the supplied windows top-left corner style.                       *
*                                                                             *
* @param window: The window to have its top-left corner style changed.        *
* @param tl_style: The new top-left corner style from twin/colours.h.         *
******************************************************************************/
void twin_set_top_left_style (twin_window_t* const window,
                              const char* tl_style);
/******************************************************************************
* twin_set_top_right_style_style:                                             *
*       Set the supplied windows top-right corner style.                      *
*                                                                             *
* @param window: The window to have its top-right corner style changed.       *
* @param tr_style: The new top-right corner style from twin/colours.h.        *
******************************************************************************/
void twin_set_top_right_style (twin_window_t* const window,
                               const char* tr_style);
/******************************************************************************
* twin_set_bottom_left_style:                                                 *
*       Set the supplied windows bottom-left corner style.                    *
*                                                                             *
* @param window: The window to have its bottom-left corner style changed.     *
* @param bl_style: The new bottom-left corner style from twin/colours.h.      *
******************************************************************************/
void twin_set_bottom_left_style (twin_window_t* const window,
                                 const char*  bl_style);
/******************************************************************************
* twin_set_bottom_right_style:                                                *
*       Set the supplied windows bottom-right corner style.                   *
*                                                                             *
* @param window: The window to have its bottom-right corner style changed.    *
* @param br_style: The new bottom-right corner style from twin/colours.h.     *
******************************************************************************/
void twin_set_bottom_right_style (twin_window_t* const window,
                                  const char* br_style);
/******************************************************************************
* twin_set_cross_style:                                                       *
*       Set the supplied windows cross style.                                 *
*                                                                             *
* @param window: The window to have its cross style changed.                  *
* @param cross_style: The new cross style from twin/colours.h.                *
******************************************************************************/
void twin_set_cross_style (twin_window_t* const window,
                           const char* cross_style);

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
bool twin_draw_windows (int num, ...);

/******************************************************************************
* twin_draw_all_windows:                                                      *
*       Draw all active windows in the global linked list of twin windows.    *
*                                                                             *
* @return bool: Whether the windows were drawn successfully or not.           *
******************************************************************************/
bool twin_draw_all_windows();

/******************************************************************************
* twin_draw_window:                                                           *
*       Draw a single twin window.                                            *
*                                                                             *
* @param window: The single window to be drawn.                               *
*                                                                             *
* @return bool: Whether the window was successfully drawn or not.             *
******************************************************************************/
bool twin_draw_window (const twin_window_t* const window);

/******************************************************************************
* twin_init:                                                                  *
*       Initialise the twin library by setting global linked list value, and  *
*       total window count.                                                   *
******************************************************************************/
void twin_init();

/******************************************************************************
* twin_free:                                                                  *
*       Free a twin window and all its contenst.                              *
******************************************************************************/
void twin_free(twin_window_t* window);

/******************************************************************************
* globals:                                                                    *
*       Globals to represent a linked list of all active windows, and the     *
*       current total window count. After a window is created it is added to  *
*       the linked list automatically and cannot be removed as of yet.        *
******************************************************************************/
extern twin_window_t* flex_window_head;
extern _node_t* flex_windows;
extern _node_t* flex_windows_tail;
extern uint8_t window_count;

#endif
