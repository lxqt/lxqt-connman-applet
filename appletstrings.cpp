/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * lxqt-connman-applet - a gui frontend for connman
 *
 * Copyright: 2014-2015 Christian Surlykke
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "appletstrings.h"

#include <QMap>
#include <QObject>

QMap<QString, QString> buildMap() {
	// Map from connman dbus strings to presentation-strings. 
	// The connman strings are property names, map keys and such.
	// This stuff is extracted from connman's dbus api (and surely incomplete..)
	QMap<QString, QString> map;
	
	map["name"] = QObject::tr("name", "network name");
	map["Identity"] = QObject::tr("Username");
	map["Username"] = QObject::tr("Username");
	map["Passphrase"] = QObject::tr("Passphrase");
	map["PreviousPassphrase"] = QObject::tr("Previous passphrase");
	
	map["Wired"] = QObject::tr("Wired");
	map["WiFi"]	= QObject::tr("Wireless");
	map["P2P"] = QObject::tr("Peer-to-peer");
	map["Bluetooth"] = QObject::tr("Bluetooth");

    map["out-of-range"] = QObject::tr("Out of range");
    map["pin-missing"] = QObject::tr("Pin code missing");
    map["dhcp-failed"] = QObject::tr("DHCP failed");
    map["connect-failed"] = QObject::tr("Connection failed");
    map["login-failed"] = QObject::tr("Login failed");
    map["auth-failed"] = QObject::tr("Authentication failed");
    map["invalid-key"] = QObject::tr("Invalid key");

    map["configuration"] = QObject::tr("Configuration");
    map["association"] = QObject::tr("Association");

	return map;
}

QString string(QString string) {
	static QMap<QString, QString> translationMap = buildMap();
	
	// If we don't know anything about the string we just return it as it is.
	return translationMap.value(string, string); 
}
