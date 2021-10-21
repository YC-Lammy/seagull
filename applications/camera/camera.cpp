#include <stdio.h>
#include <thread>
#include <pthread.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>


extern "C"{
    #include <gtk/gtk.h>
}

cv::VideoCapture cam0;
cv::VideoCapture cam1;
cv::VideoCapture cam2;
cv::VideoCapture cam3;

int cameras = 0; 

int current_camera = 0;

GtkWidget *window;

GtkWidget *implane;
cairo_t *implane_cr;

cv::Mat frame;
cv::Mat framebuffer[60];

void dummyDestroy(guchar *pixels, gpointer frame){

}

void previewdraw(GtkDrawingArea *widget, cairo_t *cr, int x,int y,gpointer data){
    if (current_camera == 0){
            cam0.read(frame);
    } else if (current_camera == 1){
        cam1.read(frame);
    } else if (current_camera == 2){
        cam2.read(frame);
    } else if (current_camera == 3){
        cam3.read(frame);
    }
    if (frame.empty()){
        //frame.release();
        return;
    }

    cv::flip(frame,frame,1);
    cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
    //cv::fastNlMeansDenoisingColored(frame,frame);
    //cv::resize(frame,frame,cv::Size{frame.cols*2,frame.rows*2},0,0,cv::INTER_AREA);

    GdkPixbuf *buf = gdk_pixbuf_new_from_data((guint8*)frame.data,GDK_COLORSPACE_RGB,
    false,8,frame.cols,frame.rows,(int)frame.step,dummyDestroy,&frame);
    if (buf){
        gdk_cairo_set_source_pixbuf(cr,buf,0,0);
        cairo_paint(cr);
        //g_object_unref(buf);
    } else{
        printf("error formatting buf");
        return;
    }

}

gboolean refresh (
  gpointer user_data
){
    gtk_widget_queue_draw(implane);
    return true;
}

static void activate (GtkApplication* app, gpointer user_data)
{
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW(window), "Camera");
  gtk_window_maximize(GTK_WINDOW (window));
  implane = gtk_drawing_area_new();
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(implane),previewdraw,NULL,NULL);
  g_timeout_add(10,refresh,NULL);
  gtk_widget_set_vexpand(implane,true);
  gtk_widget_set_hexpand(implane,true);
  gtk_window_set_child(GTK_WINDOW(window),implane);
  gtk_widget_show(window);
}

int main(int argc, char **argv){
    cam0.open(0,cv::CAP_ANY);
    if (cam0.isOpened()){
        cameras +=1;
    }
    cam1.open(1,cv::CAP_ANY);
    if (cam1.isOpened()){
        cameras +=1;
    }
    cam2.open(2,cv::CAP_ANY);
    if (cam2.isOpened()){
        cameras +=1;
    }
    cam3.open(3,cv::CAP_ANY);
    if (cam3.isOpened()){
        cameras +=1;
    }
    //std::thread camloop(cameraLoop);

    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.seagull.mobile", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
    status = g_application_run (G_APPLICATION(app), 0, argv);
    g_object_unref (app);
    //camloop.join();
}