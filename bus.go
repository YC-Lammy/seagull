package main

import (
	"net"
	"os"
)

type bus_msg struct {
	From  string
	Data  []byte
	Event string // notify, powerbtn,
	To    string
}

var bus = make(chan []byte)

func init() {
	os.Mkdir("/tmp/.seagull", os.ModePerm)
	l, err := net.Listen("unix", "/tmp/.seagull/uibus")
	if err != nil {
		panic(err)
	}
	go func() {
		defer l.Close()
		defer os.Remove("/tmp/.seagull/uibus")
		c, err := l.Accept()
		if err != nil {
			panic(err)
		}
		for {
			msg := <-bus
			c.Write(msg)
		}
	}()
}

func busSend(msg []byte) {
	bus <- msg
}
