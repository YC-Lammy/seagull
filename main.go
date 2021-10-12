package main

import (
	"flag"
	"fmt"
	"os"
    "GUI"
)

var Testing bool

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
