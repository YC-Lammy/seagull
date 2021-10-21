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
	crash := make(chan byte, 10)
	w, _ := NewWindowManager()
	crash <- 0x00
	crash <- 0x01
	for {
		select {
		case b := <-crash:
			switch b {
			case 0x00:
				go func() {
					w.Run()
					crash <- 0x00
				}()
			case 0x01:
				go func() {
					GUI.NewGtkWindow()
					crash <- 0x01
				}()
			}
		}
	}
}
