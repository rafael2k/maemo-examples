/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2008 Nokia Corporation. All rights reserved.
 */

#include <hildon/hildon-program.h>

#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <gtk/gtk.h>

#define GCONF_KEY "/apps/example_prefs/title"

/* Application UI data struct */
typedef struct _AppData AppData;
struct _AppData {
	HildonProgram *program;
	HildonWindow *window;
};

void commit_entry_data_callback(GtkWidget* entry)
{
	GConfClient* client;
        HildonProgram *program;
	gchar* str;

	str = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);

	client = g_object_get_data(G_OBJECT (entry), "client");
	gconf_client_set_string(client, GCONF_KEY, str, NULL );

	program = g_object_get_data (G_OBJECT (entry), "program");
    	if (str != NULL && *str != '\0') {
		
		/* g_set_application_name(str);  */
	}

	g_free(str);
	return ;
}

int main(int argc, char *argv[])
{
    /* Announce vars */
    HildonProgram 	*program;
    HildonWindow 	*window;
    GtkWidget		*entry, *label, *hbox;
    GConfClient		*client;
    gchar *title;

    /* Initialize GTK. */
    gtk_init(&argc, &argv);

    /* Get the default client */
    client = gconf_client_get_default();

    /*Add GConf node if absent*/
    gconf_client_add_dir (client, "/apps/example_prefs",
                    GCONF_CLIENT_PRELOAD_NONE, NULL);

    /* Read title */
    title = gconf_client_get_string(client, GCONF_KEY, NULL);

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    g_set_application_name("");
    if (title != NULL) {
	gtk_window_set_title (GTK_WINDOW (window), title);
    }

    /* Create buttons and add it to main view */
    hbox = gtk_hbox_new(FALSE, 5);
    label = gtk_label_new ("Change window title: ");
    entry = gtk_entry_new();

    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    gtk_box_pack_end (GTK_BOX (hbox), entry, FALSE, FALSE, 0);

    g_object_set_data (G_OBJECT (entry), "client", client);
    g_object_set_data (G_OBJECT (entry), "program", program);

    g_signal_connect(G_OBJECT(entry), "focus_out_event",
                     G_CALLBACK(commit_entry_data_callback), client);
    g_signal_connect(G_OBJECT(entry), "activate",
                     G_CALLBACK(commit_entry_data_callback), client);

    if (title) {
    	gtk_entry_set_text (GTK_ENTRY (entry), title);
    }
    g_free (title);

    gtk_widget_set_sensitive (entry,
		    gconf_client_key_is_writable (client,GCONF_KEY, NULL));

    gtk_container_add (GTK_CONTAINER (window), hbox);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    /* Exit */
    g_object_unref (G_OBJECT (client));
    return 0;
}
