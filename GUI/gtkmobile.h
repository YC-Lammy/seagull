#ifndef _GTKMOBILE_H
#define _GTKMOBILE_H

#include <gtk/gtk.h>

typedef struct app_ {
  char *name;
  char *exec;
  char *icon_path;
  int x; //position in the grid
  int y;
  GtkWidget *widget;

} application;

extern char done;
extern int long_click_ms;
extern int drag_offset;
extern void NewApp(application *app );
extern void recvEvent(char *event);
extern int gtkmobile();

#endif
