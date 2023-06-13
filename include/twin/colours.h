/******************************************************************************
* FILENAME :        twin/colours.h                                            *
*                                                                             *
* DESCRIPTION :                                                               *
*       ASCII border character and ANSI terminal colour code preprocessor     *
*       directives required for the twin library.                             *
*                                                                             *
* NOTES :                                                                     *
*       These preprocessor directives are part of the twin library;           *
*       See https://github.com/james-parky/twin for more detail.              *
*                                                                             *
*       Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>  *
*       Everyone is permitted to copy and distribute verbatim copies of this  *
*       licensed document, but changing it is not allowed.                    *
*                                                                             *
* AUTHOR :    James David Parkington        START DATE :    04 May 23         *
******************************************************************************/

#ifndef __TWIN_COLOURS_H__
#define __TWIN_COLOURS_H__

/******************************************************************************
* TWIN_HORZ_XXX:                                                              *
*       The compiler preprocessor directives for thin and bold, solid,        *
*       dashed, dotted, and double stroke horizontal border characters.       *                                                    *
******************************************************************************/
#define TWIN_HORZ_THIN_SOLID "─"
#define TWIN_HORZ_BOLD_SOLID "━"
#define TWIN_HORZ_THIN_DASHED "┄"
#define TWIN_HORZ_BOLD_DASHED "┅"
#define TWIN_HORZ_THIN_DOTTED "┈"
#define TWIN_HORZ_BOLD_DOTTED "┉"
#define TWIN_HORZ_DOUBLE "═"

/******************************************************************************
* TWIN_VERT_XXX:                                                              *
*       The compiler preprocessor directives for thin and bold, solid,        *
*       dashed, dotted, and double stroke vertical border characters.         *                                         *
******************************************************************************/
#define TWIN_VERT_THIN_SOLID "│"
#define TWIN_VERT_BOLD_SOLID "┃"
#define TWIN_VERT_THIN_DASHED "┆"
#define TWIN_VERT_BOLD_DASHED "┇"
#define TWIN_VERT_THIN_DOTTED "┊"
#define TWIN_VERT_BOLD_DOTTED "┋"
#define TWIN_VERT_DOUBLE "║"	 

/******************************************************************************
* TWIN_TL_XXX:                                                                *
*       The compiler preprocessor directives for thin, heavy, double, and     *
*       curved top-left corner border characters.                             *
******************************************************************************/
#define TWIN_TL_THIN "┌"
#define TWIN_TL_TOP_HEAVY "┍"
#define TWIN_TL_SIDE_HEAVY "┎"
#define TWIN_TL_BOLD "┏"
#define TWIN_TL_TOP_DOUBLE "╒"
#define TWIN_TL_SIDE_DOUBLE "╓"
#define TWIN_TL_DOUBLE "╔"
#define TWIN_TL_CURVED "╭"

/******************************************************************************
* TWIN_TR_XXX:                                                                *
*       The compiler preprocessor directives for thin, heavy, double, and     *
*       curved top-right corner border characters.                            *
******************************************************************************/
#define TWIN_TR_THIN "┐"
#define TWIN_TR_TOP_HEAVY "┑"
#define TWIN_TR_SIDE_HEAVY "┒"
#define TWIN_TR_BOLD "┓"
#define TWIN_TR_TOP_DOUBLE "╕"
#define TWIN_TR_SIDE_DOUBLE "╖"
#define TWIN_TR_DOUBLE "╗"
#define TWIN_TR_CURVED "╮"

/******************************************************************************
* TWIN_BL_XXX:                                                                *
*       The compiler preprocessor directives for thin, heavy, double, and     *
*       curved bottom-left corner border characters.                          *
******************************************************************************/
#define TWIN_BL_THIN "└"
#define TWIN_BL_TOP_HEAVY "┕"
#define TWIN_BL_SIDE_HEAVY "┖"
#define TWIN_BL_BOLD "┗"
#define TWIN_BL_TOP_DOUBLE "╘"
#define TWIN_BL_SIDE_DOUBLE "╙"
#define TWIN_BL_DOUBLE "╚"
#define TWIN_BL_CURVED "╰"

/******************************************************************************
* TWIN_BR_XXX:                                                                *
*       The compiler preprocessor directives for thin, heavy, double, and     *
*       curved bottom-right corner border characters.                         *
******************************************************************************/
#define TWIN_BR_THIN "┘"
#define TWIN_BR_TOP_HEAVY "┙"
#define TWIN_BR_SIDE_HEAVY "┚"
#define TWIN_BR_BOLD "┛"
#define TWIN_BR_TOP_DOUBLE "╛"
#define TWIN_BR_SIDE_DOUBLE "╜"
#define TWIN_BR_DOUBLE "╝"
#define TWIN_BR_CURVED "╯"

/******************************************************************************
* TWIN_LEFT_T_XXX:                                                            *
*       The compiler preprocessor directives for thin, heavy and double       *
*       left-sided, t-junction border characters.                             *
******************************************************************************/
#define TWIN_LEFT_T_THIN "├"
#define TWIN_LEFT_T_MID_HEAVY "┝"
#define TWIN_LEFT_T_TOP_HEAVY "┞"
#define TWIN_LEFT_T_BOT_HEAVY "┟"
#define TWIN_LEFT_T_TOP_BOT_HEAVY "┠"
#define TWIN_LEFT_T_TOP_MID_HEAVY "┡"
#define TWIN_LEFT_T_BOT_MID_HEAVY "┢"
#define TWIN_LEFT_T_BOLD "┣"
#define TWIN_LEFT_T_DOUBLE_HORZ "╞"
#define TWIN_LEFT_T_DOUBLE_VERT "╟"
#define TWIN_LEFT_T_DOUBLE_BOTH "╠"

/******************************************************************************
* TWIN_RIGHT_T_XXX:                                                           *
*       The compiler preprocessor directives for thin, heavy and double       *
*       right-sided, t-junction border characters.                            *
******************************************************************************/
#define TWIN_RIGHT_T_THIN "┤"
#define TWIN_RIGHT_T_MID_HEAVY "┥"
#define TWIN_RIGHT_T_TOP_HEAVY "┦"
#define TWIN_RIGHT_T_BOT_HEAVY "┧"
#define TWIN_RIGHT_T_TOP_BOT_HEAVY "┨"
#define TWIN_RIGHT_T_TOP_MID_HEAVY "┩"
#define TWIN_RIGHT_T_BOT_MID_HEAVY "┪"
#define TWIN_RIGHT_T_BOLD "┫"
#define TWIN_RIGHT_T_DOUBLE_HORZ "╡"
#define TWIN_RIGHT_T_DOUBLE_VERT "╢"
#define TWIN_RIGHT_T_DOUBLE_BOTH "╣"

/******************************************************************************
* TWIN_TOP_T_XXX:                                                             *
*       The compiler preprocessor directives for thin, heavy and double       *
*       top-sided, t-junction border characters.                              *
******************************************************************************/
#define TWIN_TOP_T_THIN "┬"
#define TWIN_TOP_T_LEFT_HEAVY "┭"
#define TWIN_TOP_T_RIGHT_HEAVY "┮"
#define TWIN_TOP_T_TOP_HEAVY "┯"
#define TWIN_TOP_T_BOT_HEAVY "┰"
#define TWIN_TOP_T_LEFT_BOT_HEAVY "┱"
#define TWIN_TOP_T_RIGHT_BOT_HEAVY "┲"
#define TWIN_TOP_T_BOLD "┳"
#define TWIN_TOP_T_DOUBLE_HORZ "╤"
#define TWIN_TOP_T_DOUBLE_VERT "╥"
#define TWIN_TOP_T_DOUBLE_BOTH "╦"

/******************************************************************************
* TWIN_BOT_T_XXX:                                                             *
*       The compiler preprocessor directives for thin, heavy and double       *
*       bottom-sided, t-junction border characters.                           *
******************************************************************************/
#define TWIN_BOT_T_THIN "┴"
#define TWIN_BOT_T_LEFT_HEAVY "┵"
#define TWIN_BOT_T_RIGHT_HEAVY "┶"
#define TWIN_BOT_T_TOP_HEAVY "┷"
#define TWIN_BOT_T_BOT_HEAVY "┸"
#define TWIN_BOT_T_LEFT_BOT_HEAVY "┹"
#define TWIN_BOT_T_RIGHT_BOT_HEAVY "┺"
#define TWIN_BOT_T_BOLD "┻"
#define TWIN_BOT_T_DOUBLE_HORZ "╧"
#define TWIN_BOT_T_DOUBLE_VERT "╨"
#define TWIN_BOT_T_DOUBLE_BOTH "╩"

/******************************************************************************
* TWIN_CROSS_XXX:                                                           *
*       The compiler preprocessor directives for thin, heavy and double       *
*       cross-junction border characters.                                     *
******************************************************************************/
#define TWIN_CROSS_THIN "┼"
#define TWIN_CROSS_LEFT "┽"
#define TWIN_CROSS_RIGHT "┾"
#define TWIN_CROSS_MID "┿"
#define TWIN_CROSS_TOP "╀"
#define TWIN_CROSS_BOT "╁"
#define TWIN_CROSS_VERT "╂"
#define TWIN_CROSS_LEFT_TOP "╃"
#define TWIN_CROSS_RIGHT_TOP "╄"
#define TWIN_CROSS_LEFT_BOT "╅"
#define TWIN_CROSS_RIGHT_BOT "╆"
#define TWIN_CROSS_MID_TOP "╇"
#define TWIN_CROSS_MID_BOT "╈"
#define TWIN_CROSS_LEFT_VERT "╉"
#define TWIN_CROSS_RIGHT_VERT "╊"
#define TWIN_CROSS_BOLD "╋"
#define TWIN_CROSS_DOUBLE_HORZ "╪"
#define TWIN_CROSS_DOUBLE_VERT "╫"
#define TWIN_CROSS_DOUBLE_BOTH "╬"

/******************************************************************************
* TWIN_DIAG_XXX:                                                              *
*       The compiler preprocessor directives for thin, diagonal border        *
*       characters.                                                           *
******************************************************************************/
#define TWIN_DIAG_RIGHT "╱"
#define TWIN_DIAG_LEFT "╲"
#define TWIN_DIAG_CROSS "╳"

/******************************************************************************
* TWIN_ANSI_UTIL:                                                             *
*       The compiler preprocessor directives for utility ANSI control codes   *
*       such as printing in bold and resetting any coloured text ANSI codes.  *
******************************************************************************/
#define TWIN_RESET_COLOUR "\e[0m"
#define TWIN_BOLD "\x1B[1m"

/******************************************************************************
* TWIN_ANSI_COLOURS:                                                          *
*       The compiler preprocessor directives for utility ANSI control codes   *
*       for printing coloured text from the 16-colour set.                    *
******************************************************************************/
#define TWIN_BLACK "\e[30m"
#define TWIN_RED "\e[31m"
#define TWIN_GREEN "\e[32m"
#define TWIN_YELLOW "\e[33m"
#define TWIN_BLUE "\e[34m"
#define TWIN_MAGENTA "\e[35m"
#define TWIN_CYAN "\e[36m"
#define TWIN_WHITE "\e[37m"

/******************************************************************************
* TWIN_ANSI_BRIGHT_COLOURS:                                                   *
*       The compiler preprocessor directives for utility ANSI control codes   *
*       for printing bright coloured text from the 16-colour set.             *
******************************************************************************/
#define TWIN_BRIGHT_BLACK "\e[30;1m"
#define TWIN_BRIGHT_RED "\e[31;1m"
#define TWIN_BRIGHT_GREEN "\e[32;1m"
#define TWIN_BRIGHT_YELLOW "\e[33;1m"
#define TWIN_BRIGHT_BLUE "\e[34;1m"
#define TWIN_BRIGHT_MAGENTA "\e[35;1m"
#define TWIN_BRIGHT_CYAN "\e[36;1m"
#define TWIN_BRIGHT_WHITE "\e[37;1m"

#endif
