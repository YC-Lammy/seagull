package GUI

import (
	"io"
	"os/exec"
	"os/user"
)

func Authenticate(pwd string) error {
	u, _ := user.Current()
	c := exec.Command("su", u.Username)
	stdin, _ := c.StdinPipe()
	go func() {
		defer stdin.Close()
		io.WriteString(stdin, pwd+"\n")
	}()

	return c.Run()
}
