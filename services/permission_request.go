package services

import (
	"bufio"
	"net"
	"os"
)

func New_permission_request_service() {
	os.Remove("/tmp/permissionRequestService")
	l, err := net.Listen("unix", "/tmp/permissionRequestService")
	if err != nil {
		return
	}
	defer l.Close()
	for {
		conn, err := l.Accept()
		if err != nil {
			continue
		}
		go func(conn net.Conn) {
			defer conn.Close()
			bufio.NewReader(conn)
		}(conn)
	}
}
