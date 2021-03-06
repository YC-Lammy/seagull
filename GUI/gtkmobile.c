#include"gtkmobile.h"
#include <time.h>
#include <gtk/gtk.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int long_click_ms = 800; // long click duration
int drag_offset = 5; // dragging offset

GtkWidget *window;
int window_height;
int window_width;

GtkWidget *scroll_window;
GtkWidget *grid;
GtkWidget *overlay;
GtkWidget *capturer;

int total_pages;
int current_page; // 0,1,2...

/////////////////////////////////////////////
//         application                     //
/////////////////////////////////////////////

application apps[150];


void NewApp(application *app){
}


// slide bar and app panel is in the same scroll area
GtkWidget *panel_slide_scroll_window; 
GtkAdjustment *panel_slide_adj;
//GtkWidget *panel_slide_fixed;
GtkWidget *panel_slide_box;
GtkWidget *panel;
GtkWidget *slide;

static GtkWidget * createPanel();
static GtkWidget * createSlide();

static void createPanel_slide(){
  GdkRectangle workarea = {0};

  GListModel *l = gdk_display_get_monitors(gdk_display_get_default());
  GdkMonitor *m=g_list_model_get_item(l,0);
  gdk_monitor_get_geometry(m,&workarea);
  window_width = workarea.width;
  window_height = workarea.height;

  panel_slide_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_box_set_homogeneous(GTK_BOX(panel_slide_box),true);
  gtk_widget_set_size_request(panel_slide_box,window_width,window_height*3);

  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(panel_slide_scroll_window),panel_slide_box);

  slide = createSlide();
  gtk_box_append(GTK_BOX(panel_slide_box),slide);

  GtkWidget *blank = gtk_label_new("hello");
  gtk_widget_set_size_request(blank,window_width,window_height);
  gtk_widget_set_hexpand(blank, true);
  gtk_widget_set_vexpand(blank, true);
  gtk_box_append(GTK_BOX(panel_slide_box),blank);

  panel = createPanel();
  gtk_box_append(GTK_BOX(panel_slide_box),panel);

  gtk_widget_set_size_request(panel_slide_scroll_window,0,window_height);
  gtk_widget_show(window);
  panel_slide_adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW (panel_slide_scroll_window));
  gtk_adjustment_set_upper(panel_slide_adj, window_height*3);
  gtk_adjustment_set_value(panel_slide_adj,(double)window_height);
  
}
////////////////////////////////////////////
////////////////////////////////////////////
//        notify slide                    //
////////////////////////////////////////////
////////////////////////////////////////////
void dragslide(gdouble y){
  gtk_adjustment_set_value(panel_slide_adj,gtk_adjustment_get_value(panel_slide_adj)-y);
}
void forwardslide(){
  double a = gtk_adjustment_get_value(panel_slide_adj);
  if (a > window_height){
    gtk_adjustment_set_value(panel_slide_adj,window_height);
  }
  else{
    gtk_adjustment_set_value(panel_slide_adj,0);
  }
  
}
void reverseslide(){
  double a = gtk_adjustment_get_value(panel_slide_adj);
  if (a > window_height){
    gtk_adjustment_set_value(panel_slide_adj,window_height*2);
  }
  else{
    gtk_adjustment_set_value(panel_slide_adj,window_height);
  }
}

static GtkWidget * createSlide(){
  
  GdkPixbuf *background_buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,true,8,1,1);
  gdk_pixbuf_fill(background_buf,0x161616e6);
  gdk_pixbuf_scale_simple(background_buf,window_width*5,window_height*10,GDK_INTERP_NEAREST);
  GtkWidget *background = gtk_image_new_from_pixbuf(background_buf);
  //gtk_widget_set_size_request(background,window_width,window_height);
  gtk_widget_set_hexpand(background, true);
  gtk_widget_set_vexpand(background, true);
  return background;
}

////////////////////////////////////////////
////////////////////////////////////////////
//        app panel                    //
////////////////////////////////////////////
////////////////////////////////////////////
void dragpanel(gdouble y){
  gtk_adjustment_set_value(panel_slide_adj,gtk_adjustment_get_value(panel_slide_adj)-y);
}
void forwardpanel(){
  double a = gtk_adjustment_get_value(panel_slide_adj);
  if (a < window_height){
    gtk_adjustment_set_value(panel_slide_adj,window_height);
  }
  else{
    gtk_adjustment_set_value(panel_slide_adj,window_height*2);
  }
  
}
void reversepanel(){
  double a = gtk_adjustment_get_value(panel_slide_adj); 
  if (a < window_height){
    gtk_adjustment_set_value(panel_slide_adj,0);
  }
  else{
    gtk_adjustment_set_value(panel_slide_adj,window_height);
  }
  
}

static GtkWidget *createPanel(){
  GdkPixbuf *background_buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,true,8,1,1);
  gdk_pixbuf_fill(background_buf,0x161616e6);
  gdk_pixbuf_scale_simple(background_buf,window_width*5,window_height*10,GDK_INTERP_NEAREST);
  GtkWidget *background = gtk_image_new_from_pixbuf(background_buf);
  //gtk_widget_set_size_request(background,window_width,window_height);
  gtk_widget_set_hexpand(background, true);
  gtk_widget_set_vexpand(background, true);
  return background;
}

////////////////////////////////////////////
////////////////////////////////////////////
//    main application slide pages        //
////////////////////////////////////////////
////////////////////////////////////////////
application *current_grab_app;

void locateapp(gdouble x, gdouble y){ 
  //locate the app in provided location and put it into current_grab_app
  int grid_x = ((int)(round(x/((double)(window_width/5)))));
  int grid_y = ((int)(round(y/((double)(gtk_widget_get_height(grid)/5)))));
}
void grabapp(){ 
  //grab app grabs the icon from grid and put it into overlay
  printf("grab app\n");
}
void dragapp(gdouble x,gdouble y){
  //drag the app
}
void reverseapp(){
  //reverse the app icon in drag mode
}
void clickapp(gdouble x, gdouble y){
  //exec the app in current_grab_app
  locateapp(x,y);
}


void dragpage(gdouble x){
  GtkAdjustment*adj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW (scroll_window));
  gtk_adjustment_set_value(adj,gtk_adjustment_get_value(adj)+x);
  printf("drag page: %f\n",gtk_adjustment_get_value(adj));
}

void forwardpage(){}
void backwardpage(){}
void reversepage(){}

static void createMain(){

}

/////////////////////////////////////////////
/////////////////////////////////////////////
//       gesture handling                  //
/////////////////////////////////////////////
/////////////////////////////////////////////
char dragbegin = 0x00; //false
struct timespec drag_begin_time;
char drag_direction = 0x00; // 0x01 = left/right 0x02=down 0x03=up 0x04=long click 0x05=drag app

gdouble drag_start_x;
gdouble drag_start_y;

gdouble drag_update_x;
gdouble drag_update_y;


void drag_begin (
  GtkGestureDrag* self,
  //gint n_press,
  gdouble start_x,
  gdouble start_y,
  gpointer user_data
){
  dragbegin=0x01;
  drag_direction=0x00;
  drag_start_x = start_x;
  drag_start_y = start_y;
  timespec_get(&drag_begin_time, TIME_UTC);
  printf("start:%f %f\n",start_x,start_y);
}

void drag_update (
  GtkGestureDrag* self,
  //gint n_press,
  gdouble offset_x,
  gdouble offset_y,
  gpointer user_data
){
  if (drag_direction==0x00){
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    double offx = offset_x;
    double offy = offset_y;
    if (offset_x<0.000){
      offx = offset_x*-1.00000000;
    }
    if (offset_y<0){
      offy = offset_y*-1.00000;
    }
    printf("%f %f\n",offx,offy);
    if (offx<5 && offy<5 && (((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000)-(((int64_t) drag_begin_time.tv_sec) * 1000 + ((int64_t) drag_begin_time.tv_nsec) / 1000000)>800){
      // this is long click or drag app
      locateapp(drag_start_x, drag_start_y);
      drag_direction=0x04; // determind long click or drag when next update
    }
    else if (offx>5 || offy>5){
      if (offx>offy){
        drag_direction=0x01; // left or right
      }
      else if (offset_y>0){
        drag_direction=0x02;// down
      }
      else{
        drag_direction=0x03; // up
      }
    }
  }
  else if (drag_direction==0x01){
    dragpage(offset_x-drag_update_x);
  }
  else if (drag_direction==0x02){
    dragslide(offset_y-drag_update_y);
  }
  else if (drag_direction==0x03){
    dragpanel(offset_y-drag_update_y);
  }
  else if (drag_direction==0x04){

    gdouble offx = offset_x;
    gdouble offy = offset_y;
    if (offset_x<0){
      offx = offset_x*-1;
    }
    if (offset_y<0){
      offy = offset_y*-1;
    }
    if (offx>=5 || offy>=5){
      // this is drag app
      drag_direction=0x05; //switch to drag app mode
      grabapp();
    }
  }
  else if (drag_direction==0x05){
    dragapp(offset_x-drag_update_x, offset_y-drag_update_y);
  }
  drag_update_x = offset_x;
  drag_update_y = offset_y;
  printf("update:%f %f\n",offset_x,offset_y);
}

void drag_end (
  GtkGestureDrag* self,
  //gint n_press,
  gdouble offset_x,
  gdouble offset_y,
  gpointer user_data
){
  dragbegin=0x00;

  if (drag_direction == 0x00){
    // this is a click
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    printf("%li\n",(((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000)-(((int64_t) drag_begin_time.tv_sec) * 1000 + ((int64_t) drag_begin_time.tv_nsec) / 1000000));
    if ((((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000)-(((int64_t) drag_begin_time.tv_sec) * 1000 + ((int64_t) drag_begin_time.tv_nsec) / 1000000)>800){
      printf("long click\n");
    }
    else{
      clickapp(drag_start_x,drag_start_y);
      printf("click\n");
    }
    
  }
  else if (drag_direction==0x04){
    printf("long click2\n");
  }
  else if (drag_direction==0x05){
    gdouble offx = 0;
    gdouble offy = 0;
    if (offset_x<0){
      offx = offset_x*-1;
    }
    if (offset_y<0){
      offy = offset_y*-1;
    }
    if (offx<=5 && offy<=5){
      // app is not moved, reverse
      reverseapp();
    }
    else {
      //this is dragged app
    }
  }
  else if  (drag_direction==0x01){
    // determind if we should go to next page or not by the drag distance
    if (offset_x < window_width/2){
      reversepage();
    }
    else{
      forwardpage();
    }
  }
  else if (drag_direction==0x02){
    //determind if we should slide fully down or reverse
    double offy = offset_y;
    if (offy < 0){
      offy = offy*-1;
    }
    if (offy < window_height/2){
      reverseslide();
    }
    else{
      forwardslide();
    }
  }
  else if (drag_direction==0x03){
    //determind if panel goes up or reverse
    double offy = offset_y;
    if (offy < 0){
      offy = offy*-1;
    }
    if (offy < window_height/2){
      reversepanel();
    }
    else{
      forwardpanel();
    }
  }
  
  drag_direction=0x00;
  
  printf("offset:%f %f\n",offset_x,offset_y);
}


///////////////////////////////////////////////////
//      shutdown page                            //
///////////////////////////////////////////////////

GtkWidget *shutdownpage; // this is a scrolled window
void shutdown_page_slide_in(){
  gtk_overlay_remove_overlay(GTK_OVERLAY(overlay),capturer);
  gtk_overlay_add_overlay(GTK_OVERLAY(overlay),shutdownpage);
}

void shutdown_page_slide_out(GtkButton* self, gpointer user_data){
  gtk_overlay_remove_overlay(GTK_OVERLAY(overlay),shutdownpage);
  gtk_overlay_add_overlay(GTK_OVERLAY(overlay),capturer);
}

void restart(GtkButton* self, gpointer user_data){
  system("reboot");
}

void poweroff(GtkButton* self, gpointer user_data){
  system("poweroff");
}

void logout(GtkButton* self, gpointer user_data){
  exit(0);
}

void shutdown_pressed(){
  shutdown_page_slide_in();
}

void create_shutdown_page(){
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid),true);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid),true);
  GtkWidget *restartbtn=gtk_button_new_with_label("restart");
  GtkWidget *poweroffbtn=gtk_button_new_with_label("power off");
  GtkWidget *logoutbtn=gtk_button_new_with_label("logout");
  GtkWidget *icon = gtk_button_new_with_label ("icon");
  GtkWidget *exitbtn=gtk_button_new_with_label("");
  //GtkWidget *empty = gtk_button_new_with_label ("");
  //gtk_widget_set_size_request(empty,window_width,window_height);
  //gtk_grid_attach(GTK_GRID(grid),empty,0,0,3,8);
  gtk_grid_attach(GTK_GRID(grid),icon,0,0,3,3);
  gtk_grid_attach(GTK_GRID(grid),restartbtn,0,3,1,1);
  gtk_grid_attach(GTK_GRID(grid),poweroffbtn,1,3,1,1);
  gtk_grid_attach(GTK_GRID(grid),logoutbtn,2,3,1,1);
  gtk_grid_attach(GTK_GRID(grid),exitbtn,0,4,3,3);

  g_signal_connect (restartbtn, "clicked",
                G_CALLBACK (restart), NULL);
  g_signal_connect (poweroffbtn, "clicked",
                G_CALLBACK (poweroff), NULL);
  g_signal_connect (logoutbtn, "clicked",
                G_CALLBACK (logout), NULL);
  g_signal_connect (exitbtn, "clicked",
                G_CALLBACK (shutdown_page_slide_out), NULL);

  shutdownpage = grid;
}



///////////////////////////////////////////
//       activate                        //
///////////////////////////////////////////
static void activate (GtkApplication* app, gpointer user_data)
{
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW(window), "seagull");
  gtk_window_maximize(GTK_WINDOW (window));
  //gtk_window_fullscreen (GTK_WINDOW (window));
  //gtk_window_set_resizable(GTK_WINDOW (window),false);
  window_width = gtk_widget_get_width (window);
  window_height = gtk_widget_get_height (window);


  // scroll pages
  overlay= gtk_overlay_new();
  gtk_widget_set_vexpand(overlay, true);
  gtk_widget_set_hexpand(overlay, true);

  capturer= gtk_label_new("");
  gtk_widget_set_vexpand(capturer, true);
  gtk_widget_set_hexpand(capturer, true);

  scroll_window = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window),GTK_POLICY_ALWAYS,GTK_POLICY_NEVER);
  //gtk_scrolled_window_set_max_content_width(GTK_SCROLLED_WINDOW(scroll_window),window_width);

  grid = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid),true);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid),true);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_window),grid);

  panel_slide_scroll_window = gtk_scrolled_window_new();
  createPanel_slide();

  GtkWidget *overlay2 = gtk_overlay_new();
  gtk_widget_set_vexpand(overlay2, true);
  gtk_widget_set_hexpand(overlay2, true);

  gtk_overlay_set_child(GTK_OVERLAY(overlay2),scroll_window);
  gtk_overlay_add_overlay(GTK_OVERLAY (overlay2), panel_slide_scroll_window);
  gtk_overlay_set_child(GTK_OVERLAY (overlay), overlay2);
  gtk_overlay_add_overlay(GTK_OVERLAY (overlay), capturer);

  // header bar
  GtkWidget *headerbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

  // main pages
  GtkWidget *text = gtk_button_new_with_label ("hello");
  gtk_grid_attach(GTK_GRID (grid), headerbar,0,0,5,1);
  gtk_grid_attach(GTK_GRID (grid), text,0,1,1,1);

  //home button bat
  GtkWidget *footerbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_box_set_homogeneous(GTK_BOX(footerbar),true);
  GtkWidget *backwardbtn = gtk_button_new_with_label("???");
  GtkWidget *homebtn = gtk_button_new_with_label("???");
  GtkWidget *tabbtn = gtk_button_new_with_label("???");
  gtk_button_set_has_frame(GTK_BUTTON(backwardbtn),false);
  gtk_button_set_has_frame(GTK_BUTTON(homebtn),false);
  gtk_button_set_has_frame(GTK_BUTTON(tabbtn),false);
  gtk_box_append(GTK_BOX(footerbar),backwardbtn);
  gtk_box_append(GTK_BOX(footerbar),homebtn);
  gtk_box_append(GTK_BOX(footerbar),tabbtn);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_box_append(GTK_BOX(vbox),overlay);
  gtk_box_append(GTK_BOX(vbox),footerbar);


  gtk_window_set_child(GTK_WINDOW(window),vbox);



  GtkGesture *drag = gtk_gesture_drag_new();
  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (drag), 0);
  g_signal_connect (drag, "drag-begin",
                G_CALLBACK (drag_begin), NULL);
  g_signal_connect (drag, "drag-update",
                G_CALLBACK (drag_update), NULL);
  g_signal_connect (drag, "drag-end",
                G_CALLBACK (drag_end), NULL);
  gtk_widget_add_controller(overlay, GTK_EVENT_CONTROLLER (drag));

  create_shutdown_page();

  gtk_widget_show (window);
}

////////////////////////////////////////////
//      connect socket to go program      //
////////////////////////////////////////////

void recvEvent(char *event){
  if (strcmp(event,"key poweroff")==0){
    shutdown_page_slide_in();
  }
}

int gtkmobile()
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.seagull.mobile", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    char* argv[1]= {"seagull"};
    status = g_application_run (G_APPLICATION (app), 0, argv);
    g_object_unref (app);

  return status;
}
