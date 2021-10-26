//+build xlib

#ifndef WINDOW_MANAGER_XLIB_H
#define WINDOW_MANAGER_XLIB_H

#include<X11/Xlib.h>

extern char wm_detect;

extern int DetectWM();

extern int ErrorHandler();

extern int EventLoop(Display *display_);

#endif
