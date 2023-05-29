#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "errorf.h"
#include "util.h"

void subst (char *s, const char from, const char to) {
    while (*s == from)
        *s++ = to;
}

void print_header (const char* file_name, const size_t line_num,
                   const size_t col_num, const char* type_col,
                   const char* type, const char* reason, const char* message) {
    FILE* file;
    if((file = fopen(file_name, "r")) == NULL) {
        print_error_header(__FILE__, __LINE__, 0,
                           "file does not exist",
                           "the file_name that you supplied to print_header()"
                           "does not exist");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "%s%s:%zu:%zu: %s%s:%s%s %s:%s %s\n", BOLD,
            file_name ? file_name : __FILE__, line_num ? line_num : __LINE__,
            col_num, type_col, type, RESET, BOLD, reason, RESET, message);
}

void print_error_header(const char* file_name, const size_t line_num,
                        const size_t col_num, const char* reason,
                        const char* message) {
    print_header(file_name, line_num, col_num, BRIGHT_RED, "error", reason,
                 message);
}

void print_warning_header(const char* file_name, const size_t line_num,
                          const size_t col_num, const char* reason,
                          const char* message) {
    print_header(file_name, line_num, col_num, BRIGHT_MAGENTA, "warning",
                 reason, message);
}


void print_underline (char* tok, const size_t start) {
    const int space = start;
    const int tok_len = strlen(tok);
    char tilde_underline[tok_len];
    tilde_underline[0] = '\0';
    if (tok_len > 1) {
        sprintf(tilde_underline, "%0*d", tok_len - 1, 0);
        subst(tilde_underline, '0', '~');
    }
    fprintf(stderr, "%s%*c%s\n", BRIGHT_GREEN, space, '^', tilde_underline);
}

void print_error_line (const size_t error_line_num, const char* file_name) {
    FILE* file; 
    if ((file = fopen(file_name, "r")) == NULL){
        print_error_header(__FILE__, __LINE__, 0,
                           "file does not exist",
                           "the file_name that you supplied to print_header()"
                           "does not exist");
        exit(EXIT_FAILURE);
    }
    char line_buf[LINE_MAX];
    for (size_t i = 0; i < error_line_num + 1; i++)
        fgets(line_buf, LINE_MAX, file);
    fprintf(stderr, "%s%s", RESET, line_buf);
    fclose(file);
}

void overlap_error () {
    terminal_stop_config();
    system("clear");
    print_error_header(__FILE__, 0, 0, "overlapping windows", "a call to draw_windows() contains windows that overlap each other");
    exit(EXIT_FAILURE);
}

void contents_error () {
    terminal_stop_config();
    system("clear");
    print_error_header(__FILE__, 0, 0, "window content too large", "the contents you provided for the window is too large to fit within the available space");
    exit(EXIT_FAILURE);

}


