package GUI

/*

#cgo pkg-config: gtk4
#cgo CFLAGS: -g -Wall -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED

#include "gtkmobile.h"
#include<gtk/gtk.h>
#include<stdlib.h>
#include <stdio.h>
*/
import "C"
import "unsafe"

func NewGtkWindow() {
	C.gtkmobile()
}

func GUISend(msg string) {
	s := C.CString(msg)
	C.recvEvent(s)
	C.free(unsafe.Pointer(s))
}
