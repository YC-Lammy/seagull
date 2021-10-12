#include"window_manager_xlib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <time.h>
#include <sys/socket.h>


pthread_mutex_t error_lock;

char wm_detect=0;

int OnWMDetect(Display *d, XErrorEvent *e){
	wm_detect=1;
	return 0;
}

int DetectWM(){
	XSetErrorHandler(OnWMDetect);
	return 0;
}

int OnXError(Display* display, XErrorEvent *e) {
  const int MAX_ERROR_TEXT_LENGTH = 1024;
  char error_text[MAX_ERROR_TEXT_LENGTH];
  XGetErrorText(display, e->error_code, error_text, sizeof(error_text));
  // The return value is ignored.
  return 0;
}


int ErrorHandler(){
	XSetErrorHandler(OnXError);
	return 0;
}

int SizeofInt(){
    int a;
    return sizeof(a);
}

int Frame(Display *display_, Window w) {
  // Visual properties of the frame to create.

  //const unsigned int BORDER_WIDTH = 0;
  //const unsigned long BORDER_COLOR = 0xff0000;
  //const unsigned long BG_COLOR = 0x0000ff;

  // We shouldn't be framing windows we've already framed.
  //CHECK(!clients_.count(w));

  // 1. Retrieve attributes of window to frame.
  XWindowAttributes x_window_attrs;
  XGetWindowAttributes(display_, w, &x_window_attrs);

  // 3. Create frame.
  //const Window frame = XCreateSimpleWindow(
      //display_,
      //root_,
      //x_window_attrs.x,
      //x_window_attrs.y,
      //x_window_attrs.width,
      //x_window_attrs.height,
      //BORDER_WIDTH,
      //BORDER_COLOR,
      //BG_COLOR);
  // 4. Select events on frame.
  XSelectInput(
      display_,
      w,
      SubstructureRedirectMask | SubstructureNotifyMask );

  // 5. Add client to save set, so that it will be restored and kept alive if we
  // crash.
  XAddToSaveSet(display_, w);

  XMapWindow(display_, w);
  // 8. Save frame handle.
  //clients_[w] = frame;
  // 9. Grab universal window management actions on client window.
  //   a. Move windows with alt + left button.
  XGrabButton(
      display_,
      Button1,
      Mod1Mask,
      w,
      false,
      ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
      GrabModeAsync,
      GrabModeAsync,
      None,
      None);

  XGrabButton(
      display_,
      Button1,
      AnyModifier,
      w,
      false,
      ButtonPressMask,
      GrabModeSync,
      GrabModeAsync,
      None,
      None);
  //   b. Resize windows with alt + right button.
  XGrabButton(
      display_,
      Button3,
      Mod1Mask,
      w,
      false,
      ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
      GrabModeAsync,
      GrabModeAsync,
      None,
      None);
  //   c. Kill windows with alt + f4.
  XGrabKey(
      display_,
      XKeysymToKeycode(display_, XK_F4),
      Mod1Mask,
      w,
      false,
      GrabModeAsync,
      GrabModeAsync);
  //   d. Switch windows with alt + tab.
  XGrabKey(
      display_,
      XKeysymToKeycode(display_, XK_Tab),
      Mod1Mask,
      w,
      false,
      GrabModeAsync,
      GrabModeAsync);

  //LOG(INFO) << "Framed window " << w << " [" << frame << "]";
}

int Unframe(Display *display_, Window w) {
  //CHECK(clients_.count(w));

  // We reverse the steps taken in Frame().
  //const Window frame = clients_[w];
  // 1. Unmap frame.
  //XUnmapWindow(display_, frame);
  // 2. Reparent client window.
  //XReparentWindow(
      //display_,
      //w,
      //root_,
      //0, 0);  // Offset of client window within root.
  // 3. Remove client window from save set, as it is now unrelated to us.
  XRemoveFromSaveSet(display_, w);
  // 4. Destroy frame.
  //XDestroyWindow(display_, frame);

  // 5. Drop reference to frame handle.
  //clients_.erase(w);
}

int OnCreateNotify(Display *display_, XCreateWindowEvent e) {}

int OnDestroyNotify(Display *display_,XDestroyWindowEvent e) {}

int OnReparentNotify(Display *display_, XReparentEvent e) {}

int OnMapNotify(Display *display_, XMapEvent e) {}

int OnUnmapNotify(Display *display_,XUnmapEvent e) {
  // If the window is a client window we manage, unframe it upon UnmapNotify. We
  // need the check because we will receive an UnmapNotify event for a frame
  // window we just destroyed ourselves.
  //if (!clients_.count(e.window)) {
  //  LOG(INFO) << "Ignore UnmapNotify for non-client window " << e.window;
  //  return;
  //}

  // Ignore event if it is triggered by reparenting a window that was mapped
  // before the window manager started.
  //
  // Since we receive UnmapNotify events from the SubstructureNotify mask, the
  // event attribute specifies the parent window of the window that was
  // unmapped. This means that an UnmapNotify event from a normal client window
  // should have this attribute set to a frame window we maintain. Only an
  // UnmapNotify event triggered by reparenting a pre-existing window will have
  // this attribute set to the root window.
  if (e . event == DefaultRootWindow(display_)) {
    //LOG(INFO) << "Ignore UnmapNotify for reparented pre-existing window "
    //          << e.window;
    return 1;
  }

  Unframe(display_,e.window);
}

int OnConfigureNotify(Display *display_,const XConfigureEvent e) {}

int OnMapRequest(Display *display_,XMapRequestEvent e) {
  // 1. Frame or re-frame window.
  Frame(display_, e.window);
  // 2. Actually map window.
  XMapWindow(display_, e.window);
}

int OnConfigureRequest(Display *display_,XConfigureRequestEvent e) {
  XWindowChanges changes;
  changes.x = e . x;
  changes.y = e . y;
  changes.width = e.width;
  changes.height = e.height;
  changes.border_width = e.border_width;
  changes.sibling = e.above;
  changes.stack_mode = e.detail;
  //if (clients_.count(e.window)) {
    //const Window frame = clients_[e.window];
    //XConfigureWindow(display_, frame, e.value_mask, &changes);
    //LOG(INFO) << "Resize [" << frame << "] to " << Size<int>(e.width, e.height);
  //}
  XConfigureWindow(display_, e.window, e.value_mask, &changes);
  //LOG(INFO) << "Resize " << e.window << " to " << Size<int>(e.width, e.height);
}

int OnButtonPress(Display *display_,XButtonEvent e) {
  if (e.button == Button1){
    XAllowEvents(display_, ReplayPointer, e.time);
	  XSync(display_, 0x00);
  }
  //CHECK(clients_.count(e.window));
  //const Window frame = clients_[e.window];

  // 1. Save initial cursor position.
  //drag_start_pos_ = Position<int>(e.x_root, e.y_root);

  // 2. Save initial window info.
  Window returned_root;
  int x, y;
  unsigned width, height, border_width, depth;
  XGetGeometry(
      display_,
      e.window,
      &returned_root,
      &x, &y,
      &width, &height,
      &border_width,
      &depth);
  //drag_start_frame_pos_ = Position<int>(x, y);
  //drag_start_frame_size_ = Size<int>(width, height);

  // 3. Raise clicked window to top.
  XRaiseWindow(display_, e.window);
  XSetInputFocus(display_, e.window, RevertToPointerRoot, CurrentTime);
}

int OnButtonRelease(Display *display_,XButtonEvent e) {
  //XRaiseWindow(display_, e.window);
  //XSetInputFocus(display_, e.window, RevertToPointerRoot, CurrentTime);
}

int OnMotionNotify(Display *display_,XMotionEvent e) {}


struct timespec power_btn_press_time;

int OnKeyPress(Display *display_, XKeyEvent e) {
  XRaiseWindow(display_, e.window);
  XSetInputFocus(display_, e.window, RevertToPointerRoot, CurrentTime);
  if ((e.state & Mod1Mask) &&
      (e.keycode == XKeysymToKeycode(display_, XK_F4))) {
    // alt + f4: Close window.
    //
    // There are two ways to tell an X window to close. The first is to send it
    // a message of type WM_PROTOCOLS and value WM_DELETE_WINDOW. If the client
    // has not explicitly marked itself as supporting this more civilized
    // behavior (using XSetWMProtocols()), we kill it with XKillClient().
    Atom* supported_protocols;
    Atom  WM_DELETE_WINDOW= XInternAtom(display_, "WM_DELETE_WINDOW", false);
    int num_supported_protocols;
    if (XGetWMProtocols(display_,
                        e.window,
                        &supported_protocols,
                        &num_supported_protocols) //&&
        //(find(supported_protocols,
                     //supported_protocols + num_supported_protocols,
                     //WM_DELETE_WINDOW) != supported_protocols + num_supported_protocols)
        ){
      //LOG(INFO) << "Gracefully deleting window " << e.window;
      // 1. Construct message.
      XEvent msg;
      memset(&msg, 0, sizeof(msg));
      msg.xclient.type = ClientMessage;
      msg.xclient.message_type = XInternAtom(display_, "WM_PROTOCOLS", false);
      msg.xclient.window = e.window;
      msg.xclient.format = 32;
      msg.xclient.data.l[0] = WM_DELETE_WINDOW;
      // 2. Send message to window to be closed.
      XSendEvent(display_, e.window, false, 0, &msg);
    } else {
      //LOG(INFO) << "Killing window " << e.window;
      XKillClient(display_, e.window);
    }
  } else if ((e.state & Mod1Mask) &&
             (e.keycode == XKeysymToKeycode(display_, XK_Tab))) {

    // alt + tab: Switch window.
    // 1. Find next window.
    //auto i = clients_.find(e.window);
    //CHECK(i != clients_.end());
    //++i;
    //if (i == clients_.end()) {
    //  i = clients_.begin();
    //}
    // 2. Raise and set focus.
    //XRaiseWindow(display_, i->second);
    //XSetInputFocus(display_, i->first, RevertToPointerRoot, CurrentTime);
  } 
  else if (e.keycode == 124){
    timespec_get(&power_btn_press_time,TIME_UTC);
  }
}

int OnKeyRelease(Display *display_, XKeyEvent e) {
  //XRaiseWindow(display_, e.window);
  //XSetInputFocus(display_, e.window, RevertToPointerRoot, CurrentTime);
  if (e.keycode == 124){
    // shutdown page or sleep directly
    struct timespec now;
    timespec_get(&now,TIME_UTC);
    if ((((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000)-(((int64_t) power_btn_press_time.tv_sec) * 1000 + ((int64_t) power_btn_press_time.tv_nsec) / 1000000)>1500){

    }
    else{
      system("");
    }
  
    printf("power clicked");
  }
}

int EventLoop(Display *display_){
	for (;;) {
		XEvent e;
		XNextEvent(display_, &e);

		switch (e.type) {

		case CreateNotify:
			OnCreateNotify(display_,e.xcreatewindow);
			break;
		case DestroyNotify:
			OnDestroyNotify(display_,e.xdestroywindow);
			break;
		case ReparentNotify:
			OnReparentNotify(display_,e.xreparent);
			break;
		case MapNotify:
			OnMapNotify(display_,e.xmap);
			break;
		case UnmapNotify:
			OnUnmapNotify(display_,e.xunmap);
			break;
		case ConfigureNotify:
			OnConfigureNotify(display_,e.xconfigure);
			break;
		case MapRequest:
			OnMapRequest(display_,e.xmaprequest);
			break;
		case ConfigureRequest:
			OnConfigureRequest(display_,e.xconfigurerequest);
			break;
		case ButtonPress:
			OnButtonPress(display_,e.xbutton);
			break;
		case ButtonRelease:
			OnButtonRelease(display_,e.xbutton);
			break;
		case MotionNotify:
			// Skip any already pending motion events.
			while (XCheckTypedWindowEvent(
				display_, e.xmotion.window, MotionNotify, &e)) {}
			OnMotionNotify(display_,e.xmotion);
			break;
		case KeyPress:
			OnKeyPress(display_,e.xkey);
			break;
		case KeyRelease:
			OnKeyRelease(display_,e.xkey);
			break;

    	}
	}
}
