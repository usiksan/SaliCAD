#ifndef VRMLTYPES_H
#define VRMLTYPES_H

#include <QString>
#include <QList>

using VrmlFloat  = float;
using VrmlInt32  = int;
using VrmlBool   = bool;
using VrmlString = QString;

using VrmlInt32List = QList<VrmlInt32>;

//Forward scaner declaration
class SdScanerVrml;

#endif // VRMLTYPES_H
