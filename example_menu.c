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

#include <gtk/gtk.h>

/* Callback for "Close" menu entry */
void item_close_cb()
{
    g_print("Closing application...\n");
    gtk_main_quit();
}

/* Create the menu items needed for the main view */
static void create_menu(HildonWindow * main_window) 
{
    /* Create needed variables */
    GtkWidget *main_menu;
    GtkWidget *menu_others;
    GtkWidget *item_others;
    GtkWidget *item_radio1;
    GtkWidget *item_radio2;
    GtkWidget *item_check;
    GtkWidget *item_close;
    GtkWidget *item_separator;

    /* Create new main menu */
    main_menu = gtk_menu_new();

    /* Create new submenu for "Others" */
    menu_others = gtk_menu_new();

    /* Create menu items */
    item_others = gtk_menu_item_new_with_label("Others");
    item_radio1 = gtk_radio_menu_item_new_with_label(NULL, "Radio1");
    item_radio2 =
        gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM
                                                       (item_radio1),
                                                       "Radio2");
    item_check = gtk_check_menu_item_new_with_label("Check");
    item_close = gtk_menu_item_new_with_label("Close");
    item_separator = gtk_separator_menu_item_new();

    /* Add menu items to right menus */
    gtk_menu_append(main_menu, item_others);
    gtk_menu_append(menu_others, item_radio1);
    gtk_menu_append(menu_others, item_radio2);
    gtk_menu_append(menu_others, item_separator);
    gtk_menu_append(menu_others, item_check);
    gtk_menu_append(main_menu, item_close);

    /* Add others submenu to the "Others" item */
    hildon_window_set_menu(HILDON_WINDOW(main_window), GTK_MENU(main_menu));
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_others), menu_others);

    /* Attach the callback functions to the activate signal */
    g_signal_connect(G_OBJECT(item_close), "activate",
                     GTK_SIGNAL_FUNC(item_close_cb), NULL);

    /* Make all menu widgets visible */
    gtk_widget_show_all(GTK_WIDGET(main_menu));
}

/* Main application */
int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("Menu Example");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Add example label to HildonWindow */
    gtk_container_add(GTK_CONTAINER(window),
                      gtk_label_new("Menu Example"));

    /* Create menu for HildonWindow */
    create_menu(window);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(item_close_cb), NULL);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    /* Exit */
    return 0;
}
