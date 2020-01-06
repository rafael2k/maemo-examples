#!/bin/sh
set -x

LIBDIR=`pkg-config libhildondesktop-1 --variable=hildondesktoplibdir`
DESKTOPDIR=`pkg-config libhildondesktop-1 --variable=hildonhomedesktopentrydir`
if [ "x$LIBDIR" = "x" ]; then
	echo "Please check if libhildondesktop-1 is installed!"
	exit -1
else
	mkdir -p $LIBDIR
	mkdir -p $DESKTOPDIR
	cp -f hello-world-home.so $LIBDIR
	cp -f hello-world-home.desktop $DESKTOPDIR
	# icon
	ICONDIR=`pkg-config libhildondesktop-1 --variable=prefix`/share/icons/hicolor
	mkdir -p $ICONDIR/40x40/hildon
	cp hello-world.40.png $ICONDIR/40x40/hildon/hello.png
	gtk-update-icon-cache -f $ICONDIR

	cat << EOF

Make sure that hildon-home is running, and the new applet is configured to be displayed.
Manual way:

mkdir -p ~/.config/hildon-desktop
echo >> ~/.config/hildon-desktop/home.plugins
echo "[hello-world-home.desktop-0]" >> ~/.config/hildon-desktop/home.plugins
echo "X-Desktop-File=/usr/share/applications/hildon-home/hello-world-home.desktop" >> ~/.config/hildon-desktop/home.plugins
echo >> ~/.config/hildon-desktop/home.plugins

EOF
	
fi
