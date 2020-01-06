/**
 * This file is part of maemo-examples package
 * 
 * Copyright (c) 2009 Nokia Corporation. All rights reserved.
 * @author Marcell Lengyel <marcell.lengyel@nokia.com>
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 */

#include <string.h>
#include <gtk/gtk.h>
#include <hildon/hildon.h>
#include <libosso.h>

#include "example_statusmenu.h"

#define EXAMPLE_STATUS_MENU_ITEM_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (obj, EXAMPLE_TYPE_STATUS_MENU_ITEM, ExampleStatusMenuItemPrivate))

#define STATUSMENU_EXAMPLE_LIBOSSO_SERVICE_NAME "example_status_menu_item"
#define STATUSMENU_EXAMPLE_ICON_SIZE         48
#define STATUSMENU_EXAMPLE_AREA_ICON_SIZE    16

struct _ExampleStatusMenuItemPrivate
{
  /* Widgets */
  GtkWidget *ex_icon;
  GtkWidget *ex_label;

  /* Other stuff */
  osso_context_t *osso;
};

HD_DEFINE_PLUGIN_MODULE (ExampleStatusMenuItem, example_status_menu_item, HD_TYPE_STATUS_MENU_ITEM);

static void
example_status_menu_item_set_area_icon (ExampleStatusMenuItem *item)
{
  /* ExampleStatusMenuItemPrivate *priv = item->priv; */
  GdkPixbuf *pixbuf = NULL;
  GtkIconTheme *icon_theme;


  icon_theme = gtk_icon_theme_get_default();
  pixbuf = gtk_icon_theme_load_icon(icon_theme, "hello",
				    STATUSMENU_EXAMPLE_AREA_ICON_SIZE,
				    GTK_ICON_LOOKUP_NO_SVG,
				    NULL);

  /* Set the small status area icon */
  hd_status_plugin_item_set_status_area_icon (HD_STATUS_PLUGIN_ITEM(item),
                                              pixbuf);
  if (pixbuf != NULL)
        g_object_unref(pixbuf);

}

/**
 * Callback for the button clicked signal
 */
static void example_status_menu_icon_pressed_cb(GtkButton *button,
                                                ExampleStatusMenuItem *item)
{
    /* ExampleStatusMenuItemPrivate *priv = item->priv; */
	
    /* Change ex_icon, ex_label, etc */
    hildon_banner_show_information(NULL, NULL, "foo");
}



static void
example_status_menu_item_class_finalize (ExampleStatusMenuItemClass *klass)
{
}

static void
example_status_menu_item_finalize (GObject *object)
{
  ExampleStatusMenuItem *item =
    EXAMPLE_STATUS_MENU_ITEM (object);
  ExampleStatusMenuItemPrivate *priv = item->priv;

  if(priv->osso)
    {
      osso_deinitialize(priv->osso);
      priv->osso = NULL;
    }

  G_OBJECT_CLASS (example_status_menu_item_parent_class)->finalize (object);
}

static void
example_status_menu_item_class_init (ExampleStatusMenuItemClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = example_status_menu_item_finalize;

  g_type_class_add_private (klass, sizeof (ExampleStatusMenuItemPrivate));
}

static void
example_status_menu_item_create_widgets (ExampleStatusMenuItem *item)
{
  ExampleStatusMenuItemPrivate *priv = item->priv;
  GtkWidget *button;
  GtkWidget *hbox, *vbox1, *vbox2;
  GtkWidget *image;
  GtkWidget *label1, *label2;
  GtkIconTheme *icon_theme;
  GdkPixbuf *pixbuf;

 /* Button with icon and two rows of text */
  button = gtk_button_new ();
  gtk_container_set_border_width (GTK_CONTAINER (button), 3);
  hbox = gtk_hbox_new (FALSE, 8);
  vbox1 = gtk_vbox_new (FALSE, 0);
  vbox2 = gtk_vbox_new (FALSE, 0);
  icon_theme = gtk_icon_theme_get_default();
  pixbuf = gtk_icon_theme_load_icon(icon_theme, "hello",
		 		    STATUSMENU_EXAMPLE_ICON_SIZE,
		                    GTK_ICON_LOOKUP_NO_SVG, NULL);
  image = gtk_image_new_from_pixbuf (pixbuf);
  if (pixbuf != NULL)
        g_object_unref(pixbuf);

  label1 = gtk_label_new ("Hello World");
  gtk_misc_set_alignment (GTK_MISC (label1), 0.0, 0.5);
  hildon_helper_set_logical_color(label1, GTK_RC_TEXT, GTK_STATE_NORMAL, "MenuInfoTextColor");
  hildon_helper_set_logical_font(label1, "SystemFont");

  label2 = gtk_label_new ("example text");
  gtk_misc_set_alignment (GTK_MISC (label2), 0.0, 0.5);
  hildon_helper_set_logical_color(label2, GTK_RC_TEXT, GTK_STATE_NORMAL, "MenuInfoTextColor");
  hildon_helper_set_logical_font(label2, "SmallSystemFont");

  gtk_container_add (GTK_CONTAINER (button), hbox);
  gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), label1, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), label2, FALSE, FALSE, 0);
  priv->ex_label = label2;
  priv->ex_icon = image;

  g_signal_connect_after (G_OBJECT (button), "clicked",
                          G_CALLBACK (example_status_menu_icon_pressed_cb),
                          item);
  gtk_container_add (GTK_CONTAINER (item), button);
  example_status_menu_item_set_area_icon(item);
  gtk_widget_show_all (GTK_WIDGET (item));
}

static void
example_status_menu_item_init (ExampleStatusMenuItem *item)
{
  ExampleStatusMenuItemPrivate *priv;

  priv = item->priv = EXAMPLE_STATUS_MENU_ITEM_GET_PRIVATE (item);

  /* Initialize */
  example_status_menu_item_create_widgets (item);
  priv->osso = osso_initialize (STATUSMENU_EXAMPLE_LIBOSSO_SERVICE_NAME,
                                "0.1",
                                TRUE,
                                NULL);

}


