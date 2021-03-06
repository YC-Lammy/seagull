package main

/*
#cgo pkg-config: wayland-server xkbcommon pixman-1 wlroots glesv2
#cgo CFLAGS: -g -Werror -I/usr/local/include -DWLR_USE_UNSTABLE

#include "window_manager_wlroots.h"
#include <pixman.h>
*/
import "C"
import (
	"os"
	"unsafe"
)

var (
	nullptr *C.char = (*C.char)(unsafe.Pointer(uintptr(0x00)))
)

type View *C.view

type Window_manager_wlroots struct {
	apps map[string]*C.view
}

func NewWindowManager() (wm *Window_manager_wlroots, err error) {
	os.Setenv("LD_LIBRARY_PATH", os.Getenv("LD_LIBRARY_PATH")+":/usr/local/lib64")
	return
}

func (*Window_manager_wlroots) Run() {
	C.Cmain()
}

func FocusApp(View) {

}
