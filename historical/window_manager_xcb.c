//+build xcb

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <xcb/randr.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_xrm.h>
#include <X11/keysym.h>

//#include "_cgo_export.h"
struct item
{
    void *data;
    struct item *prev;
    struct item *next;
};

///---Internal Constants---///
///---Globals---///
static xcb_generic_event_t *ev  = NULL;
static void (*events[XCB_NO_OPERATION])(xcb_generic_event_t *e);
static unsigned int numlockmask = 0;
static bool is_sloppy = true;              // by default use sloppy focus
int sigcode = 0;                           // Signal code. Non-zero if we've been interruped by a signal.
xcb_connection_t *conn = NULL;             // Connection to X server.
xcb_ewmh_connection_t *ewmh = NULL;        // Ewmh Connection.
xcb_screen_t     *screen = NULL;           // Our current screen.
int randrbase = 0;                         // Beginning of RANDR extension events.
const int WORKSPACES =10;
static uint8_t curws = 0;                  // Current workspace.
struct client *focuswin = NULL;            // Current focus window.
static xcb_drawable_t top_win=0;           // Window always on top.
static xcb_drawable_t dock_win=0;          // A single dock always on top.
static struct item *winlist = NULL;        // Global list of all client windows.
static struct item *monlist = NULL;        // List of all physical monitor outputs.
static struct item *wslist[WORKSPACES];
///---Global configuration.---///

void
cleanup(void)
{
    free(ev);
    if(monlist)
        delallitems(&monlist,NULL);
    struct item *curr,*wsitem;
    for(int i = 0; i < WORKSPACES; i++){
        if(!wslist[i])
            continue;
        curr = wslist[i];
        while(curr){
            wsitem = curr;
            curr = curr->next;
            free(wsitem);
        }
    }
    if(winlist){
        delallitems(&winlist,NULL);
    }
    if (ewmh != NULL){
	    xcb_ewmh_connection_wipe(ewmh);
		free(ewmh);
    }
    if(!conn){
        return;
    }
	xcb_set_input_focus(conn, XCB_NONE,XCB_INPUT_FOCUS_POINTER_ROOT,
			XCB_CURRENT_TIME);
	xcb_flush(conn);
	xcb_disconnect(conn);
}

void
sigcatch(const int sig)
{
	sigcode = sig;
}

void
install_sig_handlers(void)
{
	struct sigaction sa;
	struct sigaction sa_chld;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_NOCLDSTOP;
	//could not initialize signal handler
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
		exit(-1);
	sa.sa_handler = sigcatch;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart if interrupted by handler */
	if ( sigaction(SIGINT, &sa, NULL) == -1
		|| sigaction(SIGHUP, &sa, NULL) == -1
		|| sigaction(SIGTERM, &sa, NULL) == -1)
		exit(-1);
}

int
main(int argc, char **argv)
{
	int scrno = 0;
	atexit(cleanup);
	install_sig_handlers();
	if (!xcb_connection_has_error(conn = xcb_connect(NULL, &scrno)))
		if (setup(scrno))
			run();
	/* the WM has stopped running, because sigcode is not 0 */
	exit(sigcode);
}