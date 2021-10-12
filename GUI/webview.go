// +build webview

package GUI

import (
	"net/http"

	"embed"

	"github.com/webview/webview"
)

//go:embed static
var statics embed.FS

func NewWebviewWindow() {
	fs := http.FileServer(http.FS(statics))
	http.Handle("/", fs)
	go http.ListenAndServe("127.0.0.1:2789", nil)

	w := webview.New(true)
	w.Navigate("http://localhost:2789/html/home.html")
	w.Run()
}
