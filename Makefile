VERSION=2.0-9
DESTDIR=/
CC=gcc
LIBRARIES=gconf-2.0 hildon-1 hildon-fm-2 gtk+-2.0 libosso gdk-2.0 gconf-2.0 gnome-vfs-2.0
EXTRA_LIBRARIES=libebook-1.2 alarm bluez libgwobex osso-gpsd  osso-addressbook-1.0 gpsbt
CFLAGS= -Wall -pedantic `pkg-config --cflags  $(LIBRARIES)`
EXTRA_CFLAGS= -Wall -pedantic  `pkg-config --cflags $(LIBRARIES) $(EXTRA_LIBRARIES)`
CDEBUG=-g
LDFLAGS=`pkg-config --libs $(LIBRARIES)`
# libgstinterfaces-0.10 is defined here to work around
# a bug in gstreamer's pkg-config
EXTRA_LDFLAGS=`pkg-config --libs $(LIBRARIES) $(EXTRA_LIBRARIES)`

ifdef DEBUG
CFLAGS += $(CDEBUG)
LDFLAGS += $(CDEBUG)
endif

EXAMPLES=\
	example_backup_server \
	example_backup_client \
	example_banner \
	example_color_selector \
	example_context \
	example_findtoolbar \
	example_file_chooser \
	example_file_details \
	example_font_selector \
	example_gconf \
	example_gnomevfs \
	example_hard_keys \
	example_hildonprogram \
	example_libosso \
	example_menu \
	example_message \
	example_quickstart \
	example_toolbar \
	example_drawing \
	example_gconf_subscr

EXTRA_EXAMPLES=\
	example_abook \
	example_bluetooth \
	example_camera \
	example_global_search \
	example_gps \
	example_alarm
#	example_wavlaunch \

LIBEXAMPLES=\
	libapplet.so \
	hello-world-home.so \
	example_statusmenu.so

AUX_FILES=Makefile README README.gconf COPYING \
	  example_backup_client.desktop \
	  example_backup_server.desktop \
	  example_backup_client.service \
	  example_help_framework.service \
	  example_libosso.desktop \
	  example_libosso.service \
	  install_backup_client.sh \
	  install_controlpanel_applet.sh \
	  install_example_libosso.sh \
	  hildon-sb-helloworld.desktop
	
	

all: $(EXAMPLES) $(LIBEXAMPLES) example_bluetooth

ICON_DIR=$(DESTDIR)`pkg-config libhildondesktop-1 --variable=prefix`/share/icons/hicolor
SM_LIB_DIR=$(DESTDIR)`pkg-config libhildondesktop-1 --variable=hildondesktoplibdir`
SM_DESKTOP_DIR=$(DESTDIR)`pkg-config libhildondesktop-1 --variable=hildonstatusmenudesktopentrydir`
BIN_DIR=$(DESTDIR)`pkg-config osso-af-settings --variable=prefix`/bin
DESKTOP_DIR=$(DESTDIR)`pkg-config osso-af-settings --variable=desktopentrydir`
DBUS_SERVICE_DIR=$(DESTDIR)`pkg-config osso-af-settings --variable=dbusservicedir`
CP_LIB_DIR=$(DESTDIR)`pkg-config hildon-control-panel --variable=pluginlibdir`
CP_DESKTOP_DIR=$(DESTDIR)`pkg-config hildon-control-panel --variable=plugindesktopentrydir`
HOME_LIB_DIR=$(DESTDIR)`pkg-config osso-af-settings --variable=hildondesktoplibdir`
HOME_DESKTOP_DIR=$(DESTDIR)`pkg-config osso-af-settings --variable=homedesktopentrydir`
MIME_DIR=$(DESTDIR)`pkg-config osso-af-settings --variable=prefix`/share/mime/packages

INSTALL_FILES=$(addsuffix .c, $(EXAMPLES))\
		$(addsuffix .c, $(EXTRA_EXAMPLES))\
       		$(patsubst %.so, %.c, $(LIBEXAMPLES))\
       		$(AUX_FILES) \

INSTALL_DIR=$(DESTDIR)/usr/share/doc/maemo-examples
#HELP_INSTALL_DIR=$(DESTDIR)/usr/share/osso-help/$(LANG)
HELP_INSTALL_DIR=$(DESTDIR)/usr/share/osso-help/en_GB

install: install_help
	mkdir -p $(ICON_DIR)/48x48/hildon/
	mkdir -p $(ICON_DIR)/40x40/hildon/
	mkdir -p $(ICON_DIR)/26x26/hildon/
	mkdir -p $(ICON_DIR)/16x16/hildon/
	mkdir -p $(ICON_DIR)/scalable/hildon/
	mkdir -p $(ICON_DIR)/scalable/mimetypes/
	mkdir -p $(SM_LIB_DIR)
	mkdir -p $(SM_DESKTOP_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(DESKTOP_DIR)
	mkdir -p $(DBUS_SERVICE_DIR)
	mkdir -p $(CP_LIB_DIR)
	mkdir -p $(CP_DESKTOP_DIR)
	mkdir -p $(HOME_LIB_DIR)
	mkdir -p $(HOME_DESKTOP_DIR)
	mkdir -p $(MIME_DIR)
	cp 3d-gnu-head.png $(ICON_DIR)/scalable/hildon/3d-gnu-head.png
	cp hello-world.48.png $(ICON_DIR)/48x48/hildon/hello.png
	cp hello-world.40.png $(ICON_DIR)/40x40/hildon/hello.png
	cp hello-world.26.png $(ICON_DIR)/26x26/hildon/hello.png
	cp hello-world.16.png $(ICON_DIR)/16x16/hildon/hello.png
	cp example_statusmenu.so $(SM_LIB_DIR)
	cp example_statusmenu.desktop $(SM_DESKTOP_DIR)
	cp hello-world-home.so $(HOME_LIB_DIR)
	cp hello-world-home.desktop $(HOME_DESKTOP_DIR)
	cp example_*.desktop $(DESKTOP_DIR)
	cp applet.desktop $(CP_DESKTOP_DIR)
	cp libapplet.so $(CP_LIB_DIR)
	cp $(EXAMPLES) $(BIN_DIR)
	cp example_wavlaunch $(BIN_DIR)
	cp example_camera $(BIN_DIR)
	cp example_bluetooth $(BIN_DIR)
#	cp $(EXTRA_EXAMPLES) $(BIN_DIR)
	cp *.service $(DBUS_SERVICE_DIR)
	cp example-mime.xml $(MIME_DIR)
	ln -s ../hildon/3d-gnu-head.png $(ICON_DIR)/scalable/mimetypes/gnome-mime-application-x-example.png

.PHONY: install_help
install_help:	example.xml
	mkdir -p $(HELP_INSTALL_DIR)
	cp example.xml $(HELP_INSTALL_DIR)
	

#$(EXTRA_EXAMPLES): %: %.c
#	    $(CC) $(EXTRA_CFLAGS) $(EXTRA_LDFLAGS) -o $@ $<

$(LIBEXAMPLES): %.so: %.c
	    $(CC) -shared -fPIC  -Wall `pkg-config --cflags libhildondesktop-1` `pkg-config --libs libhildondesktop-1` -o $@ $<
	    
example_wavlaunch:
	$(CC) -Wall `pkg-config --cflags $(LIBRARIES) gstreamer-0.10 libpulse-simple` `pkg-config --libs $(LIBRARIES) gstreamer-0.10 libpulse-simple` -DHAVE_PULSEAUDIO -o $@ $@.c

example_camera: example_camera.c
	$(CC) -Wall `pkg-config --cflags $(LIBRARIES) gstreamer-0.10 gstreamer-interfaces-0.10` `pkg-config --libs $(LIBRARIES) gstreamer-0.10 gstreamer-interfaces-0.10` -o $@ $@.c

example_bluetooth:
	$(CC) -Wall `pkg-config --cflags $(LIBRARIES) bluez` `pkg-config --libs $(LIBRARIES) bluez` -o $@ $@.c

# This rule uses GNU make implicit rules, so
# examples can be built eg make example_hildonprogram
# http://www.gnu.org/software/make/manual/make.html#Implicit-Rules
$(EXAMPLES): %: %.c

clean:
	-rm -f $(EXAMPLES) $(EXTRA_EXAMPLES) $(LIBEXAMPLES)
	-rm -f example_alarm_dbus.h

dist: clean
	cd .. && \
	cp -a maemo_examples maemo-examples-$(VERSION) && \
	tar czf maemo-examples-$(VERSION).tar.gz \
		maemo-examples-$(VERSION) && \
	rm -rf maemo-examples-$(VERSION)
