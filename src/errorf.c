/******************************************************************************
* FILENAME :        errorf.c                                                  *
*                                                                             *
* DESCRIPTION :                                                               *
*       Functions to print GCC style errors that may ocurr during twin usage. *
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "twin/errorf.h"
#include "twin/util.h"


/******************************************************************************
* subst:                                                                      *
*       replace all instances of `from` with `to` in the string `s`.          *
*                                                                             *
* @param s: the string to have some characters replaced.                      *
* @param from: the character that will have all instances replaced.           *
* @param to: the character the found instances will be replaced with.         *
******************************************************************************/
void subst (char *s, const char from, const char to) {
    while (*s == from)
        *s++ = to;
}

/******************************************************************************
* print_header                                                                *
*       Print the GCC style error header, from offending code file to the     *
*       supplementary error or warning message.                               *
*                                                                             *
* @param file_name: The file containing the error or warning.                 *
* @param line_num: The line containing the error or warning.                  *
* @param col_num: The column containing the error or warning.                 *
* @param type_col: The colour of the error or warning following the GCC style.*
* @param type: The type of the message, either error or warning.              *
* @param reason: The main reason for the error or warning.                    *
* @param message: A supplementary message for the error or warning.           *
******************************************************************************/
void print_header (const char* file_name, const size_t line_num,
                   const size_t col_num, const char* type_col,
                   const char* type, const char* reason, const char* message) {
    FILE* file;
    if((file = fopen(file_name, "r")) == NULL) {
        twin_print_error_header(__FILE__, __LINE__, 0, "file does not exist",
                                "the file_name that you supplied to "
                                "print_header() does not exist");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "%s%s:%zu:%zu: %s%s:%s%s %s:%s %s\n", TWIN_ERRORF_BOLD,
            file_name ? file_name : __FILE__, line_num ? line_num : __LINE__,
            col_num, type_col, type, TWIN_ERRORF_RESET, TWIN_ERRORF_BOLD,
            reason, TWIN_ERRORF_RESET, message);
}


/******************************************************************************
* twin_print_error_header:                                                    *
*       Print a GCC style error using the supplied arguments. For example:    *
*       "src/main.c:14:7: contents error: The content supplied is too large." *
*                                                                             *
* @param file_name: The file containing the error, defaulted to __FILE__.     *
* @param line_num: The line number containing the error, by defualt __LINE__. *
* @param col_num: The column number containing the error.                     *
* @param reason: The error type of the the error message being printed.       *
* @param message: The supplementary error message detailing the error reason. *
******************************************************************************/
void twin_print_error_header(const char* file_name, const size_t line_num,
                        const size_t col_num, const char* reason,
                        const char* message) {
    print_header(file_name, line_num, col_num, TWIN_ERRORF_BRIGHT_RED,
                 "error", reason, message);
}

/******************************************************************************
* twin_print_warning_header:                                                  *
*       Print a GCC style warning using the supplied arguments. For example:  *
*       "src/main.c:14:7: contents warning: The content supplied is large."   *
*                                                                             *
* @param file_name: The file containing the warning, defaulted to __FILE__.   *
* @param line_num: The line containing the warning, by defualt __LINE__.      *
* @param col_num: The column number containing the warning.                   *
* @param reason: The type of the the warning message being printed.           *
* @param message: The supplementary warning message.                          *
******************************************************************************/
void twin_print_warning_header(const char* file_name, const size_t line_num,
                          const size_t col_num, const char* reason,
                          const char* message) {
    print_header(file_name, line_num, col_num, TWIN_ERRORF_BRIGHT_MAGENTA,
                 "warning", reason, message);
}

/******************************************************************************
* print_underline:                                                            *
*       Print a ~ underline and pointer ^ to the token that caused some       *
*       error or warning, following the GCC style.                            *
*                                                                             *
* @param tok: The token that cause some error or warning.                     *
* @param start: The column containing the first character of the token.       *
******************************************************************************/
void print_underline (char* tok, const size_t start) {
    const int space = start;
    const int tok_len = strlen(tok);
    char tilde_underline[tok_len];
    tilde_underline[0] = '\0';
    if (tok_len > 1) {
        sprintf(tilde_underline, "%0*d", tok_len - 1, 0);
        subst(tilde_underline, '0', '~');
    }
    fprintf(stderr, "%s%*c%s\n", TWIN_ERRORF_BRIGHT_GREEN, space, '^',
            tilde_underline);
}

/******************************************************************************
* print_error_line:                                                           *
*       Print a line from a supplied file - a line containing a warning or    *
*       error for which you are printing a GCC style error.                   *
*                                                                             *
* @param error_line_num: The line number containing the error or warning.     *
* @param file_name: The file containing the error or warning.                 *
******************************************************************************/
void print_error_line (const size_t error_line_num, const char* file_name) {
    FILE* file; 
    if ((file = fopen(file_name, "r")) == NULL){
        twin_print_error_header(__FILE__, __LINE__, 0, "file does not exist",
                                "the file_name that you supplied to "
                                "print_header() does not exist");
        exit(EXIT_FAILURE);
    }
    char line_buf[TWIN_ERRORF_LINE_MAX];
    for (size_t i = 0; i < error_line_num + 1; i++)
        fgets(line_buf, TWIN_ERRORF_LINE_MAX, file);
    fprintf(stderr, "%s%s", TWIN_ERRORF_RESET, line_buf);
    fclose(file);
}

/******************************************************************************
* twin_overlap_error:                                                         *
*       An in-built error for when windows overlapping inhibits some          *
*       functionality.                                                        *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_overlap_error (const char* file, const size_t line) {
    twin_terminal_stop_config();
    system("clear");
    twin_print_error_header(file, line, 0, "overlapping windows",
                            "a call to draw_windows() contains windows that "
                            "overlap each other");
    exit(EXIT_FAILURE);
}

/******************************************************************************
* twin_contents_error:                                                        *
*       An in-built error for when the contents supplied to                   *
*       twin_set_contents() is too long for the supplied window.              *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_contents_error (const char* file, const size_t line) {
    twin_terminal_stop_config();
    system("clear");
    twin_print_error_header(file, line, 0, "window content too large",
                            "the contents you provided for the window is too "
                            "large to fit within the available space");
    exit(EXIT_FAILURE);
}

/******************************************************************************
* twin_z_index_error:                                                         *
*       An in-built error for when an invalid z-index is supplied to          *
*       twin_set_z_index().                                                   *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_z_index_error (const char* file, const size_t line) {
    twin_terminal_stop_config();
    system("clear");
    twin_print_error_header(file, line, 0, "negative z index",
                            "the z_index value of a window cannot be "
                            "negative - it is used for the rendering order "
                            "of the active windows");
    exit(EXIT_FAILURE);
}

/******************************************************************************
* twin_window_count_error:                                                    *
*       An in-built error for when the number of variadic twin_window_t*      *
*       arguments passed to twin_draw_windows() does not match the num        *
*       argument.                                                             *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_window_count_error (const char* file, const size_t line) {
    twin_terminal_stop_config();
    system("clear");
    twin_print_error_header(file, line, 0, "window count error",
                            "the number of windows supplied to draw_windows() "
                            "does not match the the supplied num variable");

}

/******************************************************************************
* twin_double_init_error:                                                     *
*       An in-built error for when twin_init() is called twice in one         *
*       execution.                                                            *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_double_init_error (const char* file, const size_t line) {
    twin_terminal_stop_config();
    system("clear");
    twin_print_error_header(file, line, 0, "double init error",
                            "twin has already been initialised and therefore "
                            "twin_init() cannot be called again");
    exit(EXIT_FAILURE);
}
