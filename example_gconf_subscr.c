/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

/**
 * This example subscribes to the /apps/osso/inputmethod/ext_kb_model
 * gconf node and if any changes made to that node the label will be 
 * updated. One way to change the model of the external keyboard is
 * to open the Control Panel / Bluetooth Keyboard applet and change 
 * the keyboard model to something else.
 *
 * The other way is to use the gconftool utility:
 * gconftool --set --type=string  /apps/osso/inputmethod/ext_kb_model "pc105"
 */

#include <hildon/hildon-program.h>

#include <gtk/gtkmain.h>
#include <gtk/gtklabel.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>

#define GCONF_DIR "/apps/osso/inputmethod"
/* #define GCONF_KEY GCONF_DIR "/ext_kb_model" */
#define GCONF_KEY GCONF_DIR "/int_kb_layout"

static void
callback_gconf_notify (GConfClient *client, guint cnxn_id,
		       GConfEntry *entry, gpointer *data)
{
    g_return_if_fail(data);
    g_return_if_fail(entry);
    g_print("************************************* %s %s\n", entry->key, gconf_value_get_string(entry->value));
    gtk_label_set_text(GTK_LABEL(data),gconf_value_get_string(entry->value));
    
}

int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;
    GtkWidget *label, *model_label, *hbox;
    GConfClient *client;
    gchar *model;
    guint connection_id;
    GError    *error = NULL;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    /* Get the default client */
    client = gconf_client_get_default();

    g_return_val_if_fail(client,-1);
	    
    /*Add GConf node if absent*/
    gconf_client_add_dir (client, GCONF_DIR,
			  GCONF_CLIENT_PRELOAD_NONE, &error);

    if (error) {
        g_printerr ("%s (gconf_client_add_dir)\n", error->message);
	g_error_free (error);
	error = NULL;
    }
    
    /* Read keyboard model */
    model = gconf_client_get_string(client, GCONF_KEY, &error);
	
    if (error) {
        g_printerr ("%s (gconf_client_set_string)\n", error->message);
	g_error_free (error);
	error = NULL;
    }

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("Example Gconf Subscr");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Create a label and add it to main view */
    hbox = gtk_hbox_new(FALSE, 5);
    /* label = gtk_label_new("Current keyboard model: "); */
    label = gtk_label_new("Current keyboard layout: ");
    model_label = gtk_label_new(model);
    
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), model_label, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    /* Receive notification if the keyboard model is changed from the control panel */
    connection_id = gconf_client_notify_add(client,
		                            GCONF_KEY,
		                            (GConfClientNotifyFunc)callback_gconf_notify,
		                            model_label,
		                            NULL,
		                            &error);
    if (error) {
	    g_printerr ("%s (gconf_client_notify_add %s)\n", error->message,GCONF_KEY);
	    g_error_free (error);
    }
  
    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    gconf_client_notify_remove(client,connection_id);
    /* Free gconf settings object */
    g_object_unref(client);
     
    
    /* Exit */
    return 0;
}
