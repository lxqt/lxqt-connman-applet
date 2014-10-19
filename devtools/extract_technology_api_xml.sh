#!/usr/bin/env bash
# Tool to extract introspect information for connman's Service interface.
# Requires: dbus-send, tr, sed and xmllint in path 
# Also, to introspect the services interface, a service is needed - so you need to have a wifi-radio on,
# an ethernet connection or a bluetooth-connection active. Probably.
#
TECHNOLOGY=`dbus-send --system --print-reply --dest=net.connman /net/connman/technology org.freedesktop.DBus.Introspectable.Introspect |\
			grep "node name=" |\
			head -1 |\
			sed -e 's/.*node name="\(.*\)".*$/\1/'`

if [ -z "$TECHNOLOGY" ]; then
	echo "No technologies found" >&2
	exit 1
fi

dbus-send --system --print-reply --dest=net.connman /net/connman/technology/$TECHNOLOGY org.freedesktop.DBus.Introspectable.Introspect |\
	tr '\n' ' ' |\
	sed -e 's/^.*"<!DOC\(.*\)"\s*$/<!DOC\1/' |\
	xmllint --format - 
