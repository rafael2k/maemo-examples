#!/bin/sh
set -x

LIBDIR=`pkg-config hildon-control-panel --variable=pluginlibdir`
DESKTOPDIR=`pkg-config hildon-control-panel --variable=plugindesktopentrydir`
if [ "x$LIBDIR" = "x" ]; then
	echo "Please check if hildon-conrol-panel is installed!"
	exit -1
else
	mkdir -p $LIBDIR
	mkdir -p $DESKTOPDIR
	cp -f libapplet.so $LIBDIR
	cp -f applet.desktop $DESKTOPDIR
	# icon
	ICONDIR=`pkg-config hildon-control-panel --variable=prefix`/share/icons/hicolor
	mkdir -p $ICONDIR/40x40
	cp hello-world.40.png $ICONDIR/40x40/hello.png
	gtk-update-icon-cache -f $ICONDIR
fi
