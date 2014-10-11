#!/usr/bin/env bash
# Tool to extract introspect information for connman's Manager interface.
# Requires: dbus-send, tr, sed and xmllint in path 
#
dbus-send --system --print-reply --dest=net.connman /net/connman/service org.freedesktop.DBus.Introspectable.Introspect |\
	tr '\n' ' ' |\
	sed -e 's/^.*"<!DOC\(.*\)"\s*$/<!DOC\1/' |\
	xmllint --format - 
