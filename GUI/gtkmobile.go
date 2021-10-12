package GUI

/*

#cgo pkg-config: gtk4
#cgo CFLAGS: -g -Wall -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED

#include "gtkmobile.h"
#include<gtk/gtk.h>
*/
import "C"

func NewGtkWindow() {
	C.gtkmobile()
}
