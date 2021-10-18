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
import (
	"io/ioutil"
	"log"
	"os"
	"path"
	"strings"
	"unsafe"

	"github.com/fsnotify/fsnotify"
	"github.com/fxamacker/cbor/v2"
)

var apps = map[string]*C.application{}

var app_layout = map[string][2]uint8{}

func NewGtkWindow() {
	C.gtkmobile()
}

func GUISend(msg string) {
	s := C.CString(msg)
	C.recvEvent(s)
	C.free(unsafe.Pointer(s))
}

func init() {
	os.MkdirAll("/usr/share/seagull/apps", os.ModePerm)
	f, err := os.Open("/usr/share/seagull/apps/layout")
	if err != nil {
		f, _ = os.Create("/usr/share/seagull/apps/layout")
		return
	}
	b, _ := ioutil.ReadAll(f)
	cbor.Unmarshal(b, &app_layout)
}

func init() {
	watcher, err := fsnotify.NewWatcher()
	for err != nil {
		watcher, err = fsnotify.NewWatcher()
	}
	go func() {
		for {
			select {
			case event, ok := <-watcher.Events:
				if !ok {
					return
				}
				if event.Op&fsnotify.Write == fsnotify.Write {
					log.Println("modified file:", event.Name)
				}
			case err, ok := <-watcher.Errors:
				if !ok {
					return
				}
				log.Println("error:", err)
			}
		}
	}()

	usrs, _ := ioutil.ReadDir("/home/")
	for _, usr := range usrs {
		if !usr.IsDir() {
			continue
		}
		pat := path.Join("/home", usr.Name(), ".local/share/flatpak/exports/share/applications/")

		fs, err := ioutil.ReadDir(pat)
		if err != nil {
			continue
		}
		for _, d := range fs {
			if d.IsDir() {
				continue
			}
			n := d.Name()
			if strings.HasSuffix(n, ".desktop") {
				app := C.application{}
				exec := path.Join(pat, n)
				app.exec = C.CString(exec)
				f, _ := os.Open(exec)
				b, _ := ioutil.ReadAll(f)
				var name string
				for _, n := range strings.Split(string(b), "\n") {
					if n[:5] == "Name" {
						name = strings.Split(n, "=")[1]
					}
				}

			}
		}
		watcher.Add(pat)
	}

	//app := C.application{}
}
