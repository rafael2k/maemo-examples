/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */


#include <hildon/hildon-program.h>
#include <hildon/hildon-file-chooser-dialog.h>

#include <gtk/gtk.h>

#include <stdlib.h>
#include <gst/gst.h>
#include <unistd.h>
#include <math.h>
#ifdef HAVE_ESD
#include <esd.h>
#elif HAVE_PULSEAUDIO
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>
#endif

/*---------------------------------------------------------------------------*/
/* Application UI data struct */
typedef struct _AppData AppData;

struct _AppData {
  HildonProgram *app;
  HildonWindow *appview;
  GtkWidget *tb_statusbar;
  gint context_id;
  gchar *filename;
  GstElement *pipeline;
};


/*---------------------------------------------------------------------------*/
#ifdef HAVE_ESD
static int esdlaunch(short *buf, int bufsize)
{
  int sock = -1;

  /* Play 16bit mono stream */
  esd_format_t format = ESD_BITS16 | ESD_MONO | ESD_STREAM | ESD_PLAY;

  sock = esd_play_stream_fallback(format, ESD_DEFAULT_RATE, NULL, "wavlaunch");
  if (sock <= 0) {
    fprintf (stderr, "Error at esd open: %d", sock);
    return -1;
  }

  write(sock, buf, bufsize);

  close(sock);

  return 0;
}
#endif
#ifdef HAVE_PULSEAUDIO
static int pulselaunch(short *buf, int bufsize)
{
    /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };
    pa_simple *s = NULL;
    int ret = -1;
    int error;

     /* Create a new playback stream */
     if (!(s = pa_simple_new(NULL, "example_wavlaunch", PA_STREAM_PLAYBACK, NULL, "wavlaunch", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return ret;
     }
     /* ... and play it */
     if (pa_simple_write(s, buf, (size_t) bufsize, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
        return ret;
     }
     /* Make sure that every single sample was played */
     if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        return ret;
     }
     ret = 0;
     if (s)
               pa_simple_free(s);

     return ret;
    
}
#endif
/*---------------------------------------------------------------------------*/
static int bling(void)
{
  int sts = -1;
  short *buf;
  int bufsize, i, samplestoplay;
  float samplerate, frequency;
  float played, playtime;
  float amplitude, amplitudemax;
  float phase, phasescale;

  samplerate   = 44100.0;
  amplitudemax = 32000.0;
  frequency    =  1000.0;
  playtime     =    0.25;

  samplestoplay = (int)(playtime * samplerate);

  /* allocate buffer for "Bling". 16 bit samples. */
  bufsize = samplestoplay * 2;
  buf = (short *)malloc(bufsize);
  if (buf) {

    /* generate sinewave with linearly decreasing amplitude */
    phasescale = 6.28 * (float)frequency * playtime;
    for (i = 0; i < samplestoplay; i++) {
      played = (float)i / (float)samplestoplay;
      phase = played * phasescale;
      amplitude =  amplitudemax * (1.0 - played);
      buf[i] = (short)(amplitude * sin(phase));
    }

    /* play the buffer */
#ifdef HAVE_ESD
    sts = esdlaunch(buf, bufsize);
#elif HAVE_PULSEAUDIO
    sts = pulselaunch(buf, bufsize);
#endif
    free(buf);
  }

  return sts;  
}


/*---------------------------------------------------------------------------*/
static void eos_message_received (GstBus * bus, GstMessage * message,
    AppData * appdata)
{
  /* stop playback and free pipeline */
  gst_element_set_state (appdata->pipeline, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT(appdata->pipeline));
  appdata->pipeline = NULL;
}


/*---------------------------------------------------------------------------*/
static int play_file (AppData * appdata)
{
  GstElement *filesrc;
  GstBus *bus;
  GError *error = NULL;

  /* we're already playing */
  if (appdata->pipeline) return -1;
  
  /* setup pipeline and configure elements */
  appdata->pipeline = gst_parse_launch (
#ifdef HAVE_PULSEAUDIO
        "filesrc name=my_filesrc ! wavparse ! pulsesink",
#else
        "filesrc name=my_filesrc ! wavparse ! alsasink",
#endif
         &error);
  if (!appdata->pipeline) {
    fprintf (stderr, "Parse error: %s\n", error->message);
    goto error;
  }

  filesrc = gst_bin_get_by_name (GST_BIN (appdata->pipeline), "my_filesrc");
  if (!filesrc) {
    fprintf (stderr, "Parse error: no filesrc\n");
    goto error;
  }

  g_object_set (G_OBJECT (filesrc), "location", appdata->filename, NULL);

  /* setup message handling */
  bus = gst_pipeline_get_bus (GST_PIPELINE (appdata->pipeline));
  gst_bus_add_signal_watch_full (bus, G_PRIORITY_HIGH);
  g_signal_connect (bus, "message::eos", (GCallback) eos_message_received,
      appdata);
  gst_object_unref (GST_OBJECT(bus));
  
  /* start playback */
  gst_element_set_state (appdata->pipeline, GST_STATE_PLAYING);
  return 0;

error:
  gst_object_unref (GST_OBJECT(appdata->pipeline));
  appdata->pipeline = NULL;
  return -1;  
}


/*---------------------------------------------------------------------------*/
static gchar *file_chooser(AppData * appdata)
{
  GtkWidget *dialog = NULL;
  gchar *filename = NULL;

  dialog = hildon_file_chooser_dialog_new(
             GTK_WINDOW(appdata->appview), 
             GTK_FILE_CHOOSER_ACTION_OPEN);
  if (dialog) {
    gint response;

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        "/home/user");
    gtk_widget_show_all(GTK_WIDGET(dialog));

    response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response  == GTK_RESPONSE_OK) {
      filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }

    gtk_widget_destroy(dialog);
  }

  return filename;
}


/*---------------------------------------------------------------------------*/
static void tb_open_cb(GtkToolButton *button, AppData * appdata)
{
  gchar *filename = NULL;

  filename = file_chooser(appdata);

  if (filename) {
    if (appdata->filename) g_free(appdata->filename);
    appdata->filename = g_strdup(filename);

    gtk_statusbar_push (GTK_STATUSBAR (appdata->tb_statusbar),
                        GPOINTER_TO_INT (appdata->context_id),
                        filename);
  }

  /* Play audible bling sound */
  /* bling-function uses ESD, so this 
   * may block the program on Scratchbox. This can be
   * worked around by enabling dmixing in ALSA */  
   bling();

  return;
}


/*---------------------------------------------------------------------------*/
/* Callback for "Play" toolbar button */
static void tb_play_cb(GtkToolButton * button, AppData * appdata)
{
  g_print("Playing file %s\n", appdata->filename );

  play_file( appdata );
}


/*---------------------------------------------------------------------------*/
/* Callback for "Close" menu entry */
static void item_close_cb(void)
{
  g_print("Closing application...\n");

  gtk_main_quit();
}


/*---------------------------------------------------------------------------*/
/* Create the menu items needed for the main view */
static void create_menu(HildonWindow * main_view)
{
  /* Create needed variables */
  GtkMenu *main_menu;
  GtkWidget *item_close;

  GtkWidget *item_separator;


  /* Get the menu from view */
  main_menu = hildon_window_get_menu(main_view);

  if (main_menu) {
    /* Create menu items */
    item_close = gtk_menu_item_new_with_label("Close");
    if (item_close) {
      item_separator = gtk_separator_menu_item_new();

      /* Add menu items */
      gtk_menu_append(main_menu, item_close);

      /* Attach the callback functions to the activate signal */
      g_signal_connect(G_OBJECT(item_close), "activate",
                       GTK_SIGNAL_FUNC(item_close_cb), NULL);

      /* Make all menu widgets visible */
      gtk_widget_show_all(GTK_WIDGET(main_menu));
    }
  }
}



/*---------------------------------------------------------------------------*/
/* Create the toolbar needed for the main view */
static void create_toolbar(HildonWindow * main_view, AppData *appdata)
{
  /* Create needed variables */
  GtkWidget *main_toolbar;
  GtkToolItem *tb_open;
  GtkToolItem *tb_play;
  GtkToolItem *tb_separator;
  GtkToolItem *tb_comboitem;

  /* Create toolbar */
  main_toolbar = gtk_toolbar_new();

  if (main_toolbar) {
  /* Create toolbar button items */
    tb_open = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    tb_play = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);

    /* Create toolbar combobox item */
    tb_comboitem = gtk_tool_item_new();

    appdata->tb_statusbar = gtk_statusbar_new();

    appdata->context_id = gtk_statusbar_get_context_id(
                          GTK_STATUSBAR (appdata->tb_statusbar),
                          "Playing file");

    gtk_statusbar_push (GTK_STATUSBAR (appdata->tb_statusbar),
                        GPOINTER_TO_INT (appdata->context_id),
                        "  Please open pcm wav file to play");


    /* Make combobox to use all available toolbar space */
    gtk_tool_item_set_expand(tb_comboitem, TRUE);

    /* Add combobox inside toolitem */
    gtk_container_add(GTK_CONTAINER(tb_comboitem),
                      GTK_WIDGET(appdata->tb_statusbar));


    /* Create separator */
    tb_separator = gtk_separator_tool_item_new();

    /* Add all items to toolbar */
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_open, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_play, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(main_toolbar), tb_comboitem, -1);

    /* Add signal lister to "Close" button */
    g_signal_connect(G_OBJECT(tb_open), "clicked",
                     G_CALLBACK(tb_open_cb), appdata);
    g_signal_connect(G_OBJECT(tb_play), "clicked",
                     G_CALLBACK(tb_play_cb), appdata);

    /* Add toolbar to 'vbox' of HildonWindow */
    hildon_window_add_toolbar(HILDON_WINDOW(main_view), GTK_TOOLBAR(main_toolbar));
  }
}



/*---------------------------------------------------------------------------*/
/* Main application */
int main(int argc, char *argv[])
{
  /* Create needed variables */
  AppData *appdata;

  HildonProgram *app;
  HildonWindow *appview;

  /* Initialize the GTK and GStreamer. */
  g_thread_init(NULL);
  gtk_init(&argc, &argv);
  gst_init(&argc, &argv);

  /* Create the hildon application and setup the title */
  app = HILDON_PROGRAM(hildon_program_get_instance());
  g_set_application_name("Wav Launch Demo");

  /* Create HildonAppView and set it to HildonApp */
  appview = HILDON_WINDOW(hildon_window_new());

  /* Create AppData */
  appdata = g_new0(AppData, 1);
  if (appdata) {
    GtkWidget *label;

    appdata->app = app;
    appdata->appview = appview;

    /* Add example label to appview */
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
        "<b>GStreamer launch Demo</b>\n\n"
        "Plays pcm wav files.\n\n"
        "Uses gstreamer function gst_parse_launch, with plugins\n"
#ifdef HAVE_PULSEAUDIO
        "filesrc, wavparse and pulsesink.\n\n"
#else
        "filesrc, wavparse and alsasink.\n\n"
#endif
        "Also shows how to listen to messages from the GStreamer bus.\n\n"
        "At file open, audible response is given using esd low level API.\n");

    gtk_container_add(GTK_CONTAINER(appview), label);

      /* Create menu for view */
    create_menu(appview);

    /* Create toolbar for view */
    create_toolbar(appview, appdata);

    /* Begin the main application */
    gtk_widget_show_all(GTK_WIDGET(appview));

    /* Connect signal to X in the upper corner */
    g_signal_connect(G_OBJECT(appview), "delete_event",
                    G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_main();

    if (appdata->pipeline) gst_object_unref(GST_OBJECT(appdata->pipeline));
    if (appdata->filename) g_free(appdata->filename);
    g_free(appdata);
  }    

  /* Exit */
  return 0;
}
