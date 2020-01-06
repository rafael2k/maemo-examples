
/*
 * This file is part of hello-world-app
 *
 * Copyright (C) 2006-2008 Nokia Corporation. All rights reserved.
 *
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the package
 * root directory.
 *
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>

#include <glib.h>
#include <gtk/gtk.h>

#include <libhildondesktop/libhildondesktop.h>

#include "hello-world-home.h"

#define HELLO_HOME_PLUGIN_GET_PRIVATE(obj) \
	        (G_TYPE_INSTANCE_GET_PRIVATE (obj, HELLO_TYPE_HOME_PLUGIN, HelloHomePluginPrivate))

HD_DEFINE_PLUGIN_MODULE (HelloHomePlugin, hello_home_plugin,
			 HD_TYPE_HOME_PLUGIN_ITEM);

GtkDialog *
hello_world_dialog_new ()
{
    GtkWidget *dialog;

    dialog = gtk_dialog_new_with_buttons ("Hello World",
					  NULL,
					  GTK_DIALOG_DESTROY_WITH_PARENT |
					  GTK_DIALOG_NO_SEPARATOR,
					  "Close", GTK_RESPONSE_OK, NULL);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox),
		       gtk_label_new ("Hello World!"));
    gtk_widget_show_all (dialog);

    return GTK_DIALOG (dialog);
}



void
hello_world_dialog_show ()
{
    GtkWidget *dialog = GTK_WIDGET (hello_world_dialog_new ());

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}


GtkWidget *
hello_world_button_new (int padding)
{
    GtkIconTheme *icon_theme;
    GdkPixbuf *icon;
    GtkWidget *icon_image, *button;

    icon_theme = gtk_icon_theme_get_default ();
    icon = gtk_icon_theme_load_icon (icon_theme, "hello", 40, 0, NULL);
    if (icon == NULL)
	icon = gtk_icon_theme_load_icon (icon_theme,
					 "qgn_list_gene_default_app",
					 40, 0, NULL);

    icon_image = gtk_image_new_from_pixbuf (icon);
    gtk_misc_set_padding (GTK_MISC (icon_image), padding, padding);
    g_object_unref (G_OBJECT (icon));
    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), icon_image);

    gtk_widget_show_all (button);

    return button;
}




static void
hello_home_plugin_init (HelloHomePlugin * home_plugin)
{
    GtkWidget *button;

    button = hello_world_button_new (10);
    g_signal_connect (button, "clicked",
		      G_CALLBACK (hello_world_dialog_show), NULL);

    gtk_widget_show_all (button);

    /* Set the resizing behavior */
    hd_home_plugin_item_set_resize_type (HD_HOME_PLUGIN_ITEM (home_plugin),
					 HD_HOME_PLUGIN_ITEM_RESIZE_BOTH);

    gtk_container_add (GTK_CONTAINER (home_plugin), button);
}

static void
hello_home_plugin_class_init (HelloHomePluginClass * klass)
{
}

static void
hello_home_plugin_class_finalize (HelloHomePluginClass * klass)
{
}
