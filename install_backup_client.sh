#!/bin/sh
set -x
cp -f example_backup_client example_backup_server /usr/bin/
cp -f example_backup_client.desktop /usr/share/applications/hildon/example_backup_client.desktop
cp -f example_backup_client.service /usr/share/dbus-1/services/
( cd /etc/others-menu/ ; ln -fs /usr/share/applications/hildon/example_backup_client.desktop /etc/others-menu/0010_example_backup_client.desktop )

