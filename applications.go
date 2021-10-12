package main

import (
	"log"
	"os"
	"path"

	"github.com/fsnotify/fsnotify"
)

type application struct {
	name string
	icon string
	exec string
}

func init() {
	var err error = nil
	var watcher *fsnotify.Watcher
	watcher, err = fsnotify.NewWatcher()
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
				log.Println("event:", event)
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

	h, _ := os.UserHomeDir()
	watcher.Add(path.Join(h, ".local/share/flatpak/exports/share/applications/"))
}
