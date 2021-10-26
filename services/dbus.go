package services

import "github.com/godbus/dbus/v5"

const (
	PROCESS_SUCESS uint32 = iota
)

func init() {
	bus, _ := dbus.SessionBus()

	bus.ExportMethodTable(map[string]interface{}{
		"Query": Sqlite_query,
	}, "org.seagull.Seagull.Sqlite", "/org/seagull/Seagull")

	signal_chan := make(chan *dbus.Signal, 100)
	bus.Signal(signal_chan)
}

func Sqlite_query(sql string) (uint32, map[string][]interface{}, *dbus.Error) {
	return 0, nil
}
