package GUI

type Notify struct {
	title          string
	body           string
	icon           interface{}
	priority       string
	defaut_actions string
}

type ActionInvoke struct {
	Id        string
	Action    string
	Parameter []interface{}
}

var notifyID uint64

var NotifyStore map[uint64]Notify

func NewNotify(title string, body string, icon interface{},
	priority string, defaut_actions string) {
	//control if notification is allowed on screen
}
