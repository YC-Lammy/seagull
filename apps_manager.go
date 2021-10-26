package main

import "os/exec"

var apps = map[string]app{}

var launched_apps = map[string]*app{}

var currentFocus *app

type app struct {
	name         string
	view         View
	icon_path    string
	exec         string
	desktop_path string
}

func init() {}

func Launchapp(name string) {
	if app, ok := apps[name]; ok {
		if lapp, ok := launched_apps[name]; ok {
			FocusApp(lapp.view)
		} else {
			cmd := exec.Command(app.exec)
			err := cmd.Start()
			if err != nil {
				return
			}
			launched_apps[name] = &app
		}
	}
}
