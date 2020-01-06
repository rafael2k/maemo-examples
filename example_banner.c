/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2009 Nokia Corporation. All rights reserved.
 */

/* Includes */
#include <hildon/hildon-program.h>
#include <hildon/hildon-banner.h>
#include <hildon/hildon-gtk.h>
#include <hildon/hildon-main.h>

static gint banner_type = 1;
GtkWidget *banner = NULL;

/* Callback to show information banners */
void show_banner(GtkButton * widget, HildonWindow * window)
{
    switch (banner_type) {
    case 1:
        /* Show normal information banner and this automatically goes away */
        banner = hildon_banner_show_information(GTK_WIDGET(window), NULL, "Hi there!");
        break;

    case 2:
        /* Remove current information banner */
        gtk_widget_destroy(GTK_WIDGET(banner));
        /* Show progress indicator */
        hildon_gtk_window_set_progress_indicator(GTK_WINDOW(window), 1);
        break;

    case 3:
        /* Hide progress indicator */
        hildon_gtk_window_set_progress_indicator(GTK_WINDOW(window), 0);
        break;

    case 4:
        /* With fourth click, end the application */
        gtk_main_quit();
    }

    /* Increase the counter */
    banner_type++;
}


/* Main application */
int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;
    GtkWidget *main_vbox;
    GtkWidget *button1;

    /* Initialize the GTK+ and hildon libraries. */
    hildon_gtk_init(&argc, &argv);

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("App Title");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Add vbox to appview */
    main_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    /* Add button to vbox */
    button1 = gtk_button_new_with_label("Show Info");
    gtk_box_pack_end(GTK_BOX(main_vbox), button1, FALSE, TRUE, 0);

    /* Add signal listener to button */
    g_signal_connect(G_OBJECT(button1), "clicked",
                     G_CALLBACK(show_banner), window);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    /* Exit */
    return 0;
}

