#include "version.h"
#ifndef CLIENTVERSION_H
#define CLIENTVERSION_H

//
// client versioning
//

// These need to be macros, as version.cpp's and bitcoin-qt.rc's voodoo requires it
#define CLIENT_VERSION_MAJOR       2
#define CLIENT_VERSION_MINOR       1
#define CLIENT_VERSION_REVISION    0
#define CLIENT_VERSION_BUILD       1

//#define CLIENT_VERSION_MAJOR       DISPLAY_VERSION_MAJOR
//#define CLIENT_VERSION_MINOR       DISPLAY_VERSION_MINOR
//#define CLIENT_VERSION_REVISION    DISPLAY_VERSION_REVISION
//#define CLIENT_VERSION_BUILD       DISPLAY_VERSION_BUILD

// Converts the parameter X to a string after macro replacement on X has been performed.
// Don't merge these into one macro!
#define STRINGIZE(X) DO_STRINGIZE(X)
#define DO_STRINGIZE(X) #X

#endif // CLIENTVERSION_H
