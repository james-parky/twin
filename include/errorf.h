#ifndef ERRORF_H_
#define ERRORF_H_

#define BRIGHT_RED "\e[31;1m"
#define BRIGHT_GREEN "\e[32;1m"
#define BRIGHT_MAGENTA "\e[35;1m"
#define RESET "\033[0m"
#define BOLD "\x1B[1m"

#define LINE_MAX 1024

void print_error_header (const char* file_name, const size_t line_num,
                        const size_t col_num, const char* reason,
                        const char* message);

void print_warning_header (const char* file_name, const size_t line_num,
                          const size_t col_num, const char* reason,
                          const char* message); 

void overlap_error (const char* file, const size_t line);

void contents_error (const char* file, const size_t line);

void z_index_error (const char* file, const size_t line);

void window_count_error (const char* file, const size_t line);

void double_init_error (const char* file, const size_t line);

#endif
