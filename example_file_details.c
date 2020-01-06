/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <hildon/hildon-program.h>
#include <hildon/hildon-fm.h>

#include <gtk/gtk.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>	/* access(), close() */
#include <sys/stat.h>	/* mkdir() */
#include <fcntl.h>	/* open() */

/* Application UI data struct */
typedef struct _AppData AppData;
struct _AppData {
    HildonProgram *program;
    HildonWindow *window;
};

/* This variable will get filled with the
 * filename that was called to run this program
 * and it will be inspected by the file details dialog*/
static char *global_running_file;

#define TEXT_FILE "/MyDocs/.example/foo.txt"

static HildonFileSystemModel* get_file_system_model(GtkWidget *ref_widget)
{
	 return HILDON_FILE_SYSTEM_MODEL(g_object_new(HILDON_TYPE_FILE_SYSTEM_MODEL,
				         "ref_widget", ref_widget, NULL));
}



void callback_file_details(GtkWidget * widget, AppData * data)
{
    HildonFileDetailsDialog *dialog;
    HildonFileSystemModel *model = NULL;
    gint result;
    GtkTreeIter iter;

    /* Create dialog */
    /* DEPRECATED
    dialog = HILDON_FILE_DETAILS_DIALOG
        (hildon_file_details_dialog_new(GTK_WINDOW(data->window), global_running_file)); */

    if( (model = get_file_system_model(GTK_WIDGET(data->window)) ) == NULL)
    {
       g_print("could not get file system model\n\n");
       return;
    }
    if( !hildon_file_system_model_load_uri( model,
                                            global_running_file,
                                            &iter ) )
    {
        g_print("could not load uri %s\n", global_running_file);
        return;
    }
    
    dialog = HILDON_FILE_DETAILS_DIALOG
              (hildon_file_details_dialog_new_with_model(GTK_WINDOW(data->window), model));

    hildon_file_details_dialog_set_file_iter
       (HILDON_FILE_DETAILS_DIALOG(dialog), &iter);
    
    /* custom information can be added to the dialog as below */
    hildon_file_details_dialog_add_label_with_value(dialog, "newlabel", "newvalue");

    /* Show the dialog */
    gtk_widget_show_all(GTK_WIDGET(dialog));

    /* Wait for user to select OK or CANCEL */
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    /* Close the dialog */
    gtk_widget_destroy(GTK_WIDGET(dialog));

}

char *dir_name(const char *path)
{
  char *result_path;
  const char *slash;
  int len;			/* Length of result, not including NUL.  */

  slash = strrchr (path, '/');
  if (slash == 0) {
      /* does not contain '/', file is in the current directory  */
      path = ".";
      len = 1;
  } else {
      /* remove trailing slashes  */
      while (slash > path && *slash == '/')
	  --slash;

      len = slash - path + 1;
  }
  result_path = (char *)malloc(len + 1);
  if (result_path == 0)
      return 0;
  strncpy(result_path, path, len);
  result_path[len] = 0;
  return result_path;
}


int main(int argc, char *argv[])
{
    /* Create needed variables */
    HildonProgram *program;
    HildonWindow *window;
    GtkWidget *button;
    GtkWidget *vbox;
    AppData *appdata;
    char text_file[256] = "";
    int fp;
    char *parent_dir;

    /* Initialize the GTK. */
    gtk_init(&argc, &argv);

    
    sprintf(text_file,"%s/%s", getenv("HOME"), TEXT_FILE);

    /* Check if the file exists */
    if (access(text_file, F_OK) == -1) {
        /* File does not exist, create it */
	parent_dir = dir_name(text_file);
	if (parent_dir) {
	    mkdir(parent_dir, 0777);
	    free(parent_dir);
	}
	fp = open(text_file,O_CREAT|O_RDWR|O_TRUNC, 0644);
	if (fp > 0)
	    close(fp);
	else {
	    g_error("Could not create %s: %s\n", text_file, strerror(errno));
	    return 1;		
	}
    }
    
    /** File details dialog needs an absolute path */
    if(!(global_running_file = realpath(text_file, NULL)))
    {
	    g_error("Couldn't get absolute path (for %s): %s", text_file, strerror(errno));
	    return 1;
    }
    
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
    button = gtk_button_new_with_label("File Details");
    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(callback_file_details), appdata);
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
