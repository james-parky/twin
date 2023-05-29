#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include "errorf.h"
#include "window.h"
#include "util.h"
#include "colours.h"
/*
 * MUST HAVES FOR LIBRARY
 *
 * create windows
 * set colour
 * set title
 * set contents
 * draw window
 * draw windows
 * errors when draw space is too small
 * draw returns an int to check if it drew properly or not
 * can be based on struff such as not enough space or invlaid coords
 *
 * IMPLEMENTATION
 * auto resize
 * updating content
 * close one q
 * resize on SIGWITCH
 *
 * */

int update_happened = 0;

void exit_listener () {
    char c;
    while ((c = getchar()) != 'q');
    stop(EXIT_SUCCESS);    
}
int resized = 0;


void sig_handler (int signo) {
    if (signo == SIGWINCH) {
        resized = 1;
    }
    if (signo == SIGSEGV) {
        stop(EXIT_FAILURE);

    }
}

int main () {
    terminal_start_config();
    pthread_t exit_thread;
    pthread_create(&exit_thread, NULL, (void*) exit_listener, NULL);
    signal(SIGWINCH, sig_handler); 
    signal(SIGSEGV, sig_handler);

    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    window_t* window = create_window(2, (size_t) sz.ws_row / 2, 1, (size_t) sz.ws_col / 2);
    set_title(window, "Testing");
    set_colour(window, BRIGHT_GREEN);

    char* contents = "Some contents are here but the list is very long";
    set_contents(window, contents);
    //draw_window(window);
    //draw_windows(1, window);
    window_t* window1 = create_window((size_t) sz.ws_row / 2, sz.ws_row - 5,(size_t) sz.ws_col / 2 + 5, sz.ws_col - 4);
    //window_t* window1 = create_window(2, (size_t) sz.ws_row / 2, 1, (size_t) sz.ws_col / 2);
    //draw_window(window1);
    set_colour(window1, BRIGHT_RED);
    set_title(window1, "Window Two");
    set_contents(window1, "This window has some contents too");
    if (!draw_windows(2, window, window1)) overlap_error();
    
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
