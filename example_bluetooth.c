/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

/* !!!README!!!
 * The SDK doesn't include the daemon that implements the
 * bluetooth-services that this example uses, so it can be
 * run successfully only on the device. However, it compiles
 * and runs on the Scratchbox as well, but connecting
 * to the bluetooth UI service fails */

#include "example_common.h"
#include <gtk/gtk.h>
#include <hildon/hildon-file-chooser-dialog.h>

#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus-glib.h>

/** Define the service access information */
/** Conbtdialogs service, resides in system dbus */
#define CONBTDIALOGS_DBUS_SERVICE		"com.nokia.bt_ui"

/** Conbtdialogs interface */
#define CONBTDIALOGS_DBUS_INTERFACE		"com.nokia.bt_ui"
/** Conbtdialogs path */
#define CONBTDIALOGS_DBUS_PATH		        "/com/nokia/bt_ui"

/* Define structure for app-wide data */
typedef struct
{
	HildonProgram *program;
	HildonWindow *window;

	HildonFileChooserDialog *selection;
	DBusGConnection *dbus;
} AppData;

/* Send file over bluetooth. Opens a dialog for user to select device */
static void send_file(gchar *filename, AppData *appdata)
{	
	DBusGProxy *bt_ui;
	GError *error = NULL;
	char *files[2];
	gboolean call_result, result;

	/* The DBUS-type DBUS_TYPE_STRING expects an array
	 * with null termination, but only one file is sent,
	 * so the array is defined like this */
	files[0] = filename; files[1] = NULL;

	/* Create a proxy object for bt_ui service */
	bt_ui = dbus_g_proxy_new_for_name(
			appdata->dbus, CONBTDIALOGS_DBUS_SERVICE,
			CONBTDIALOGS_DBUS_PATH, CONBTDIALOGS_DBUS_INTERFACE);
	
	/* Call show_send_file_dlg which shows a device selection
	 * dialog to user and handles the sendling also */
	call_result = dbus_g_proxy_call(bt_ui, "show_send_file_dlg",
			&error,
			G_TYPE_STRV, files,
			G_TYPE_INVALID, /* End arguments */
			G_TYPE_INVALID); /* End return values */

	/* Check for errors and report them to user */
	/* call_result tells if the call itself worked, this is returned
	 * by the framework, not from the service */
	if(!call_result)
	{
		g_warning("Failed to call DBUS service: %s\n",
				error->message);
		g_object_unref(bt_ui);
		g_error_free(error);
		return;
	}
	/* This is the return value from the service and specifies if
	 * the dialog was shown properly or not */
	if(!result)
	{
		hildon_banner_show_information(GTK_WIDGET(appdata->window),
				"gtk-dialog-error",
				"Failed to call DBUS service");

		g_object_unref(bt_ui);
		return;
	}
			
	
	g_object_unref(bt_ui);
}

/* Open a file selection dialog */
static gboolean select_file(AppData *appdata)
{
	gchar *file;
	GtkWidget *selection;
	guint response;

	/* Create new hildon_file_chooser dialog
	 * for opening a file */
	selection = hildon_file_chooser_dialog_new(
			GTK_WINDOW(appdata->window),
			GTK_FILE_CHOOSER_ACTION_OPEN);

	/* Run the dialog and see if user clicked the
	 * "Send"-button or not */
	response = gtk_dialog_run(GTK_DIALOG(selection));
	if(response != GTK_RESPONSE_OK)
	{
		gtk_widget_destroy(selection);
		gtk_main_quit();
		return FALSE;
	}

	/* Get name of the selected file from the dialog */
	file = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(selection));
	/* Send the file using the internal function above */
	send_file(file, appdata);

	/* Free allocated resource and destroy (and hide)
	 * the dialog */
	g_free(file);
	gtk_widget_destroy(selection);

	/* Return TRUE meaning that the g_idle_add-callback won't
	 * be removed and this function is called again just after
	 * it returns to the main loop */
	return TRUE;
}

int main(int argc, char **argv)
{
	AppData appdata;
	
	/* Initialize the program and window */
	example_gui_initialize(&appdata.program,
			&appdata.window, &argc, &argv,
			"Bluetooth example");

	/* Add an idle callback to select_file-function. Idle callback
	 * gets called, when there are no pending events in the event
	 * loop. This gets called for the first time when the application
	 * has initialized and then again practically just after it returns */
	g_idle_add((GSourceFunc)select_file, &appdata);

	/* Initialize a D-BUS-connection to the system bus */
	appdata.dbus = dbus_g_bus_get(DBUS_BUS_SYSTEM, NULL);
	if(!appdata.dbus)
	{
		g_error("Couldn't open dbus");
		return 1;
	}
	
	/* Run the program */
	example_gui_run(appdata.program, appdata.window);

	/* Clean up */
	dbus_g_connection_unref(appdata.dbus);
	return 0;
}
