package main

import (
	"bytes"
	"os/exec"
	"strconv"
	"strings"
)

type Wifi struct {
	SSID      string
	BSSID     string
	Rate      string
	Signal    uint8
	Security  string
	Connected bool
}

type Wwan struct {
	APN string
}

var wifi_interfaces = []string{}

func ScanWifi() ([]Wifi, error) {

	r := bytes.NewBuffer([]byte{})
	cmd := exec.Command("nmcli", "--terse", "-c", "no", "dev", "wifi")
	cmd.Stdout = r
	err := cmd.Run()
	if err != nil {
		return nil, err
	}
	var wifis = []Wifi{}
	for {
		var b []byte
		b, err = r.ReadBytes('\n')
		if err != nil {
			break
		}
		var line = string(b)
		line = strings.ReplaceAll(line, `\:`, ".")
		f := strings.Split(line, ":")

		var strength int
		strength, err = strconv.Atoi(f[6])
		if err != nil {
			continue
		}

		wifis = append(wifis, Wifi{
			Connected: f[0] != "*",
			BSSID:     f[1],
			SSID:      f[2],
			Rate:      f[5],
			Signal:    uint8(strength),
			Security:  f[8],
		})
	}

	return wifis, nil
}

func network_wifi_on(t bool) {
	var cmd *exec.Cmd
	if t {
		cmd = exec.Command("nmcli", "radio", "wifi", "on")
	} else {
		cmd = exec.Command("nmcli", "radio", "wifi", "off")
	}
	cmd.Run()
}

func network_wwan_on(t bool) {
	var cmd *exec.Cmd
	if t {
		cmd = exec.Command("nmcli", "radio", "wwan", "on")
	} else {
		cmd = exec.Command("nmcli", "radio", "wwan", "off")
	}
	cmd.Run()
}

func network_airplaneMode(enable bool) {
	var cmd *exec.Cmd
	if enable {
		cmd = exec.Command("nmcli", "radio", "off")
	} else {
		cmd = exec.Command("nmcli", "radio", "on")
	}
	cmd.Start()
}
