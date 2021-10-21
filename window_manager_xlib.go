//+build xlib

package main

/*
#cgo LDFLAGS: -lX11

#include"window_manager_xlib.h"
#include<X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

static char IsDisplayNull(Display *d){
	return !(d);
}

extern int DetectWM();
extern int ErrorHandler();
extern int EventLoop(Display *display_);
extern char wm_detect;
*/
import "C"
import (
	"errors"
	"fmt"
	"unsafe"
)

var c_int_size int

var window_manager_xlib_num uint16

type WindowManager struct {
	Display   *C.Display
	Wm_detect *C.char

	clients map[C.Window]C.Window
}

func NewWindowManager(display_str string) (*WindowManager, error) {
	W := &WindowManager{}
	display_c_str := C.CString(display_str)
	//C.XtToolkitInitialize()
	if display_str == "" {
		W.Display = C.XOpenDisplay((*C.char)(C.NULL))
	} else {
		W.Display = C.XOpenDisplay(display_c_str)
	}

	b := C.IsDisplayNull(W.Display)
	fmt.Println(b)
	if *(*bool)(unsafe.Pointer(&b)) {
		a := C.XDisplayName(display_c_str)
		defer C.free(unsafe.Pointer(a))
		return nil, errors.New("Failed to open X display " + C.GoString(a))
	}
	C.free(unsafe.Pointer(display_c_str))

	window_manager_xlib_num += 1

	return W, nil
}

func (W *WindowManager) Close() {
	window_manager_xlib_num -= 1
	C.XCloseDisplay(W.Display)
}

func (W *WindowManager) Run() {

	C.DetectWM()
	C.XSelectInput(W.Display, C.XDefaultRootWindow(W.Display), C.SubstructureRedirectMask|C.SubstructureNotifyMask)
	C.XGrabKey(W.Display, 124, C.AnyModifier, C.XDefaultRootWindow(W.Display), 0x00, C.GrabModeAsync, C.GrabModeAsync)
	C.XAllowEvents(W.Display, C.ReplayPointer, C.CurrentTime)
	C.XSync(W.Display, 0x00)
	wm_detect := C.wm_detect
	C.wm_detect = 0x00
	if *(*bool)(unsafe.Pointer(&wm_detect)) {
		if !Testing {
			panic("Detected another window manager on display")
		}
	}
	C.ErrorHandler()

	//C.XGrabServer(W.Display)

	C.EventLoop(W.Display)

}

//export GoSend
func GoSend(msg *C.char) {
	switch v := C.GoString(msg); v {
	case "key poweroff":
		BusSend(v)
		//C.free(unsafe.Pointer(msg))
	}
}
