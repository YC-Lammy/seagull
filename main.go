package main

import (
	"GUI"
	"flag"
	"fmt"
	"os"
)

var Testing bool
var isGUI bool

func init() {
	fmt.Println("Seagull start")
	flag.BoolVar(&Testing, "test", false, "")
	flag.BoolVar(&isGUI, "gui", false, "")
	flag.Parse()

	os.Setenv("XDG_SESSION_DESKTOP", "Seagull")
}

func main() {
	if isGUI {
		GUI.NewGtkWindow()
		return
	}
	w, err := NewWindowManager("")
	if err != nil {
		panic(err)
	}
	go w.Run()
	GUI.NewGtkWindow()
}
