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
	Freq      string
	Rate      string
	Signal    uint8
	Security  string
	WPA       string
	RSN       string
	Connected bool
}

type Wwan struct {
	APN string
}

var wifi_interfaces = []string{}

func ScanWifi() ([]Wifi, error) {

	r := bytes.NewBuffer([]byte{})
	cmd := exec.Command("nmcli", "-f",
		"SSID,BSSID,FREQ,RATE,SIGNAL,SECURITY,WPA-FLAGS,RSN-FLAGS,IN-USE",
		"--terse", "-c", "no", "dev", "wifi")
	cmd.Stdout = r
	err := cmd.Run()
	if err != nil {
		return nil, err
	}
	var wifis = []Wifi{}
	lines := strings.Split(r.String(), "\n")
	for _, line := range lines {
		line = strings.ReplaceAll(line, `\:`, ".")
		f := strings.Split(line, ":")

		if len(f) != 9 {
			continue
		}

		var strength int
		strength, err = strconv.Atoi(f[4])
		if err != nil {
			continue
		}

		wifis = append(wifis, Wifi{
			SSID:      f[0],
			BSSID:     f[1],
			Freq:      f[2],
			Rate:      f[3],
			Signal:    uint8(strength),
			Security:  f[5],
			WPA:       f[6],
			RSN:       f[7],
			Connected: f[8] == "*",
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
