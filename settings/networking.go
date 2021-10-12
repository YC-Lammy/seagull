package settings

import (
	"bytes"
	"os/exec"
)

type Wifi struct {
	SSID     string
	BSSID    string
	Rate     uint32
	Signal   uint8
	Security string
}

type Wwan struct {
	APN string
}

var wifi_interfaces = []string{}

func ScanWifi() ([]Wifi, error) {

	r := bytes.NewBuffer([]byte{})
	cmd := exec.Command("nmcli", "dev", "wifi")
	cmd.Stdout = r
	err := cmd.Run()
	if err != nil {
		return nil, err
	}
	f, err := r.ReadBytes('\n')
	if err != nil {
		return nil, err
	}

	indexes := []int{}
	b := false
	for i, k := range f {
		if k != ' ' && !b {
			indexes = append(indexes, i)
			b = true
		} else if k == ' ' && b {
			indexes = append(indexes, i)
			b = false
		}
	}
	return nil, nil
}

func AirplaneMode(enable bool) {
	var cmd *exec.Cmd
	if enable {
		cmd = exec.Command("nmcli", "radio", "off")
	} else {
		cmd = exec.Command("nmcli", "radio", "on")
	}
	cmd.Start()
}
