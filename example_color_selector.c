/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <hildon/hildon-program.h>
#include <hildon/hildon-color-chooser-dialog.h>
#include <hildon/hildon-color-button.h>

#include <gtk/gtk.h>

/* Application UI data struct */
typedef struct _AppData AppData;
struct _AppData {
    HildonProgram *program;
    HildonWindow *window;
};

void color_button_clicked(GtkWidget * widget, gpointer data)
{
    GdkColor *new_color = NULL;
    g_object_get(widget, "color", &new_color, NULL);
}

void ui_show_color_selector(GtkWidget * widget, AppData * appdata)
{
    GdkColor color = {0, 0, 0, 0};
    GtkWidget *selector;
    gint result;

    selector = hildon_color_chooser_dialog_new();
    /* Set the current selected color to selector */
    hildon_color_chooser_dialog_set_color(
		    HILDON_COLOR_CHOOSER_DIALOG(selector),
                    &color);

    /* Show dialog */
    result = gtk_dialog_run(GTK_DIALOG(selector));

    /* Wait for user to select OK or Cancel */
    switch (result) {
    case GTK_RESPONSE_OK:
        /* Get the current selected color from selector */
        hildon_color_chooser_dialog_get_color
            (HILDON_COLOR_CHOOSER_DIALOG(selector), &color);

        /* Now the new color is in 'color' variable */
        /* Use it however suitable for the application */

        gtk_widget_destroy(selector);
        break;

    default:

        /* If dialog didn't return OK then it was canceled */
        gtk_widget_destroy(selector);
        break;
    }

}


int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;
    HildonColorButton *color_button;
    GtkWidget *button;
    GtkWidget *vbox;
    AppData *appdata;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("Hello maemo!");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Create AppData */
    appdata = g_new0(AppData, 1);
    appdata->program = program;
    appdata->window = window;

    /* Create buttons and add it to main view */
    vbox = gtk_vbox_new(TRUE, 5);

    /* Test */
    color_button = HILDON_COLOR_BUTTON(hildon_color_button_new());
    g_signal_connect(G_OBJECT(color_button), "clicked",
                     G_CALLBACK(color_button_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(color_button));

    button = gtk_button_new_with_label("Color Selector");
    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(ui_show_color_selector), appdata);
    gtk_container_add(GTK_CONTAINER(vbox), button);


    /* Add VBox to AppView */
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    /* Exit */
    return 0;
}
