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

#ifndef HELLO_HOME_PLUGIN_H
#define HELLO_HOME_PLUGIN_H

#include <libhildondesktop/libhildondesktop.h>

G_BEGIN_DECLS

/* Common struct types declarations */
typedef struct _HelloHomePlugin HelloHomePlugin;
typedef struct _HelloHomePluginClass HelloHomePluginClass;
typedef struct _HelloHomePluginPrivate HelloHomePluginPrivate;

/* Common macros */
#define HELLO_TYPE_HOME_PLUGIN            (hello_home_plugin_get_type ())
#define HELLO_HOME_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), HELLO_TYPE_HOME_PLUGIN, HelloHomePlugin))
#define HELLO_HOME_PLUGIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  HELLO_TYPE_HOME_PLUGIN, HelloHomePluginClass))
#define HELLO_IS_HOME_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), HELLO_TYPE_HOME_PLUGIN))
#define HELLO_IS_HOME_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  HELLO_TYPE_HOME_PLUGIN))
#define HELLO_HOME_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  HELLO_TYPE_HOME_PLUGIN, HelloHomePluginClass))

/* Instance struct */
struct _HelloHomePlugin
{
	HDHomePluginItem parent;
	HelloHomePluginPrivate *priv;
};

/* Class struct */
struct _HelloHomePluginClass
{
	HDHomePluginItemClass parent_class;
};

GType  hello_home_plugin_get_type  (void);

G_END_DECLS

#endif
