#ifndef WINDOW_MANAGER_XLIB_H
#define WINDOW_MANAGER_XLIB_H

#include<X11/Xlib.h>

extern char wm_detect;

char IsDisplayNull(Display *d);

int DetectWM();

int ErrorHandler();

int EventLoop(Display *display_);

#endif
