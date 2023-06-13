/******************************************************************************
* FILENAME :        twin/util.h                                               *
*                                                                             *
* DESCRIPTION :                                                               *
*       Functions to start and stop the required terminal mode for drawing    *
*       windows using twin.                                                   *
*                                                                             *
* NOTES :                                                                     *
*       These preprocessor directives are part of the twin library;           *
*       See https://github.com/james-parky/twin for more detail.              *
*                                                                             *
*       Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>  *
*       Everyone is permitted to copy and distribute verbatim copies of this  *
*       licensed document, but changing it is not allowed.                    *
*                                                                             *
* AUTHOR :    James David Parkington        START DATE :    04 May 23         *
******************************************************************************/
#ifndef __TWIN_UTIL_H__
#define __TWIN_UTIL_H__

/******************************************************************************
* twin_terminal_start_config:                                                 *
*       Prepare the terminal for drawing windows with twin by entering it     *
*       into the alternate mode, making the cursor invisible, positioning the *
*       cursor at [0,0] and setting stty type to raw.                         *   
******************************************************************************/
void twin_terminal_start_config ();

/******************************************************************************
* twin_terminal_stop_config:                                                  *
*       Reset the terminal back to regular by making the cursor visible       *
*       again, setting stty back to cooked, and exiting alternate mode.       *
******************************************************************************/
void twin_terminal_stop_config ();

/******************************************************************************
* twin_stop:                                                                  *
*       Clears the screen, calls twin_terminal_stop_config(), and exits with  *
*       the supplied exit_code.                                               *
*                                                                             *
* @param exit_code: The exit code supplied to exit().                         *
******************************************************************************/
void twin_stop (int exit_code);

#endif
