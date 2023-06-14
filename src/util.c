/******************************************************************************
* FILENAME :        util.c                                                    *
*                                                                             *
* DESCRIPTION :                                                               *
*       Functions to start and stop the required terminal mode for drawing    *
*       windows using twin.                                                   *
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
#include <stdio.h>
#include <stdlib.h>

#include "twin/util.h"

/******************************************************************************
* twin_terminal_start_config:                                                 *
*       Prepare the terminal for drawing windows with twin by entering it     *
*       into the alternate mode, making the cursor invisible, positioning the *
*       cursor at [0,0] and setting stty type to raw.                         *   
******************************************************************************/
void twin_terminal_start_config () {
    system("tput smcup");
    printf("\e[?25l");
    printf("\e[?30l");
    printf("\e[?30h");
    system("/bin/stty raw");
}

/******************************************************************************
* twin_terminal_stop_config:                                                  *
*       Reset the terminal back to regular by making the cursor visible       *
*       again, setting stty back to cooked, and exiting alternate mode.       *
******************************************************************************/
void twin_terminal_stop_config () {
    system("/bin/stty cooked");
    printf("\e[?25h");
    system("tput rmcup");

}

/******************************************************************************
* twin_terminal_stop_config:                                                  *
*       Reset the terminal back to regular by making the cursor visible       *
*       again, setting stty back to cooked, and exiting alternate mode.       *
******************************************************************************/
void twin_stop (int exit_code) {
    system("clear");
    twin_terminal_stop_config();
    exit(exit_code);
}


