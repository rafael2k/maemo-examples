/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <hildon/hildon-program.h>
#include <hildon/hildon-find-toolbar.h>

#include <gtk/gtk.h>

/* Application UI data struct */
typedef struct _AppData AppData;
struct _AppData {

    /* View items */
    HildonProgram *program;
    HildonWindow *window;

    /* Toolbar */
    GtkWidget *main_toolbar;

    /* Find toolbar */
    HildonFindToolbar *find_toolbar;

    /* Is Find toolbar visible or not */
    gboolean find_visible;

    /* Result label */
    GtkWidget *label;
};

/* Callback for "Close" menu entry */
void item_close_cb()
{
    g_print("Closing application...\n");
    gtk_main_quit();
}

/* Callback for "Close" toolbar button */
void tb_close_cb(GtkToolButton * widget, AppData * view)
{
    g_print("Closing application...\n");
    gtk_main_quit();
}

/* Callback for "Find" toolbar button */
void tb_find_cb(GtkToolButton * widget, AppData * view)
{
    /* Show or hide find toolbar */
    if (view->find_visible) {
        gtk_widget_hide_all(GTK_WIDGET(view->find_toolbar));
        view->find_visible = FALSE;
    } else {
        gtk_widget_show_all(GTK_WIDGET(view->find_toolbar));
        view->find_visible = TRUE;
    }
}

/* Callback for "Close" find toolbar button */
void find_tb_close(GtkWidget * widget, AppData * view)
{
    gtk_widget_hide_all(GTK_WIDGET(view->find_toolbar));
    view->find_visible = FALSE;
}

/* Callback for "Search" find toolbar button */
void find_tb_search(GtkWidget * widget, AppData * view)
{
    gchar *find_text = NULL;
    gchar *label_text = NULL;
    
    g_object_get(G_OBJECT(widget), "prefix", &find_text, NULL);
    /* Implement the searching here... */
    label_text = g_strdup_printf ("HildonFindToolbar Example.\n\nSearch for:\n\n%s.",  find_text);
    gtk_label_set_text (GTK_LABEL (view->label), label_text);
    g_free(label_text);
			
    g_print("Search for: %s\n", find_text);
}

/* Create the find toolbar */
void ui_create_find_toolbar(AppData * view)
{
    HildonFindToolbar *find_toolbar;
    find_toolbar = HILDON_FIND_TOOLBAR
        (hildon_find_toolbar_new("Find String: "));

    /* Add signal listers to "Search" and "Close" buttons */
    g_signal_connect(G_OBJECT(find_toolbar), "search",
                     G_CALLBACK(find_tb_search), view);
    g_signal_connect(G_OBJECT(find_toolbar), "close",
                     G_CALLBACK(find_tb_close), view);
    hildon_window_add_toolbar(view->window, GTK_TOOLBAR(find_toolbar));
/*    gtk_box_pack_start(GTK_BOX(view->appview->vbox),
                       GTK_WIDGET(find_toolbar), TRUE, TRUE, 0); */

    /* Set variables to AppData */
    view->find_toolbar = find_toolbar;
    view->find_visible = FALSE;
}

/* Create the toolbar for the main view */
static void create_toolbar(AppData * appdata)
{
    /* Create needed variables */
    GtkWidget *main_toolbar;
    GtkToolItem *tb_new;
    GtkToolItem *tb_open;
    GtkToolItem *tb_save;
    GtkToolItem *tb_find;
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
    tb_find = gtk_tool_button_new_from_stock(GTK_STOCK_FIND);
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
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_find, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_close, -1);

    /* Add signal lister to "Close" button */
    g_signal_connect(G_OBJECT(tb_close), "clicked",
                     G_CALLBACK(tb_close_cb), NULL);

    /* Add signal lister to "Find" button */
    g_signal_connect(G_OBJECT(tb_find), "clicked",
                     G_CALLBACK(tb_find_cb), appdata);

    /* Add toolbar HildonWindow */
    hildon_window_add_toolbar(appdata->window, GTK_TOOLBAR(main_toolbar));

    gtk_widget_show_all(main_toolbar);
    appdata->main_toolbar = main_toolbar;
}

/* Main application */
int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;
    AppData *appdata;
    GtkWidget *label;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("HildonFindToolbar Example");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Add example label to window */
    label = gtk_label_new("HildonFindToolbar Example");
    gtk_container_add(GTK_CONTAINER(window), label);
		    
    /* Create AppData */
    appdata = g_new0(AppData, 1);
    appdata->program = program;
    appdata->window = window;
    appdata->label = label;

    /* Create toolbar for view */
    create_toolbar(appdata);

    /* Create find toolbar, but keep it hidden */
    ui_create_find_toolbar(appdata);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(appdata->window), "delete_event",
      G_CALLBACK(item_close_cb), NULL);

    /* Show all other widgets except the find toolbar */
    gtk_widget_show_all(GTK_WIDGET(appdata->window));
    gtk_widget_hide_all(GTK_WIDGET(appdata->find_toolbar));

    /* Begin the main application */
    gtk_main();

    /* Exit */
    return 0;
}
