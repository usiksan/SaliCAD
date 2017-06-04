/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing layers order and contens
*/
#ifndef SDSTRATUM_H
#define SDSTRATUM_H

#include "SdConfig.h"
#include <QJsonObject>



class SdStratum
  {
    quint64 mStratum;
  public:
    SdStratum();
  };

#endif // SDSTRATUM_H
