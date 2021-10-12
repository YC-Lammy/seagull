package main

import (
	"flag"
	"fmt"
	"os"
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
	w.Run()
}
