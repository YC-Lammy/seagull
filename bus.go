package main

import (
	"net"
)

type bus_msg struct {
	From  string
	Data  []byte
	Event string // notify, powerbtn,
	To    string
}

var bus = make(chan []byte)

func init() {
	l, _ := net.Listen("unix", "/tmp/.seagull/uibus")
	go func() {
		c, _ := l.Accept()
		for {
			msg := <-bus
			c.Write(msg)
		}
	}()
}

func busSend(msg []byte) {
	bus <- msg
}
