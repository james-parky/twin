/******************************************************************************
* FILENAME :        twin/errorf.h                                             *
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
#ifndef __TWIN_ERRORF_H__
#define __TWIN_ERRORF_H__

/******************************************************************************
* TWIN_ERRORF_XXX                                                             *
*       The compiler preprocessor directives for the required ANSI colour     *
*       contorl codes when printing GCC formatted errors ocurring during twin *
*       usage.                                                                *
******************************************************************************/
#define TWIN_ERRORF_BRIGHT_RED "\e[31;1m"
#define TWIN_ERRORF_BRIGHT_GREEN "\e[32;1m"
#define TWIN_ERRORF_BRIGHT_MAGENTA "\e[35;1m"
#define TWIN_ERRORF_RESET "\033[0m"
#define TWIN_ERRORF_BOLD "\x1B[1m"

/******************************************************************************
* TWIN_ERRORF_LINE_MAX:                                                       *
*       The maximum line length for printing a specific line error.           *
******************************************************************************/
#define TWIN_ERRORF_LINE_MAX 1024

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
void twin_print_error_header (const char* file_name, const size_t line_num,
                              const size_t col_num, const char* reason,
                              const char* message);

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
void twin_print_warning_header (const char* file_name, const size_t line_num,
                                const size_t col_num, const char* reason,
                                const char* message); 

/******************************************************************************
* twin_overlap_error:                                                         *
*       An in-built error for when windows overlapping inhibits some          *
*       functionality.                                                        *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_overlap_error (const char* file, const size_t line);

/******************************************************************************
* twin_contents_error:                                                        *
*       An in-built error for when the contents supplied to                   *
*       twin_set_contents() is too long for the supplied window.              *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_contents_error (const char* file, const size_t line);

/******************************************************************************
* twin_z_index_error:                                                         *
*       An in-built error for when an invalid z-index is supplied to          *
*       twin_set_z_index().                                                   *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_z_index_error (const char* file, const size_t line);

/******************************************************************************
* twin_window_count_error:                                                    *
*       An in-built error for when the number of variadic twin_window_t*      *
*       arguments passed to twin_draw_windows() does not match the num        *
*       argument.                                                             *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_window_count_error (const char* file, const size_t line);

/******************************************************************************
* twin_double_init_error:                                                     *
*       An in-built error for when twin_init() is called twice in one         *
*       execution.                                                            *
*                                                                             *
* @param file: The file containing the offending code, most likely __FILE__.  *
* @param line: The line containing the offenidng code, most likely __LINE__.  *
******************************************************************************/
void twin_double_init_error (const char* file, const size_t line);

#endif
