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

#define RESET_COLOUR "\e[0m"

#define BLACK "\e[30m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define BLUE "\e[34m"
#define MAGENTA "\e[35m"
#define CYAN "\e[36m"
#define WHITE"\e[37m"

#define BRIGHT_BLACK "\e[30;1m"
#define BRIGHT_RED "\e[31;1m"
#define BRIGHT_GREEN "\e[32;1m"
#define BRIGHT_YELLOW "\e[33;1m"
#define BRIGHT_BLUE "\e[34;1m"
#define BRIGHT_MAGENTA "\e[35;1m"
#define BRIGHT_CYAN "\e[36;1m"
#define BRIGHT_WHITE"\e[37;1m"

int update_happened = 0;

void terminal_start_config () {
    system("tput smcup");
    printf("\e[?25l");
    printf("\e[?30l");
    printf("\e[?30h");
    system("/bin/stty raw");
}

void terminal_stop_config () {
    system("/bin/stty cooked");
    printf("\e[?25h");
    system("tput rmcup");

}
void stop (int exit_code) {
    system("clear");
    terminal_stop_config();
    exit(exit_code);
}




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
    struct winsize current_terminal_sz;
    struct winsize original_terminal_sz;
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
    /*
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
    */
    window->colour = malloc(strlen(colour) + 1);
    strncpy(window->colour, colour, strlen(colour));
    window->colour[strlen(colour)] = '\0';
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
                else if (col > window->dim->left && col < window->dim->right - 1) printf(" ");
                else if (col == window->dim->right) printf("%s", VERTICAL);
                else if (col > window->dim->right) printf(" ");
            }
        }
        else if (row == window->dim->bot) {
            for (size_t col = 0; col <= window->current_terminal_sz.ws_col; col++) {
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

void* exit_listener () {
    char c;
    while ((c = getchar()) != 'q');
    stop(EXIT_SUCCESS);    
}
int resized;


void sig_handler (int signo) {
    if (signo == SIGWINCH) {
        resized = 1;
    }
}
int main () {
    terminal_start_config();
    pthread_t exit_thread;
    pthread_create(&exit_thread, NULL, exit_listener, NULL);
    signal(SIGWINCH, sig_handler); 

    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    window_t* window = create_window(2, sz.ws_row - 50, 1, sz.ws_col - 10);
    set_title(window, "Testing");
    set_colour(window, BRIGHT_GREEN);
    draw_window(window);

    while (1) {
        if (resized) {
            system("clear");
            ioctl(0, TIOCGWINSZ, &window->current_terminal_sz);
            set_colour(window, BRIGHT_RED);
            set_title(window, "Changed");
            resize_window(window, 2, window->current_terminal_sz.ws_row - 1, 1, window->current_terminal_sz.ws_col - 1);
            draw_window(window);
            resized = 0;
        }
    }
    return 0;
}
