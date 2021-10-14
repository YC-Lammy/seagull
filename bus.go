package main

import "GUI"

/*
Bus is the central data communiction between C and go
It connects different components
*/

type Bus_msg struct {
	From  string
	Event string // notify, powerbtn,
	To    string
	Data  []byte
}

var bus = make(chan string)

func init() {
	go func() {
		for {
			msg := <-bus
			GUI.GUISend(msg)
		}
	}()
}

func BusSend(msg string) {
	bus <- msg
}
