package services

import "github.com/godbus/dbus/v5"

func init() {
	bus, _ := dbus.SessionBus()
}
