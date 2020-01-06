/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <hildon/hildon-program.h>

#include <gtk/gtk.h>

/* Callback for "Close" menu entry */
void item_close_cb()
{
    g_print("Closing application...\n");
    gtk_main_quit();
}

/* Callback for "Close" toolbar button */
void tb_close_cb(GtkToolButton * widget)
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

/* Create the toolbar needed for the main view */
/* XXX: s/menu items/toolbar/ */
static void create_toolbar(HildonWindow * main_window)
{
    /* Create needed variables */
    GtkWidget *main_toolbar;
    GtkToolItem *tb_new;
    GtkToolItem *tb_open;
    GtkToolItem *tb_save;
    GtkToolItem *tb_close;
    GtkToolItem *tb_separator;
    GtkToolItem *tb_comboitem;
    GtkComboBox *tb_combo;

    /* Create toolbar */
    main_toolbar = gtk_toolbar_new();

    /* Create toolbar button items */
    tb_new = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
    tb_open = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    tb_save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
    tb_close = gtk_tool_button_new_from_stock(GTK_STOCK_CLOSE);

    /* Create toolbar combobox item */
    tb_comboitem = gtk_tool_item_new();
    tb_combo = GTK_COMBO_BOX(gtk_combo_box_new_text());
    gtk_combo_box_append_text(tb_combo, "Entry 1");
    gtk_combo_box_append_text(tb_combo, "Entry 2");
    gtk_combo_box_append_text(tb_combo, "Entry 3");
    /* Select second item as default */
    gtk_combo_box_set_active(GTK_COMBO_BOX(tb_combo), 1);
    /* Make combobox to use all available toolbar space */
    gtk_tool_item_set_expand(tb_comboitem, TRUE);
    /* Add combobox inside toolitem */
    gtk_container_add(GTK_CONTAINER(tb_comboitem), GTK_WIDGET(tb_combo));

    /* Create separator */
    tb_separator = gtk_separator_tool_item_new();

    /* Add all items to toolbar */
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_new, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_separator, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_open, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_save, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_comboitem, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_close, -1);

    /* Add signal lister to "Close" button */
    g_signal_connect(G_OBJECT(tb_close), "clicked",
                     G_CALLBACK(tb_close_cb), NULL);

    /* Add toolbar HildonWindow */
    hildon_window_add_toolbar(main_window, GTK_TOOLBAR(main_toolbar));
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
    g_set_application_name("HildonToolbar Example");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Add example label to HildonWindow */
    gtk_container_add(GTK_CONTAINER(window),
                      gtk_label_new("HildonToolbar Example"));

    /* Create menu for HildonWindow */
    create_menu(window);

    /* Create toolbar for HildonWindow */
    create_toolbar(window);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    /* Exit */
    return 0;
}
