/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

/* Includes */
#include <hildon/hildon-program.h>

#include <gtk/gtkmain.h>
#include <gtk/gtklabel.h>

int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("App Title");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Add example label to window */
    gtk_container_add(GTK_CONTAINER(window),
                      GTK_WIDGET(gtk_label_new("HildonProgram Example")));

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    /* Exit */
    return 0;
}
