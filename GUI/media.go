package GUI

/*
#cgo pkg-config: gtk4

*/
import "C"
import (
	"fmt"
	"os"

	"github.com/godbus/dbus/v5"
)

type MediaPlayer struct {
	ID             string
	Title          string
	Icon           interface{}
	Length         int64
	PlaybackStatus string

	dest string
	conn dbus.BusObject
}

var mediaBus *dbus.Conn

var mediaPlayerID uint64

var mediaPlayerStore map[string]MediaPlayer

func NewMediaPlayer(dest string) {
	var player MediaPlayer
	player.conn = mediaBus.Object(dest, "/org/mpris/MediaPlayer2")
	if v, err := player.conn.GetProperty("PlaybackStatus"); err != nil {
		if v, ok := v.Value().(string); ok {
			player.PlaybackStatus = v
		}
	}
	if v, err := player.conn.GetProperty("Metadata"); err != nil {
		if v, ok := v.Value().(map[string]interface{}); ok {
			if l, ok := v["mpris:length"]; ok {
				if l, ok := l.(int64); ok {
					player.Length = l
				}
			}
			if l, ok := v["xesam:title"]; ok {
				if l, ok := l.(string); ok {
					player.Title = l
				}
			}
			if l, ok := v["xesam:artist"]; ok {
				if l, ok := l.([]string); ok && len(l) >= 1 {
					player.Title = l[0]
				}
			}
		}
	}
}

func (m *MediaPlayer) Next() {
	m.conn.Call("org.mpris.MediaPlayer2.Player.Next", 0)
}

func (m *MediaPlayer) Previous() {
	m.conn.Call("org.mpris.MediaPlayer2.Player.Previous", 0)
}

func (m *MediaPlayer) PlayPause() {
	m.conn.Call("org.mpris.MediaPlayer2.Player.PlayPause", 0)
}

func (m *MediaPlayer) Stop() {
	m.conn.Call("org.mpris.MediaPlayer2.Player.Stop", 0)
}

func (m *MediaPlayer) Play() {
	m.conn.Call("org.mpris.MediaPlayer2.Player.Play", 0)
}

//export PlayerIsConnected
func PlayerIsConnected(playername *C.char) C.char {
	if player, ok := mediaPlayerStore[C.GoString(playername)]; ok {
		_, err := player.conn.GetProperty("PlaybackStatus")
		if err != nil {
			return 0x01
		}
	}
	return 0x00
}

//export PlayerIsPaused
func PlayerIsPaused(name *C.char) C.char {
	if player, ok := mediaPlayerStore[C.GoString(name)]; ok {
		v, _ := player.conn.GetProperty("PlaybackStatus")
		if v.String() != "Playing" {
			return 0x01
		}
	}
	return 0x00
}

//export PlayerPosition
func PlayerPosition(name *C.char) C.longlong {
	if player, ok := mediaPlayerStore[C.GoString(name)]; ok {
		v, _ := player.conn.GetProperty("Position")
		if t, ok := v.Value().(int64); ok {
			return C.longlong(t)
		}
	}
	return 0
}

func init() {
	go func() {
		bus, _ := dbus.SessionBus()
		rules := []string{
			"type='signal',member='NameAcquired'",
			"type='signal',member='NameLost'",
		}
		var flag uint = 0
		call := bus.BusObject().Call("org.freedesktop.DBus.Monitoring.BecomeMonitor", 0, rules, flag)
		if call.Err != nil {
			fmt.Fprintln(os.Stderr, "Failed to become monitor:", call.Err)
			os.Exit(1)
		}
		sig_chan := make(chan *dbus.Message, 10)
		bus.Eavesdrop(sig_chan)
		for {
			msg := <-sig_chan
			if len(msg.Body) == 1 {
				if iface, ok := msg.Body[0].(string); ok {
					if len(iface) > 23 && iface[:24] == "org.mpris.MediaPlayer2." {
						NewMediaPlayer(iface)
					}
				}
			}
			fmt.Println(msg.Body)
		}
	}()
}
