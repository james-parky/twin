#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>



#define TOP_LEFT_CORNER "┌"
#define BOTTOM_LEFT_CORNER "└"
#define TOP_RIGHT_CORNER "┐"
#define BOTTOM_RIGHT_CORNER "┘"
#define HORIZONTAL "─"
#define VERTICAL "│"

#define CYAN "\e[36;1m"
#define RED "\e[36;31m"
#define RESET_COLOUR "\e[0m"
/*
 * IDEAS
 *
 * have a struct for window which has linked list of frames 
 * has count of frames which is incremented
 * frames have coordinates
 *
 *
 *
 *
 * */


/*
 * Create window dimension struct and take multiple with va args
 * then iterate through each row and column like a tape, if any cell is part of a any frames top bot left or right draw the appropriate character
 *
 * implement dimension check
 *
 * auto redrwa on terminal size change
 *
 * threading for keyboard input so it doesnt have to have input to redraw
 *
 * if terminal too small show error like spotify
 * can do this straight away as part of library rather than implementation
 *
 * */

struct dim_t {
    size_t top, bot, left, right;
};
typedef struct dim_t dim_t;

struct window_t {
    char* title;
    char** contents;
    dim_t* dim;
    char* colour;
    struct winsize terminal_sz;
};
typedef struct window_t window_t;


window_t* create_window (size_t top, size_t bot, size_t left,
                               size_t right) {
    dim_t* dim = malloc(sizeof(dim_t));
    dim->top = top;
    dim->bot = bot;
    dim->left = left;
    dim->right = right;

    window_t* window = malloc(sizeof(window_t));
    window->dim = dim;

    ioctl(0, TIOCGWINSZ, &window->terminal_sz);
    return window;
}

void resize_window (window_t* window, size_t top, size_t bot,
                        size_t left, size_t right) {
    window->dim->top = top;
    window->dim->bot = bot;
    window->dim->left = left;
    window->dim->right = right;
}

void set_colour (window_t* window, const char* colour) {
    if (strcmp(colour, "Cyan") == 0) {
        window->colour = malloc(strlen(CYAN) + 1);
        strncpy(window->colour, CYAN, strlen(CYAN));
        window->colour[strlen(CYAN)] = '\0';
    }
    else if (strcmp(colour, "Red") == 0) {
        window->colour = malloc(strlen(RED) + 1);
        strncpy(window->colour, RED, strlen(RED));
        window->colour[strlen(RED)] = '\0';
    }
}

void set_title (window_t* window, const char* title) {
    window->title = malloc(strlen(title) + 1);
    strncpy(window->title, title, strlen(title));
    window->title[strlen(title)] = '\0';
}


/*
 * sort dimensions to get a list of when to turn on and off drawing 
 *
 * cant just do < left since it may be left of one but right of another
 *
 *
 *
 */



void draw_windows (size_t num, ...) {
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);
    va_list windows;
    size_t count = 0;
    va_start(windows, num);
    for (size_t i = 0; i < num; i++) {
        va_arg(windows, window_t*);
        count++;
    }
    if (count != num) {
        printf("bad args");
        exit(EXIT_FAILURE);
    }
    va_end(windows);

    dim_t* dims[num];
    char* titles[num];
    va_start(windows, num);
    for (size_t i = 0; i < num; i++) {
        window_t* window = va_arg(windows, window_t*);

        dims[i] = window->dim;
        titles[i] = malloc(strlen(window->title) + 1);
        strncpy(titles[i], window->title, strlen(window->title));
        titles[i][strlen(window->title)] = '\0';
    }

    for (size_t row = 0; row < sz.ws_row; row++){
        for (size_t col = 0; col < sz.ws_col; col++) {
            int is_in = 0;
            for (size_t i = 0; i < num; i++) {
                if (row == dims[i]->top) {
                    if (col < dims[i]->left) printf(" ");
                    if (col == dims[i]->left) { 
                        printf(TOP_LEFT_CORNER);
                        printf("%s", titles[i]);
                        col += strlen(titles[i]);
                    }
                    else if (col == dims[i]->right) printf(TOP_RIGHT_CORNER);
                    else printf(HORIZONTAL);
                }
                else printf(" ");
                //if (row >= dims[i]->top && row <= dims[i]->bot && col >= dims[i]->left && col <= dims[i]->right) is_in = 1;
            }
            //if (is_in) printf("x");
            //else printf(" ");
        }
    }




}




void draw_window (window_t* window) {
    if (window->colour) printf("%s", window->colour);
    for (size_t row = 0; row <= window->terminal_sz.ws_row; row++) {
        if (row < window->dim->top) {
            for (size_t col = 0; col < window->terminal_sz.ws_col; col++)
                printf(" ");
        }
        else if (row == window->dim->top) {
            for (size_t col = 0; col <= window->terminal_sz.ws_col; col++) {
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
            for (size_t col = 0; col <= window->terminal_sz.ws_col; col++) {
                if (col < window->dim->left) printf(" ");
                else if (col == window->dim->left) printf("%s", VERTICAL);
                else if (col > window->dim->left && col < window->dim->right - 1) printf(" ");
                else if (col == window->dim->right) printf("%s", VERTICAL);
                else if (col > window->dim->right) printf(" ");
            }
        }
        else if (row == window->dim->bot) {
            for (size_t col = 0; col <= window->terminal_sz.ws_col; col++) {
                if (col < window->dim->left) printf(" ");
                else if (col == window->dim->left) printf("%s", BOTTOM_LEFT_CORNER);
                else if (col > window->dim->left && col < window->dim->right - 1) printf("%s", HORIZONTAL);
                else if (col == window->dim->right) printf("%s", BOTTOM_RIGHT_CORNER);
                else if (col > window->dim->right) printf(" ");
            }
        }
        else if (row > window->dim->bot) printf("\n\r");
    }
    printf("%s\r", RESET_COLOUR);
}


void draw_border (const char* title, const size_t top, const size_t bot,
                  const size_t left, const size_t right) {
    size_t width = right - left;
    size_t height = bot - top;
    size_t title_len = strlen(title);

    for (size_t i = 0; i < top; i++) printf("\n");



    for (size_t i = 0; i < left; i++) printf("%c",' ');
    printf("%s", TOP_LEFT_CORNER);
    printf("%s", title);
    for (size_t i = left; i < right - title_len - 2; i++)
        printf("%s", HORIZONTAL);
    printf("%s\n\r", TOP_RIGHT_CORNER);

    for (size_t row = top + 1; row < bot - 1; row++) {
        for (size_t j = 0; j < left; j++) printf(" ");
        printf("%s", VERTICAL);
        for (size_t i = 0; i < width - 2; i++) printf(" ");
        printf("%s\n\r", VERTICAL);
    }

    for (size_t i = 0; i < left; i++) printf(" ");
    printf("%s", BOTTOM_LEFT_CORNER);
    for (size_t i = left; i < right - 2; i++) printf("%s", HORIZONTAL);
    printf("%s", BOTTOM_RIGHT_CORNER);

}

void* exit_listener () {
    char c;
    while ((c = getchar()) != 'q');
    pthread_exit((void*) 1);
}

void* sz_listener () {
    struct winsize current_sz;
    struct winsize prev_sz;
    ioctl(0, TIOCGWINSZ, &current_sz);
    ioctl(0, TIOCGWINSZ, &prev_sz);

    while (1) {
        ioctl(0, TIOCGWINSZ, &current_sz);
        if (current_sz.ws_row != prev_sz.ws_row || current_sz.ws_col != prev_sz.ws_col) {
            return ((void*) 1);
        }
        prev_sz.ws_col = current_sz.ws_col;
        prev_sz.ws_row = current_sz.ws_row;
    }

}


int main () {
    system("tput smcup");
    printf("\e[?25l");
    printf("\e[?30l");

    char c;
    struct winsize sz;
    //struct winsize prev_sz;
    ioctl(0, TIOCGWINSZ, &sz);
    //ioctl(0, TIOCGWINSZ, &prev_sz);
    system("/bin/stty raw");
    window_t* window = create_window(2, sz.ws_row - 1, 1, sz.ws_col - 1);
    //TF1_window* window = TF1_create_window(10, 10, 10, 10);
    set_title(window, "Testing");
    set_colour(window, "Cyan");

    pthread_t exit_thread;
    pthread_create(&exit_thread, NULL, exit_listener, NULL);
    pthread_t sz_thread;
    pthread_create(&sz_thread, NULL, sz_listener, NULL);
    
    while(1) {
        system("clear");
        struct winsize temp_sz;
        ioctl(1, TIOCGWINSZ, &temp_sz);
        system("tput cols");
        system("tput lines");
        /*
        if (temp_sz.ws_row < window->dim->bot || temp_sz.ws_col < window->dim->right) {
            system("clear");
            exit(EXIT_FAILURE);
            printf("changed");
            set_colour(window, "Red");
            set_title(window, "Error");
        }
        */
        //draw_window(window);
        void* thread_ended;
        pthread_join(exit_thread, &thread_ended);
        if (thread_ended) break;
    }
    system("clear");
    system("/bin/stty cooked");
    printf("\e[?25h");
    system("tput rmcup");
    return 0;
}
