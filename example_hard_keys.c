/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <hildon/hildon-program.h>
#include <hildon/hildon-banner.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

/* Callback for hardware keys */
gboolean key_press_cb(GtkWidget * widget, GdkEventKey * event,
                      HildonWindow * window)
{
    switch (event->keyval) {
    case GDK_Up:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Navigation Key Up");
        return TRUE;

    case GDK_Down:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Navigation Key Down");
        return TRUE;

    case GDK_Left:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Navigation Key Left");
        return TRUE;

    case GDK_Right:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Navigation Key Right");
        return TRUE;

    case GDK_Return:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Navigation Key select");
        return TRUE;

    case GDK_F6:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Full screen");
        return TRUE;

    case GDK_F7:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Increase (zoom in)");
        return TRUE;

    case GDK_F8:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Decrease (zoom out)");
        return TRUE;

    case GDK_Escape:
        hildon_banner_show_information(GTK_WIDGET(window), NULL, "Cancel/Close");
        return TRUE;
    }

    return FALSE;
}

/* Main application */
int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;
    GtkWidget *main_vbox;
    GtkWidget *label;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("AppView Title");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Add vbox to appview */
    main_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    /* Add button to vbox */
    label = gtk_label_new("Press Hardware Keys!");
    gtk_box_pack_start(GTK_BOX(main_vbox), label, FALSE, TRUE, 0);

    /* Add hardware button listener to application */
    g_signal_connect(G_OBJECT(window),
                     "key_press_event", G_CALLBACK(key_press_cb), window);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    /* Exit */
    return 0;
}
