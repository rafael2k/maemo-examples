#!/bin/sh
set -x

LIBDIR=`pkg-config libhildondesktop-1 --variable=hildondesktoplibdir`
DESKTOPDIR=`pkg-config libhildondesktop-1 --variable=hildonstatusmenudesktopentrydir`
if [ "x$LIBDIR" = "x" ]; then
	echo "Please check if libhildondesktop-1 is installed!"
	exit -1
else
	mkdir -p $LIBDIR
	mkdir -p $DESKTOPDIR
	cp -f example_statusmenu.so $LIBDIR
	cp -f example_statusmenu.desktop $DESKTOPDIR
	# icon
	ICONDIR=`pkg-config libhildondesktop-1 --variable=prefix`/share/icons/hicolor
	mkdir -p $ICONDIR/48x48/hildon
	mkdir -p $ICONDIR/16x16/hildon
	cp hello-world.48.png $ICONDIR/48x48/hildon/hello.png
	cp hello-world.16.png $ICONDIR/16x16/hildon/hello.png
	gtk-update-icon-cache -f $ICONDIR

	cat << EOF

Make sure that hildon-status-menu is running for the new applet to be displayed.
Manual way:

run-standalone.sh hildon-status-menu &
EOF
	
fi
