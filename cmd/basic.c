// basic creates an empty window using gtk4
#include <gtk/gtk.h>

// activate is signaled on application startup
// (from g_application_run()
static void
activate(GtkApplication* app, gpointer user_data) {
  GtkWidget *window;
  
  // initialize a new window
  window = gtk_application_window_new(app);
  
  // set window properties
  gtk_window_set_title(GTK_WINDOW(window), 
    "New GTK application");
  gtk_window_set_default_size(GTK_WINDOW(window), 
    200, 200);
  gtk_window_present(GTK_WINDOW(window));
}

int 
main(int argc, char** argv) {
  GtkApplication *app;
  int status;
  
  // initialize a new gtk application
  app = gtk_application_new("eris.sample",
    G_APPLICATION_DEFAULT_FLAGS);
    
  // connect the activate signal
  g_signal_connect(app, "activate",
    G_CALLBACK(activate), NULL);
  
  // run the application; blocks until exit
  status = g_application_run(G_APPLICATION(app), 
    argc, argv);
  
  // destroy application reference
  g_object_unref(app);
  
  return 0;
}
