#include <stdarg.h>
#include <sys/ioctl.h>


struct dim_t {
    size_t top, bot, left, right;
};
typedef struct dim_t dim_t;

struct window_t {
    char* title;
    char* contents;
    size_t contents_ptr;
    dim_t* dim;
    char* colour;
    struct winsize current_terminal_sz;
    struct winsize original_terminal_sz;
};
typedef struct window_t window_t;

struct node_t {
    window_t* window;
    struct node_t* next;
};
typedef struct node_t node_t;

node_t* new_node (window_t* window);

node_t* insert_at_tail (node_t* tail, node_t* new);

window_t* create_window (size_t top, size_t bot, size_t left,
                               size_t right);

void resize_window (window_t* window, size_t _top, size_t _bot,
                        size_t _left, size_t _right);

void set_colour (window_t* window, const char* colour);

void set_title (window_t* window, const char* title);

void set_contents(window_t* window, char* contents);

int draw_windows (size_t num, ...);

void draw_window (window_t* window);

void twin_init();

extern window_t* flex_window_head;
extern node_t* flex_windows;
extern node_t* flex_windows_tail;