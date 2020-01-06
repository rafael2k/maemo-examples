/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#ifndef _EXAMPLE_COMMON_H_
#define _EXAMPLE_COMMON_H_

#include <hildon/hildon-banner.h>
#include <hildon/hildon-program.h>
#include <gtk/gtk.h>

/* Initialize the gui by creating a HildonProgram
 * and HildonWindow */
void example_gui_initialize(
		HildonProgram **program,
		HildonWindow **window,
		int *argc, char ***argv,
		gchar *example_name)
{
	g_thread_init(NULL);

	/* Initialize GTK+ */
	gtk_init(argc, argv);

	/* Create HildonProgram and set application name */
	*program = HILDON_PROGRAM(hildon_program_get_instance());
	g_set_application_name(example_name);

	/* Create the toplevel HildonWindow */
	*window = HILDON_WINDOW(hildon_window_new());

	/* Connect destroying of the main window to gtk_main_quit */
	g_signal_connect(G_OBJECT(*window), "delete_event",
			G_CALLBACK(gtk_main_quit), NULL);
}

void example_gui_run(HildonProgram *program,
			HildonWindow *window)
{
	/* Show the window and widgets it contains
	 * and go to the main loop. */
	gtk_widget_show_all(GTK_WIDGET(window));
	gtk_main();
}



#endif
