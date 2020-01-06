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

#include <gtk/gtkmain.h>

static GtkWidget* popup_create(GtkWidget *window)
{
    GtkWidget *menu, *item_close;

    menu = gtk_menu_new ();

    /* Create a menu item */
    item_close = gtk_menu_item_new_with_label ("Close");
    gtk_menu_shell_append (GTK_MENU_SHELL(menu), item_close);
    gtk_widget_show_all (menu);

    /* Menu item causes application to quit */
    g_signal_connect_swapped (G_OBJECT (item_close), "activate",
        G_CALLBACK (gtk_main_quit),
        G_OBJECT (window));

    return menu;
} 


int main(int argc, char *argv[])
{
    /* Create needed variables */

    HildonProgram *program;
    HildonWindow *window;
    GtkWidget *menu, *button;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("Context Example");

    /* Create HildonWindow */
    window = HILDON_WINDOW(hildon_window_new());

    /* the tap-and-hold signal will be connected to this button */
    button = gtk_button_new_with_label("Tap and hold me");

    /* add the textarea to the window */
    gtk_container_add (GTK_CONTAINER (window), button);

    /* set the HildonWindow to HildonProgram */
    hildon_program_add_window(program, window);

    /* Create a menu and set it as the context sensitive menu. */
    menu = popup_create(GTK_WIDGET(window));
    gtk_widget_tap_and_hold_setup(GTK_WIDGET(button), menu, NULL, 0);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    /* Exit */
    return 0;
}
