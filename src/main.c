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
 * keep a linked list of all windows that exist
 * on a terminal resize, all windows are resized properly
 *      if it is made narrowwer, all windows are made narrower by the same amount
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

void resize_flex_windows (node_t* flex_windows) {
    struct winsize new_sz;
    ioctl(0, TIOCGWINSZ, &new_sz);
    node_t* tmp = flex_windows->next;
    struct winsize org_sz = tmp->window->current_terminal_sz;
    
    while (tmp) {
        window_t* win = tmp->window;
        //if (new_sz.ws_row < win->current_terminal_sz.ws_row) resize_window(win, )
        if (new_sz.ws_row != win->current_terminal_sz.ws_row) resize_window(win, win->dim->top, win->dim->bot - (win->current_terminal_sz.ws_row - new_sz.ws_row), win->dim->left, win->dim->right);
        if (new_sz.ws_col != win->current_terminal_sz.ws_col) resize_window(win, win->dim->top, win->dim->bot, win->dim->left, win->dim->right - (win->current_terminal_sz.ws_col - new_sz.ws_col));
        win->current_terminal_sz = new_sz; 
        tmp = tmp->next;
    }

    /*
    size_t v_diff = (new_sz.ws_col > org_sz.ws_col) ? (new_sz.ws_col - org_sz.ws_col) : (org_sz.ws_col - new_sz.ws_col);
    size_t h_diff = (new_sz.ws_row > org_sz.ws_row) ? (new_sz.ws_row - org_sz.ws_row) : (org_sz.ws_row - new_sz.ws_row);
    if (new_sz.ws_col != org_sz.ws_col) {
        while (tmp) {
            window_t* win = tmp->window;
            //resize_window(win, (win->dim->top), (), (), ());
            tmp = tmp->next;
        }
    }
    if (new_sz.ws_row != org.sz_ws.row) {

    }
    */
}

int main () {
    twin_init();
    terminal_start_config();
    pthread_t exit_thread;
    pthread_create(&exit_thread, NULL, (void*) exit_listener, NULL);
    signal(SIGWINCH, sig_handler); 
    signal(SIGSEGV, sig_handler);
    
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    window_t* window = create_window(2, (size_t) sz.ws_row / 2, 1, (size_t) sz.ws_col / 2);
    set_title(window, "Testing");
    set_colour(window, BRIGHT_YELLOW);

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
            resize_flex_windows(flex_windows);
            system("clear");
            if (!draw_windows(2, window, window1)) overlap_error();
            //ioctl(0, TIOCGWINSZ, &window->current_terminal_sz);
            //set_colour(window, BRIGHT_RED);
            //set_title(window, "Changed");
            //resize_window(window, 2, window->current_terminal_sz.ws_row - 1, 1, window->current_terminal_sz.ws_col - 1);
            //draw_window(window);
            resized = 0;
        }
    }
    
    
    return 0;
}
