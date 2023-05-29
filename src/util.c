#include <stdio.h>
#include <stdlib.h>

#include "util.h"

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


