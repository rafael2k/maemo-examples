/**
 * example_drawing.c
 * Copyright (c) 2006 Tomi Juhola (tomi.juhola@tieturi.fi)
 * Copyright (C) 2007-2008 Nokia Corporation. All rights reserved.
 *
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 */

#include <hildon/hildon-program.h>

#include <gtk/gtk.h>

#include "example_common.h"

typedef struct
{
  HildonProgram *program;
  HildonWindow *window;
  /* Pixmap for drawing area */
  GdkPixmap *pixmap;
  /* Widget that holds information about actions */
  GtkWidget* xyLabel;
} AppData;

/* 
 * CB used fo creating a new pixmap, called in the beginning
 */
gboolean configure_event( GtkWidget *widget, GdkEventConfigure *event, AppData *appdata)
{
/* If pixmap already exists */
  if (appdata->pixmap)
    g_object_unref(appdata->pixmap);

/* Create a new pixmap of the right size (that is the allocation size of the window */
  appdata->pixmap = gdk_pixmap_new(widget->window,
			  widget->allocation.width,
			  widget->allocation.height,
			  -1);
  
/* Draw a white rectangle to initialize pixmap as white */
  gdk_draw_rectangle(appdata->pixmap,
		      widget->style->white_gc, /* White color */
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);

  return TRUE;
}

/* 
 * Method for drawing a rectangle on pixbuf and queue it for draw 
 */
int draw_item(GtkWidget *widget, gdouble x, gdouble y, GdkPixmap *pixmap)
{
  /* The area to be updated */
  GdkRectangle update_rect;

  update_rect.x = x - 5;
  update_rect.y = y - 5;
  update_rect.width = 10;
  update_rect.height = 10;

  /* Draw the point */
  gdk_draw_point(pixmap, widget->style->black_gc, x,y);

  /* Queue the are where the point was drawn */
  gtk_widget_queue_draw_area (widget, 		      
                              update_rect.x, update_rect.y,
		              update_rect.width, update_rect.height);
  return 0;
}


/* 
 * CB used for catching button press events
 */
gboolean button_press_event( GtkWidget *widget, GdkEventButton *event, AppData *appdata)
{
  if (event->button == 1 && appdata->pixmap != NULL)
  {
	char text[64]="";

	draw_item(widget, event->x, event->y, appdata->pixmap);
	
	sprintf(text, "You pressed point(%d,%d)", (int)event->x, (int)event->y);
	g_object_set(appdata->xyLabel, "label",text, NULL);
  }

  return TRUE;
}

/* 
 *CB for redrawing the screen from the pixmap
 */
gboolean expose_event( GtkWidget *widget, GdkEventExpose *event, AppData *appdata)
{
  
	gdk_draw_drawable(widget->window,
		    widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		    appdata->pixmap,
		    event->area.x, event->area.y,
		    event->area.x, event->area.y,
		    event->area.width, event->area.height);

  return FALSE;
}

/*
 * Main method
 */
int main(int argc, char** argv)
{
  AppData *appdata;
  GtkWidget *drawing_area;
  GtkWidget *vbox;

  /* Allocate an AppData structure */
  appdata = g_new0(AppData, 1);
  /* Initialize the GUI */
  example_gui_initialize(
		  &appdata->program, &appdata->window,
		  &argc, &argv,
		  "Drawing example");

  /* Create the drawing area widget */
  drawing_area = gtk_drawing_area_new();
  
  /* Create the Label for showing information */
  appdata->xyLabel = (GtkWidget*)gtk_object_new(GTK_TYPE_LABEL, 
  		"label", "Information about clicks displayed here!", 
		NULL);

  /* Connecting signals to drawing area */
  gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
		      (GtkSignalFunc) expose_event, appdata);
  gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
		      (GtkSignalFunc) configure_event, appdata);
  gtk_signal_connect (GTK_OBJECT (drawing_area), "button_press_event",
		      (GtkSignalFunc) button_press_event, appdata);

  /* Set event mask for drawing_area */
  gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK
			 | GDK_LEAVE_NOTIFY_MASK
			 | GDK_BUTTON_PRESS_MASK);

  /* Create a layout box for the window since it can only hold one widget */
  vbox = gtk_vbox_new(FALSE, 0);

  /* Add the vbox as a child to the window */
  gtk_container_add(GTK_CONTAINER(appdata->window), vbox); 

  /* Pack the label and drawing_area into the VBox. */
  gtk_box_pack_end(GTK_BOX(vbox), appdata->xyLabel,FALSE,FALSE,0);
  gtk_box_pack_end(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

  /* Display the window and go to the main loop */
  example_gui_run(appdata->program, appdata->window);  
  
  return 0;
}
