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
	flag.Parse()

	os.Setenv("XDG_SESSION_DESKTOP", "Seagull")
}

func main() {
	w, err := NewWindowManager("")
	if err != nil {
		panic(err)
	}
	go w.Run()
	GUI.NewGtkWindow()
}
