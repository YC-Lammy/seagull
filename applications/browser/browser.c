#include<gtk/gtk.h> //gtk+-3.0
#include<webkit2/webkit2.h>

WebKitWebView *webview;

WebKitWebView *currentWebview;

GtkWidget *notebook;


void backwardbutton_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *view = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
    webkit_web_view_go_back(WEBKIT_WEB_VIEW(view));
}

void forwardbutton_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *view = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
    webkit_web_view_go_forward(WEBKIT_WEB_VIEW(view));
}

void reloadbutton_clicked(GtkWidget *widget, gpointer data){
    GtkWidget *view = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
    webkit_web_view_reload (WEBKIT_WEB_VIEW(view));
}

void search(GtkWidget *widget, gpointer data){
    gtk_entry_get_text (GTK_ENTRY (widget));
}

void addpage(GtkWidget *widget, gpointer data){

    WebKitWebView *view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    webkit_web_view_load_uri(view, "http://duckduckgo.com");
    //gtk_notebook_append_page(GTK_NOTEBOOK(notebook),GTK_WIDGET (view),NULL);
    gtk_notebook_insert_page(GTK_NOTEBOOK (notebook),GTK_WIDGET (view),NULL,gtk_notebook_get_n_pages(GTK_NOTEBOOK (notebook))-1);
    gtk_widget_show_all(GTK_WIDGET(notebook));
}

void addlongpress(GtkGestureLongPress *swipe, gdouble x, gdouble y, gpointer user_data){
    printf("x:%v y:%v",x,y);
}

void swipepage(GtkGestureSwipe *swipe, gdouble x, gdouble y, gpointer user_data){
    printf("x:%v y:%v",x,y);
}



int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(grid));

    GtkWidget *url_bar = gtk_entry_new();

    GtkWidget *backwardButton = gtk_button_new_with_label("<");
    GtkWidget *forwardButton = gtk_button_new_with_label(">");
    GtkWidget *reloadButton = gtk_button_new_with_label("⟳");
    //GtkWidget *backArrow = gtk_arrow_new(GTK_ARROW_LEFT, GTK_SHADOW_NONE);
    //GtkWidget *forwardArrow = gtk_arrow_new(GTK_ARROW_RIGHT, GTK_SHADOW_NONE);
    //gtk_button_set_child(GTK_BUTTON(backwardButton),backArrow);
    //gtk_button_set_child(GTK_BUTTON(forwardButton),forwardArrow);

    gtk_grid_attach(GTK_GRID (grid), backwardButton,0,0,1,1);
    gtk_grid_attach(GTK_GRID (grid), forwardButton,1,0,1,1);
    gtk_grid_attach(GTK_GRID (grid), reloadButton,2,0,1,1);
    gtk_grid_attach(GTK_GRID (grid), url_bar, 4, 0, 10, 1);


    webview = WEBKIT_WEB_VIEW(webkit_web_view_new());

    GtkWidget *addbutton = gtk_button_new_with_label("+");
    GtkGesture *addlong = gtk_gesture_long_press_new(GTK_WIDGET (addbutton));
    g_signal_connect (addlong,"pressed",G_CALLBACK(addlongpress),NULL);

    notebook = gtk_notebook_new();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),GTK_WIDGET (webview),NULL);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),GTK_WIDGET (webview),GTK_WIDGET (addbutton));

    GtkGesture *swipe = gtk_gesture_swipe_new(GTK_WIDGET (notebook));
    g_signal_connect (swipe,"swipe",G_CALLBACK(swipepage),NULL);

    gtk_grid_attach(GTK_GRID (grid),  notebook, 0, 1, 20, 4);

    g_signal_connect (backwardButton,"clicked",G_CALLBACK(backwardbutton_clicked),NULL);
    g_signal_connect (forwardButton,"clicked",G_CALLBACK(forwardbutton_clicked),NULL);
    g_signal_connect (reloadButton,"clicked",G_CALLBACK(reloadbutton_clicked),NULL);
    g_signal_connect (addbutton,"clicked",G_CALLBACK(addpage),NULL);

    webkit_web_view_load_uri(webview, "http://duckduckgo.com");

    gtk_widget_grab_focus(GTK_WIDGET(webview));

    gtk_widget_set_vexpand(GTK_WIDGET(webview), true);
    gtk_widget_set_hexpand(GTK_WIDGET(webview), true);

    gtk_widget_show_all(main_window);

    gtk_main();

    return 0;
}
