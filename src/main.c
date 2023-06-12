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
 * set border type using BORDER TYPE enum and set_border() method
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
        printf("SEGFAULT");
        stop(EXIT_FAILURE);

    }
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
    set_title(window, "Window One");
    set_colour(window, BRIGHT_YELLOW);
    set_general_style(window, GENERAL_THIN_SOLID);
    set_top_left_style(window, TL_CURVED);
    set_top_right_style(window, TR_CURVED);
    set_bottom_left_style(window, BL_CURVED);
    set_bottom_right_style(window, BR_CURVED);
    set_contents(window, "Some contents are here but the list is very long");
    //draw_window(window);
    
    //draw_windows(1, window);
    
    window_t* window1 = create_window((size_t) sz.ws_row / 3, sz.ws_row - 5,(size_t) 20 + 5, sz.ws_col - 4);
    //window_t* window1 = create_window(2, (size_t) sz.ws_row / 2, 1, (size_t) sz.ws_col / 2);
    //draw_window(window1);
    set_colour(window1, BRIGHT_RED);
    set_title(window1, "Window Two");
    set_contents(window1, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum mollis ornare ornare. Donec vestibulum mi et sem tincidunt, vel suscipit turpis vestibulum. Duis euismod, nunc dapibus tempus maximus, quam risus mattis libero, id feugiat turpis mi nec nisi. Morbi semper leo non sem interdum malesuada. Mauris id urna aliquam, posuere mi sed, facilisis nibh. Nulla tincidunt odio ac molestie hendrerit. Vestibulum sapien urna, tincidunt ac hendrerit at, hendrerit et nisi. Integer ex eros, fermentum in gravida pellentesque, finibus nec sem. Nunc ac ullamcorper massa, nec pretium neque. Integer tristique pellentesque facilisis. Sed laoreet elit vitae gravida lacinia. Aenean bibendum nibh faucibus enim ultricies sagittis. Sed ante felis, hendrerit eu justo sit amet, luctus rutrum arcu. Donec eros risus, ullamcorper nec ultrices sit amet, molestie quis nisi. Nam aliquam eget velit non placerat. Integer lobortis non ipsum non sodales. Aenean egestas pellentesque tincidunt. Quisque ornare enim et ex viverra, et viverra leo tristique. In at tellus sed metus accumsan auctor vel a eros. Nulla ultrices mi eu erat molestie dapibus. Quisque ultricies magna eget metus tristique laoreet sed ac dolor. Donec id condimentum tortor, ac malesuada libero. Suspendisse tincidunt suscipit facilisis. Donec eu sollicitudin arcu. Ut nunc eros, rhoncus quis diam a, facilisis auctor sapien. Nunc lobortis nibh a justo faucibus tincidunt. Praesent sollicitudin gravida leo a dictum. Nulla sed erat eget justo vehicula congue a eu lacus. Vestibulum auctor aliquet mauris at rhoncus. Nam ullamcorper quam et interdum efficitur. Quisque euismod tellus mi, vitae auctor tortor egestas sed. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec consequat nunc sed volutpat euismod. Nulla dapibus semper lorem, in blandit est aliquam id. Aliquam ut sem et libero suscipit laoreet ut sed sem. Quisque nec pretium ligula. Quisque tempor consequat est at ullamcorper. Praesent dapibus quis felis et gravida. Duis tincidunt pulvinar dolor, eget condimentum velit fermentum id. Sed eleifend ac ex eget facilisis. Nullam in pellentesque nisl, quis semper leo. Praesent tristique ultrices urna. Quisque venenatis pretium molestie. Nam fringilla est pellentesque urna finibus, at aliquam quam bibendum. Donec est leo, vestibulum vel dolor ac, aliquet rhoncus augue. Maecenas vitae aliquam quam, id sagittis leo. Phasellus nec massa est. Proin dictum ligula at lacus euismod dictum. Nulla maximus neque vitae faucibus ornare. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras bibendum ultrices nisi, at euismod ipsum rhoncus non. Donec eget dui eget tortor pulvinar aliquam. Nam ex nunc, placerat id finibus eu, mattis nec turpis. Nullam laoreet ligula eros, ac ornare purus aliquam non. Nam lectus magna, auctor quis sagittis mollis, commodo et sapien. Pellentesque sed mauris velit. Donec vitae elit vel mauris dapibus fringilla a nec leo. Proin sed euismod massa, commodo lobortis lorem. Donec laoreet at magna ac mattis. Suspendisse nec ultrices mauris, sed viverra orci. Cras sodales vehicula risus id sagittis. In condimentum libero eget elit gravida, eget dapibus diam malesuada. Nulla lobortis ultricies ante, id placerat libero porta eget. Ut in fermentum velit, non semper massa. Cras odio mi, suscipit id volutpat. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum mollis ornare ornare. Donec vestibulum mi et sem tincidunt, vel suscipit turpis vestibulum. Duis euismod, nunc dapibus tempus maximus, quam risus mattis libero, id feugiat turpis mi nec nisi. Morbi semper leo non sem interdum malesuada. Mauris id urna aliquam, posuere mi sed, facilisis nibh. Nulla tincidunt odio ac molestie hendrerit. Vestibulum sapien urna, tincidunt ac hendrerit at, hendrerit et nisi. Integer ex eros, fermentum in gravida pellentesque, finibus nec sem. Nunc ac ullamcorper massa, nec pretium neque. Integer tristique pellentesque facilisis. Sed laoreet elit vitae gravida lacinia. Aenean bibendum nibh faucibus enim ultricies sagittis. Sed ante felis, hendrerit eu justo sit amet, luctus rutrum arcu. Donec eros risus, ullamcorper nec ultrices sit amet, molestie quis nisi. Nam aliquam eget velit non placerat. Integer lobortis non ipsum non sodales. Aenean egestas pellentesque tincidunt. Quisque ornare enim et ex viverra, et viverra leo tristique. In at tellus sed metus accumsan auctor vel a eros. Nulla ultrices mi eu erat molestie dapibus. Quisque ultricies magna eget metus tristique laoreet sed ac dolor. Donec id condimentum tortor, ac malesuada libero. Suspendisse tincidunt suscipit facilisis. Donec eu sollicitudin arcu. Ut nunc eros, rhoncus quis diam a, facilisis auctor sapien. Nunc lobortis nibh a justo faucibus tincidunt. Praesent sollicitudin gravida leo a dictum. Nulla sed erat eget justo vehicula congue a eu lacus. Vestibulum auctor aliquet mauris at rhoncus. Nam ullamcorper quam et interdum efficitur. Quisque euismod tellus mi, vitae auctor tortor egestas sed. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec consequat nunc sed volutpat euismod. Nulla dapibus semper lorem, in blandit est aliquam id. Aliquam ut sem et libero suscipit laoreet ut sed sem. Quisque nec pretium ligula. Quisque tempor consequat est at ullamcorper. Praesent dapibus quis felis et gravida. Duis tincidunt pulvinar dolor, eget condimentum velit fermentum id. Sed eleifend ac ex eget facilisis. Nullam in pellentesque nisl, quis semper leo. Praesent tristique ultrices urna. Quisque venenatis pretium molestie. Nam fringilla est pellentesque urna finibus, at aliquam quam bibendum. Donec est leo, vestibulum vel dolor ac, aliquet rhoncus augue. Maecenas vitae aliquam quam, id sagittis leo. Phasellus nec massa est. Proin dictum ligula at lacus euismod dictum. Nulla maximus neque vitae faucibus ornare. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras bibendum ultrices nisi, at euismod ipsum rhoncus non. Donec eget dui eget tortor pulvinar aliquam. Nam ex nunc, placerat id finibus eu, mattis nec turpis. Nullam laoreet ligula eros, ac ornare purus aliquam non. Nam lectus magna, auctor quis sagittis mollis, commodo et sapien. Pellentesque sed mauris velit. Donec vitae elit vel mauris dapibus fringilla a nec leo. Proin sed euismod massa, commodo lobortis lorem. Donec laoreet at magna ac mattis. Suspendisse nec ultrices mauris, sed viverra orci. Cras sodales vehicula risus id sagittis. In condimentum libero eget elit gravida, eget dapibus diam malesuada. Nulla lobortis ultricies ante, id placerat libero porta eget. Ut in fermentum velit, non semper massa. Cras odio mi, suscipit id volutpat.");
    set_general_style(window1, GENERAL_BOLD_SOLID);
    
    window_t* window2 = create_window((size_t) 2 * sz.ws_row / 3, sz.ws_row - 20, (size_t) 70, sz.ws_col - 10);
    set_colour(window2, BRIGHT_GREEN);
    set_title(window2, "Window Three");
    //set_z_index(window, 2);
    draw_windows(3, window, window1, window2);
    //draw_all_windows();
    while(1);
   /* 
    if (!draw_windows(2, window, window1)) overlap_error(__FILE__, __LINE__);
    //if(!draw_all_windows()) overlap_error(__FILE__, __LINE__);
     
    while (1) {
        if (resized) {
            resize_flex_windows(flex_windows);
            system("clear");
            //if (!draw_windows(2, window, window1)) overlap_error();
            //ioctl(0, TIOCGWINSZ, &window->current_terminal_sz);
            //set_colour(window, BRIGHT_RED);
            //set_title(window, "Changed");
            //resize_window(window, 2, window->current_terminal_sz.ws_row - 1, 1, window->current_terminal_sz.ws_col - 1);
            //draw_window(window);
            resized = 0;
        }
    }
    */
    return 0;
}
