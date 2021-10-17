package main

type permission struct {
	enable  func(App)
	disable func(App)
}

type settings struct {
	Network struct {
		Wifi struct {
			Enabled bool

			on      func()
			off     func()
			connect func(Wifi)
		}
		Mobile struct {
			Enabled bool
			on      func()
			off     func()
		}
		Aeroplane bool
		Hotspot   struct {
			Name     string
			Password string
		}
		Tethering struct{}
		VPN       struct{}
		DNS       string
	}
	Apps struct {
		app_info    struct{}
		permissions struct {
			Networking                  struct{}
			Audio                       struct{}
			Camera                      struct{}
			Bluetooth                   struct{}
			Files                       struct{}
			Notification                struct{}
			Contacts                    struct{}
			Location                    struct{}
			GPU_acceleration            struct{}
			Interprocess_communications struct{}
			Background                  struct{}
		}
		Default struct {
			Browser string
			File    string
			Phone   string
			SMS     string
			Email   string
		}
	}
	Notification struct {
		Do_not_disturb bool
	}
	Battery struct {
		DimScreen uint8
		Sleep     uint8
	}
}
