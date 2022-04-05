#ifndef SDJSONIO_H
#define SDJSONIO_H

#include "SvJsonIO.h"

class SdObjectMap;

using SdJsonWriter = SvJsonWriter;

using SdJsonReader = SvJsonReaderExt<SdObjectMap>;

#endif // SDJSONIO_H
