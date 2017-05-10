/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for part pin
*/
#ifndef SDPROPBARPARTPIN_H
#define SDPROPBARPARTPIN_H

#include "SdPropBar.h"
#include "objects/SdPropPartPin.h"

class SdPropBarPartPin : public SdPropBar
  {
    Q_OBJECT

    QComboBox *mPinType;  //Part pin type
  public:
    SdPropBarPartPin( const QString title );

    void setPropPartPin( SdPropPartPin *propPartPin );
    void getPropPartPin( SdPropPartPin *propPartPin );
  };

#endif // SDPROPBARPARTPIN_H
