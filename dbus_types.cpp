#include "dbus_types.h"

bool dbus_types_registered = []() -> bool {
    qDBusRegisterMetaType<ObjectProperties>();
    qDBusRegisterMetaType<ObjectPropertiesList>();
    return true;
}();
