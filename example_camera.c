/**
 * This file is part of maemo-examples package
 * 
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 * Copyright (c) 2006 INdT.
 * @author Talita Menezes <talita.menezes@indt.org.br>
 * @author Cidorvan Leite <cidorvan.leite@indt.org.br>
 * @author Jami Pekkanen <jami.pekkanen@nokia.com>
 * 
  * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 */

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
#include "example_common.h"


/* Define save folder and output picture name. */
#define SAVE_FOLDER_DEFAULT  	 NULL
#define PHOTO_NAME_DEFAULT	 "Picture"
#define PHOTO_NAME_SUFFIX_DEFAULT ".jpg"
#define DEFAULT_IMAGEDIR "/MyDocs/.images/"

/* Define sources and sinks according to
 * running environment
 * NOTE: If you want to run the application
 * in ARM scratchbox, you have to change these*/
#ifdef __arm__
/* The device by default supports only
 * vl4l2src for camera and xvimagesink
 * for screen */
#define VIDEO_SRC "v4l2camsrc"
#define VIDEO_SINK "xvimagesink"
#else
/* These are for the X86 SDK. Xephyr doesn't
 * support XVideo extension, so the application
 * must use ximagesink. The video source depends
 * on driver of your Video4Linux device so this
 * may have to be changed - one can try autovideosrc
 * or even videotestsrc
 */
#define VIDEO_SRC "v4l2src"
#define VIDEO_SINK "ximagesink"
#endif

/* Define structure for variables that
 * are needed thruout the application */
typedef struct
{
	HildonProgram *program;
	HildonWindow *window;

	GstElement *pipeline;
	GtkWidget *screen;
	guint buffer_cb_id;
} AppData;

static gboolean create_jpeg(unsigned char *buffer);

/* This callback will be registered to the image sink
 * after user requests a photo */
static gboolean buffer_probe_callback(
		GstElement *image_sink,
		GstBuffer *buffer, GstPad *pad, AppData *appdata)
{
	GstMessage *message;
	gchar *message_name;
	/* This is the raw RGB-data that image sink is about
	 * to discard */
	unsigned char *data_photo =
	    (unsigned char *) GST_BUFFER_DATA(buffer);

	/* Create a JPEG of the data and check the status */
	if(!create_jpeg(data_photo))
		message_name = "photo-failed";
	else
		message_name = "photo-taken";
	
	/* Disconnect the handler so no more photos
	 * are taken */
	g_signal_handler_disconnect(G_OBJECT(image_sink),
			appdata->buffer_cb_id);
	
	/* Create and send an application message which will be
	 * catched in the bus watcher function. This has to be
	 * sent as a message because this callback is called in
	 * a gstreamer thread and calling GUI-functions here would
	 * lead to X-server synchronization problems */
	message = gst_message_new_application(GST_OBJECT(appdata->pipeline),
			gst_structure_new(message_name, NULL));
	gst_element_post_message(appdata->pipeline, message);
	
	/* Returning TRUE means that the buffer can is OK to be
	 * sent forward. When using fakesink this doesn't really
	 * matter because the data is discarded anyway */
	return TRUE;
}

/* Callback that gets called when user clicks the "Take photo" button */
static void take_photo(GtkWidget *widget, AppData *appdata)
{
	GstElement *image_sink;
	
	/* Get the image sink element from the pipeline */
	image_sink = gst_bin_get_by_name(GST_BIN(appdata->pipeline),
			"image_sink");
	/* Display a note to the user */
	hildon_banner_show_information(GTK_WIDGET(appdata->window),
		NULL, "Taking Photo");

	/* Connect the "handoff"-signal of the image sink to the
	 * callback. This gets called whenever the sink gets a
	 * buffer it's ready to pass forward on the pipeline */
	appdata->buffer_cb_id = g_signal_connect(
			G_OBJECT(image_sink), "handoff",
			G_CALLBACK(buffer_probe_callback), appdata);
}

/* Callback that gets called whenever pipeline's message bus has
 * a message */
static void bus_callback(GstBus *bus, GstMessage *message, AppData *appdata)
{
	gchar *message_str;
	const gchar *message_name;
	GError *error;
	
	/* Report errors to the console */
	if(GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR)
	{	
		gst_message_parse_error(message, &error, &message_str);
		g_warning("GST error: %s, %s\n", error->message, message_str);
		g_error_free(error);
		g_free(message_str);
		gtk_main_quit();
	}
	
	/* Report warnings to the console */
	if(GST_MESSAGE_TYPE(message) == GST_MESSAGE_WARNING)
	{	
		gst_message_parse_warning(message, &error, &message_str);
		g_warning("GST warning: %s, %s\n", error->message, message_str);
		g_error_free(error);
		g_free(message_str);
	}

	/* See if the message type is GST_MESSAGE_APPLICATION which means
	 * thet the message is sent by the client code (this program) and
	 * not by gstreamer. */
	if(GST_MESSAGE_TYPE(message) == GST_MESSAGE_APPLICATION)
	{
		/* Get name of the message's structure */
		message_name = gst_structure_get_name(gst_message_get_structure(message));
		
		/* The hildon banner must be shown in here, because the bus callback is
		 * called in the main thread and calling GUI-functions in gstreamer threads
		 * usually leads to problems with X-server */
		
		/* "photo-taken" message means that the photo was succefully taken
		 * and saved and message is shown to user */
		if(!strcmp(message_name, "photo-taken"))
		{
			hildon_banner_show_information(
					GTK_WIDGET(appdata->window),
					NULL, "Photo taken");
		}
		
		/* "photo-failed" means that the photo couldn't be captured or saved */	
		if(!strcmp(message_name, "photo-failed"))
		{
			hildon_banner_show_information(
					GTK_WIDGET(appdata->window),
					"gtk-dialog-error",
					 "Saving photo failed");
		}
	}
			
}

/* Callback to be called when the screen-widget is exposed */
static gboolean expose_cb(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{

	/* Tell the xvimagesink/ximagesink the x-window-id of the screen
	 * widget in which the video is shown. After this the video
	 * is shown in the correct widget */
	gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(data),
				     GDK_WINDOW_XWINDOW(widget->window));
	return FALSE;
}

/* Initialize the the Gstreamer pipeline. Below is a diagram
 * of the pipeline that will be created:
 * 
 *                   |Screen|  |Screen|
 *                 ->|queue |->|sink  |-> Display
 * |Camera|  |Tee|/
 * |src   |->|   |\  |Image|   |Image |  |Image|
 *                 ->|queue|-> |filter|->|sink |-> JPEG file
 *
 * gst-launch v4l2camsrc ! "video/x-raw-yuv,format=(fourcc)UYVY,width=640,heigth=480,framerate=(fraction)25/1" ! xvimagesink
 */
static gboolean initialize_pipeline(AppData *appdata,
		int *argc, char ***argv)
{
	GstElement *pipeline, *camera_src, *screen_sink, *image_sink;
	GstElement *screen_queue, *image_queue;
	GstElement *image_filter, *tee;
	GstCaps *caps;
	GstBus *bus;


	/* Initialize Gstreamer */
	gst_init(argc, argv);
	
	/* Create pipeline and attach a callback to it's
	 * message bus */
	pipeline = gst_pipeline_new("test-camera");

	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	gst_bus_add_watch(bus, (GstBusFunc)bus_callback, appdata);
	gst_object_unref(GST_OBJECT(bus));
	
	/* Save pipeline to the AppData structure */
	appdata->pipeline = pipeline;
	
	/* Create elements */
	/* Camera video stream comes from a Video4Linux driver */
	camera_src = gst_element_factory_make(VIDEO_SRC, "camera_src");
	/* Tee that copies the stream to multiple outputs */
	tee = gst_element_factory_make("tee", "tee");
	/* Queue creates new thread for the stream */
	screen_queue = gst_element_factory_make("queue", "screen_queue");
	/* Sink that shows the image on screen. Xephyr doesn't support XVideo
	 * extension, so it needs to use ximagesink, but the device uses
	 * xvimagesink */
	screen_sink = gst_element_factory_make(VIDEO_SINK, "screen_sink");
	/* Creates separate thread for the stream from which the image
	 * is captured */
	image_queue = gst_element_factory_make("queue", "image_queue");
	/* Filter to convert stream to use format that the gdkpixbuf library
	 * can use */
	image_filter = gst_element_factory_make("ffmpegcolorspace", "image_filter");
	/* A dummy sink for the image stream. Goes to bitheaven */
	image_sink = gst_element_factory_make("fakesink", "image_sink");

	/* Check that elements are correctly initialized */
	if(!(pipeline && camera_src && screen_sink && screen_queue
		&& image_queue && image_filter && image_sink))
	{
		g_critical("Couldn't create pipeline elements");
		return FALSE;
	}

	/* Set image sink to emit handoff-signal before throwing away
	 * it's buffer */
	g_object_set(G_OBJECT(image_sink),
			"signal-handoffs", TRUE, NULL);
	
	/* Add elements to the pipeline. This has to be done prior to
	 * linking them */
	gst_bin_add_many(GST_BIN(pipeline), camera_src,
			tee, screen_queue, screen_sink, image_queue,
			image_filter, image_sink, NULL);
	
	/* Specify what kind of video is wanted from the camera */
	caps = gst_caps_new_simple("video/x-raw-yuv",
			"format",  GST_TYPE_FOURCC, GST_MAKE_FOURCC ('U', 'Y', 'V', 'Y'),
			"width", G_TYPE_INT, 640,
			"height", G_TYPE_INT, 480,
			"framerate", GST_TYPE_FRACTION, 25, 1,
			NULL);
			

	/* Link the camera source and colorspace filter using capabilities
	 * specified */
	if(!gst_element_link_filtered(camera_src, tee, caps))
	{
		return FALSE;
	}
	gst_caps_unref(caps);
	
	/* Connect Colorspace Filter -> Tee -> Screen Queue -> Screen Sink
	 * This finalizes the initialization of the screen-part of the pipeline */
	if(!gst_element_link_many(tee, screen_queue, screen_sink, NULL))
	{
		return FALSE;
	}

	/* gdkpixbuf requires 8 bits per sample which is 24 bits per
	 * pixel */
	caps = gst_caps_new_simple("video/x-raw-rgb",
			"width", G_TYPE_INT, 640,
			"height", G_TYPE_INT, 480,
			NULL);
			
	/* Link the image-branch of the pipeline. The pipeline is
	 * ready after this */
	if(!gst_element_link_many(tee, image_queue, image_filter, NULL)) return FALSE;
	if(!gst_element_link_filtered(image_filter, image_sink, caps)) return FALSE;

	gst_caps_unref(caps);
	
	/* As soon as screen is exposed, window ID will be advised to the sink */
	g_signal_connect(appdata->screen, "expose-event", G_CALLBACK(expose_cb),
			 screen_sink);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	return TRUE;
}

/* Destroy the pipeline on exit */
static void destroy_pipeline(GtkWidget *widget, AppData *appdata)
{
	/* Free the pipeline. This automatically also unrefs all elements
	 * added to the pipeline */
	gst_element_set_state(appdata->pipeline, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(appdata->pipeline));
}

static gboolean
start_pipeline (AppData *appdata)
{
	GstStateChangeReturn sret;

	sret = gst_element_set_state (appdata->pipeline, GST_STATE_PLAYING);
	if (sret == GST_STATE_CHANGE_FAILURE) {
		destroy_pipeline(GTK_WIDGET(appdata->window),appdata);
		gtk_main_quit();
	}
	return FALSE;
}

int main(int argc, char **argv)
{
	AppData appdata;
	GtkWidget *button, *hbox, *vbox_button, *vbox;

	/* Initialize and create the GUI */
	
	example_gui_initialize(
		&appdata.program, &appdata.window,
		&argc, &argv, "Camera example");

	vbox = gtk_vbox_new(FALSE, 0);
	hbox = gtk_hbox_new(FALSE, 0);
	vbox_button = gtk_vbox_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox_button, FALSE, FALSE, 0);

	appdata.screen = gtk_drawing_area_new();
	gtk_widget_set_size_request(appdata.screen, 500, 380);
	gtk_box_pack_start(GTK_BOX(vbox), appdata.screen, FALSE, FALSE, 0);

	button = gtk_button_new_with_label("Take photo");
	gtk_widget_set_size_request(button, 170, 380);
	gtk_box_pack_start(GTK_BOX(vbox_button), button, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(take_photo), &appdata);
	gtk_container_add(GTK_CONTAINER(appdata.window), hbox);

	/* Initialize the GTK pipeline */
	if(!initialize_pipeline(&appdata, &argc, &argv))
	{
		hildon_banner_show_information(
				GTK_WIDGET(appdata.window),
				"gtk-dialog-error",
				"Failed to initialize pipeline");
	}

	g_signal_connect(G_OBJECT(appdata.window), "destroy",
			G_CALLBACK(destroy_pipeline), &appdata);
			
	g_idle_add ((GSourceFunc)start_pipeline, &appdata);

	/* Begin the main application */
	example_gui_run(appdata.program, appdata.window);

	/* Free the gstreamer resources. Elements added
	 * to the pipeline will be freed automatically */
	
	return 0;
}

/* Creates a jpeg file from the buffer's raw image data */
static gboolean create_jpeg(unsigned char *data)
{
	GdkPixbuf *pixbuf = NULL;
	GError *error = NULL;
	guint height, width, bpp;
	gchar *directory;
	GString *filename;
	guint base_len, i;
	struct stat statbuf;

	width = 640; height = 480; bpp = 24;

	directory = g_strconcat (g_get_home_dir (), DEFAULT_IMAGEDIR, NULL);
	printf("Directory: %s\n", directory);
	

	/* Create an unique file name */
	filename = g_string_new(g_build_filename(directory, PHOTO_NAME_DEFAULT, NULL));
	base_len = filename->len;
	g_string_append(filename, PHOTO_NAME_SUFFIX_DEFAULT);
	for(i = 1; !stat(filename->str, &statbuf); ++i)
	{
		g_string_truncate(filename, base_len);
		g_string_append_printf(filename, "%d%s", i, PHOTO_NAME_SUFFIX_DEFAULT);
	}


	/* Create a pixbuf object from the data */
	pixbuf = gdk_pixbuf_new_from_data(data,
			GDK_COLORSPACE_RGB, /* RGB-colorspace */
			FALSE, /* No alpha-channel */
			bpp/3, /* Bits per RGB-component */
			width, height, /* Dimensions */
			3*width, /* Number of bytes between lines (ie stride) */
			NULL, NULL); /* Callbacks */

	/* Save the pixbuf content's in to a jpeg file and check for
	 * errors */
	if(!gdk_pixbuf_save(pixbuf, filename->str, "jpeg", &error, NULL))
	{
		g_warning("%s\n", error->message);
		g_error_free(error);
		gdk_pixbuf_unref(pixbuf);
		g_string_free(filename, TRUE);
		return FALSE;
	}
	
	/* Free allocated resources and return TRUE which means
	 * that the operation was succesful */
	g_string_free(filename, TRUE);
	gdk_pixbuf_unref(pixbuf);
	return TRUE;
}


