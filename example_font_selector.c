/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <hildon/hildon-program.h>
#include <hildon/hildon-font-selection-dialog.h>

#include <gtk/gtk.h>

/* Application UI data struct */
typedef struct _AppData AppData;
struct _AppData {
    HildonProgram *program;
    HildonWindow *window;
};

void callback_font_selector(GtkWidget * widget, gpointer data)
{
    HildonFontSelectionDialog *dialog;
    gint result;
    PangoAttrList *list = NULL;
    GSList *attrs = NULL;
    PangoAttrIterator *iter;
    AppData *appdata = (AppData *) data;

    /* Create dialog */
    dialog = HILDON_FONT_SELECTION_DIALOG
        (hildon_font_selection_dialog_new(GTK_WINDOW(appdata->window), "Font selector"));

    /* Show the dialog */
    gtk_widget_show_all(GTK_WIDGET(dialog));

    /* Wait for user to select OK or Cancel */
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_OK) {
        /* Get selected font from dialog */
        //FIXME list = hildon_font_selection_dialog_get_font(dialog);
        //iter = pango_attr_list_get_iterator(list);
        //attrs = pango_attr_iterator_get_attrs(iter);


	/* Now the new font setting are in 'attrs' variable */ 
        /* Use it however suitable for the application */

    }

    /* Close the dialog */
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;
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
    button = gtk_button_new_with_label("Font Selector");
    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(callback_font_selector), appdata);
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
