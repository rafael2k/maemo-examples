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
#include <libosso.h>

gboolean ICanWrite = TRUE;

/* Application UI data struct */
typedef struct _AppData AppData;
struct _AppData {
    HildonProgram *program;
    HildonWindow *window;
    osso_context_t *osso_context;
};

/* Callback for exit D-BUS event */
void exit_event_handler(gboolean die_now, gpointer data)
{
    AppData *appdata;
    appdata = (AppData *) data;
    g_print("exit_event_handler called\n");
    /* Do whatever application needs to do before exiting */
    hildon_banner_show_information(GTK_WIDGET(appdata->window), NULL,
                                   "Exiting...");
}

/* Callback for normal D-BUS messages */
gboolean dbus_req_handler(const gchar * interface, const gchar * method,
                      GArray * arguments, gpointer data,
                      osso_rpc_t * retval)
{
    AppData *appdata;
    appdata = (AppData *) data;

    osso_system_note_infoprint(appdata->osso_context, method, retval);

    if (g_strcasecmp(method, "backup_start") == 0)
        ICanWrite = FALSE;
    if (g_strcasecmp(method, "backup_finish") == 0)
	ICanWrite = TRUE;

    osso_rpc_free_val(retval);

    return OSSO_OK;
}

/* Callback for showing status */
gboolean status(gpointer data)
{
    AppData *appdata;
    appdata = (AppData *) data;
    if (ICanWrite)
        hildon_banner_show_information(GTK_WIDGET(appdata->window),
                                       NULL, "I can write");

    else
        hildon_banner_show_information(GTK_WIDGET(appdata->window),
                                       NULL, "I cannot write now");
    return TRUE;
}

/* Main application */
int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;
    osso_context_t *osso_context;
    osso_return_t result;
    GtkWidget *main_vbox;
    GtkWidget *label;
    AppData *appdata;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    /* Initialize maemo application */
    osso_context = osso_initialize("com.nokia.backup", "0.0.2", TRUE, NULL);

    /* Check that initialization was ok */
    if (osso_context == NULL) {
        return OSSO_ERROR;
    }

    /* Create the hildon program and setup the title */
    program = HILDON_PROGRAM(hildon_program_get_instance());
    g_set_application_name("Backup");

    /* Create HildonWindow and set it to HildonProgram */
    window = HILDON_WINDOW(hildon_window_new());
    hildon_program_add_window(program, window);

    /* Create AppData */
    appdata = g_new0(AppData, 1);
    appdata->program = program;
    appdata->window = window;
    appdata->osso_context = osso_context;

    /* Add vbox to appview */
    main_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    /* Add button to vbox */
    label = gtk_label_new("Waiting for DBUS message...");
    gtk_box_pack_start(GTK_BOX(main_vbox), label, FALSE, TRUE, 0);

    /* Add handler for D-BUS messages */
    result = osso_rpc_set_cb_f(appdata->osso_context, 
                               "com.nokia.backup", 
                               "/com/nokia/backup", 
                               "com.nokia.backup", 
                               dbus_req_handler, appdata);
    if (result != OSSO_OK) {
        g_print("Error setting D-BUS callback (%d)\n", result);
        return OSSO_ERROR;
    }

    /* Add handler for Exit D-BUS messages */
    result = osso_application_set_exit_cb(appdata->osso_context,
                                          exit_event_handler,
                                          (gpointer) appdata);
    if (result != OSSO_OK) {
        g_print("Error setting exit callback (%d)\n", result);
        return OSSO_ERROR;
    }

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(window), "delete_event",
      G_CALLBACK(gtk_main_quit), NULL);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(window));
    g_timeout_add(3000, status, appdata);
    gtk_main();

    /* Deinitialize OSSO */
    osso_deinitialize(osso_context);

    /* Exit */
    return 0;
}
