/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <hildon-cp-plugin/hildon-cp-plugin-interface.h>
#include <gtk/gtk.h>

osso_return_t execute(osso_context_t *osso, gpointer data, gboolean user_activated)
{
	GtkWidget *dialog;
	gint response;

	/* Create dialog with OK and Cancel buttons. Leave the separator out, as we don't have any content. */
	dialog = gtk_dialog_new_with_buttons(
		"Hello control panel",
		GTK_WINDOW(data),
		GTK_DIALOG_MODAL | GTK_DIALOG_NO_SEPARATOR,
		GTK_STOCK_OK,
		GTK_RESPONSE_OK,
		GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL,
		NULL);

	/* ... add something to the dialog ... */

	if (!user_activated)
	{
		/* ... load state ... */
	}

	/* Wait until user finishes the dialog. */
	response = gtk_dialog_run(GTK_DIALOG(dialog));

	if (response == GTK_RESPONSE_OK)
	{
		/* ... do something with the dialog stuff ... */
	}

	/* Free the dialog (and it's children) */
	gtk_widget_destroy(GTK_WIDGET(dialog));

	return OSSO_OK;
}

osso_return_t save_state(osso_context_t *osso, gpointer data)
{
	/* ... save state ... */

	return OSSO_OK;
}
