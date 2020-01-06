/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <gtk/gtk.h>
#include <libgnomevfs/gnome-vfs.h>
#include <string.h>

#include "example_common.h"

/* Declare structure for data that's needed throughout
 * the program */
typedef struct
{
	HildonProgram *program;
	HildonWindow *window;
	gchar *testfile;
} AppData;

#define APP_NAME "GnomeVFS example"

/* URI for default testfile. Used if
 * no file is given as an argument */
#define DEFAULT_TESTFILE_URI "file:/tmp/testfile"

#define READ_BUFFER_SIZE 32

/* This function reads all of the file pointed by text_uri
 * to string */
static GnomeVFSResult _read_file_to_string(gchar *text_uri,
		GString *string)
{	
	/* Initialize needed variables */
	GnomeVFSResult result;
	GnomeVFSHandle *handle;
	gchar buffer[READ_BUFFER_SIZE];
	GnomeVFSFileSize bytes_read;
	
	/* Open a file handle for reading and
	 * check for errors */ 
	result = gnome_vfs_open(&handle, text_uri,
			GNOME_VFS_OPEN_READ);
	if(result != GNOME_VFS_OK)
	{
		return result;
	}
	
	/* Read the file to string READ_BUFFER_SIZE bytes
	 * at a time */
	do
	{
		/* Read READ_BUFFER_SIZE bytes from the handle
		 * and write them to buffer */
		result = gnome_vfs_read(handle, buffer,
			READ_BUFFER_SIZE, &bytes_read);

		/* Append read data from the buffer to string */
		g_string_append_len(string, buffer, bytes_read);
		
	} while(result == GNOME_VFS_OK);

	/* Close the file handle */
	gnome_vfs_close(handle);
	
	/* GNOME_VFS_ERROR_EOF is a success here
	 * so GNOME_VFS_OK is returned instead. Else
	 * return the result as is */
	if(result == GNOME_VFS_ERROR_EOF)
		return GNOME_VFS_OK;
	else
		return result;
}

/* Callback for the "Read textfile" button. Reads data from
 * appdata->textfile and shows it in a dialog */
static void read_testfile(GtkWidget *widget, AppData *appdata)
{
	GnomeVFSResult result;
	GtkMessageDialog *dialog;
	GString *data;

	/* Initialize the data string and
	 * read file data by the auxiliarry function */
	data = g_string_new("");
	result = _read_file_to_string(appdata->testfile, data);
	
	/* Check that the reading went OK */
	if(result != GNOME_VFS_OK)
	{
		hildon_banner_show_information(GTK_WIDGET(appdata->window),
				"gtk-dialog-error",
				gnome_vfs_result_to_string(result));
		goto cleanup;
	}

	/* Show the data read in a dialog */
	dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
			GTK_WINDOW(appdata->window),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_CLOSE,
			"%s",
			appdata->testfile));
	gtk_message_dialog_format_secondary_text(dialog,
			"%s", data->str);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));

	/* Clean up reserved resources */	
	cleanup:
	g_string_free(data, TRUE);
}

/* This string is written in to the testfile */
#define TESTFILE_STRING "### Maemo GnomeVFS testfile ###\n"

/* Writes TESTFILE_STRING to the testfile if it doesn't exist.
 * Doesn't overwrite existing files */
static void write_testfile(GtkWidget *widget, AppData *appdata)
{
	GnomeVFSHandle *handle;
	GnomeVFSResult result;
	GnomeVFSURI *file_uri;
	GnomeVFSFileSize bytes_written;
	GnomeVFSFileSize bytes_written_total = 0;
	const gchar *testfile_string = TESTFILE_STRING;
	guint string_len = strlen(testfile_string);
	
	/* Create a GnomeVFSURI object and use it to
	 * check that the file doesn't exist to prevent
	 * "oops" when testing. */
	file_uri = gnome_vfs_uri_new(appdata->testfile);
	if(gnome_vfs_uri_exists(file_uri))
	{
		hildon_banner_show_information(
				GTK_WIDGET(appdata->window),
				"gtk-dialog-error",
				"File exists. Won't overwrite");
		gnome_vfs_uri_unref(file_uri);
		return;
	}
	gnome_vfs_uri_unref(file_uri);

	/* Open a handle for writing with global read-write
	 * permissions */
	result = gnome_vfs_create(&handle, appdata->testfile,
			GNOME_VFS_OPEN_WRITE, FALSE, 0666);

	/* Check that the handle was properly opened */
	if(result != GNOME_VFS_OK)
	{
		hildon_banner_show_information(
			GTK_WIDGET(appdata->window), "gtk-dialog-error",
			gnome_vfs_result_to_string(result));
		return;
	}

	/* Write the TESTFILE_STRING to the opened file */
	do {
		/* gnome_vfs_write doesn't necessarily write
		 * amount of bytes requested, so this loop
		 * makes sure that the whole string is written */
		result = gnome_vfs_write(handle,
				&testfile_string[bytes_written_total],
				string_len - bytes_written_total,
				&bytes_written);
		bytes_written_total += bytes_written;
				
	} while(bytes_written_total < string_len &&
		result == GNOME_VFS_OK);

	/* Check if the writing went OK */
	if(result != GNOME_VFS_OK)
	{
		hildon_banner_show_information(
			GTK_WIDGET(appdata->window), "gtk-dialog-error",
			gnome_vfs_result_to_string(result));
		goto cleanup;
	}

	/* Notify user that the file was written */
	hildon_banner_show_information(
			GTK_WIDGET(appdata->window), NULL,
			"File written");

	/* Close the opened GnomeVFS handle */
	cleanup:
	gnome_vfs_close(handle);
}

/* Deletes the testfile but only if it has the exact contents
 * of TESTFILE_STRING */
static void delete_testfile(GtkWidget *widget, AppData *appdata)
{
	GnomeVFSResult result;
	GString *data;

	/* Initialize data string and read the file's contents to it */
	data = g_string_new("");
	result = _read_file_to_string(appdata->testfile, data);
	
	/* Check that the reading went fine */
	if(result != GNOME_VFS_OK)
	{
		hildon_banner_show_information(
			GTK_WIDGET(appdata->window), "gtk-dialog-error",
			gnome_vfs_result_to_string(result));
		goto cleanup;
	}

	/* Check that data in the file is exactly what write_testfile
	 * would write (to prevent "oops" when testing) */
	if(strcmp(data->str, TESTFILE_STRING) != 0)
	{
		hildon_banner_show_information(
			GTK_WIDGET(appdata->window), "gtk-dialog-error",
			"Won't delete files not written by this program");
		goto cleanup;
	}

	/* Delete the file */
	result = gnome_vfs_unlink(appdata->testfile);

	/* Check that it was really deleted */
	if(result != GNOME_VFS_OK)
	{
		hildon_banner_show_information(
			GTK_WIDGET(appdata->window), "gtk-dialog-error",
			gnome_vfs_result_to_string(result));
		goto cleanup;
	}
	
	/* Inform the user that the file was deleted */
	hildon_banner_show_information(
		GTK_WIDGET(appdata->window), NULL,
		"Test file deleted");

	/* Free allocated resources */
	cleanup:
	g_string_free(data, TRUE);
}

int main(int argc, char **argv)
{
	AppData *appdata;
	GtkVBox *vbox;
	GtkWidget *write_button, *read_button, *delete_button;
	GtkWidget *uri_label;
	
	appdata = g_new0(AppData, 1);
	example_gui_initialize(&appdata->program,
			&appdata->window,
			&argc, &argv,
			APP_NAME);

	/* Assign argument given at commandline to appdata->testfile
	 * or the DEFAULT_TESTFILE_URI if not specified. */
	if(argc > 1)
	{
		appdata->testfile = argv[1];
	}
	else
	{
		appdata->testfile = DEFAULT_TESTFILE_URI;
	}
	
	/* Initialize GnomeVFS */
	if(!gnome_vfs_init())
	{
		g_critical("Couldn't initialize GnomeVFS\n");
		return 1;
	}

	/* Create the buttons and arrange them to a vbox */
	vbox = GTK_VBOX(gtk_vbox_new(TRUE, 2));
	write_button = gtk_button_new_with_label("Write testfile");
	read_button = gtk_button_new_with_label("Read testfile");
	delete_button = gtk_button_new_with_label("Delete testfile");
	uri_label = gtk_label_new(appdata->testfile);

	gtk_container_add(GTK_CONTAINER(vbox), uri_label);
	gtk_container_add(GTK_CONTAINER(vbox), write_button);
	gtk_container_add(GTK_CONTAINER(vbox), read_button);
	gtk_container_add(GTK_CONTAINER(vbox), delete_button);
	gtk_container_add(GTK_CONTAINER(appdata->window),
			GTK_WIDGET(vbox));

	/* Connect clicked-signals to their callbacks */
	g_signal_connect(G_OBJECT(write_button), "clicked",
			G_CALLBACK(write_testfile), appdata);
	g_signal_connect(G_OBJECT(read_button), "clicked",
			G_CALLBACK(read_testfile), appdata);
	g_signal_connect(G_OBJECT(delete_button), "clicked",
			G_CALLBACK(delete_testfile), appdata);

	/* Run the program */
	example_gui_run(appdata->program,
			appdata->window);

	/* Shut down GnomeVFS and free the heap-allocated appdata */
	gnome_vfs_shutdown();
	g_free(appdata);

	return 0;
}


