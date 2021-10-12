module seagull

go 1.16

require (
	github.com/fsnotify/fsnotify v1.5.1 // indirect
	github.com/gotk3/gotk3 v0.6.1
	github.com/webview/webview v0.0.0-20210330151455-f540d88dde4e // indirect
	GUI v0.0.0
)

replace(
	GUI => ./GUI
)