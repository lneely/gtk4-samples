#include <gtk/gtk.h>
#include <stdio.h>
#include <epoxy/gl.h>
#include <math.h>
#include "draw.h"

// a simple gtk application wrapper around a GL drawing area

static void activate(GtkApplication*, gpointer);
static void realize(GtkWidget*);
static void unrealize(GtkWidget*);
static gboolean render (GtkGLArea*, GdkGLContext*);


int
main(int argc, char **argv) {
	printf("%s\n", __func__);
	GtkApplication* app;
	int status;

	app = gtk_application_new("eris.sample.glarea", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_clear_object(&app);
	return status;
}

static void
activate(GtkApplication* self, gpointer data) {
	printf("%s\n", __func__);
	GtkWidget* win;
	GtkWidget* glarea;

	win = gtk_application_window_new(self);
	gtk_window_set_title(GTK_WINDOW(win), "glarea");
	gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);

	glarea = gtk_gl_area_new();
	gtk_gl_area_set_auto_render(GTK_GL_AREA(glarea), true);
	gtk_widget_set_halign(glarea, GTK_ALIGN_FILL);
	gtk_widget_set_valign(glarea, GTK_ALIGN_FILL);
	g_signal_connect(glarea, "realize", G_CALLBACK(realize), NULL);
	g_signal_connect(glarea, "unrealize", G_CALLBACK(unrealize), NULL);
	g_signal_connect(glarea, "render", G_CALLBACK(render), NULL);

	gtk_window_set_child(GTK_WINDOW(win), glarea);

	gtk_widget_show(win);
}

static void
realize(GtkWidget *self) {
	printf("%s\n", __func__);
	gtk_gl_area_make_current (GTK_GL_AREA(self));
	if (gtk_gl_area_get_error (GTK_GL_AREA(self)) != NULL) {
		printf("error making gl area current\n");
		return;
	}

	draw_init();
}

static void
unrealize(GtkWidget *self) {
	printf("%s\n", __func__);
	gtk_gl_area_make_current (GTK_GL_AREA(self));
	if (gtk_gl_area_get_error (GTK_GL_AREA(self)) != NULL) {
		printf("error making gl area current\n");
		return;
	}

	draw_shutdown();
}

static gboolean
render (GtkGLArea *area, GdkGLContext *context) {
	printf("%s\n", __func__);
	glClearColor (0, 0, 0, 0);
	glClear (GL_COLOR_BUFFER_BIT);

	printf("rendering graphics\n");
	draw_render();

	printf("flushing buffer\n");
	glFlush();

	return TRUE;
}