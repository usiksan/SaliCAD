/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for symbol pin
*/
#ifndef SDPROPBARSYMPIN_H
#define SDPROPBARSYMPIN_H

#include "SdPropBar.h"
#include "objects/SdPropSymPin.h"

class SdPropBarSymPin : public SdPropBar
  {
    Q_OBJECT

    QComboBox *mPinType;  //Symbol pin type
  public:
    SdPropBarSymPin( const QString title );

    void setPropSymPin(const SdPropSymPin &propSymPin );
    void getPropSymPin( SdPropSymPin &propSymPin );

    void setPropSymPin( const SdPropComposerSymPin &propSymPin );
    void getPropSymPin( SdPropComposerSymPin &propSymPin );

  };

#endif // SDPROPBARSYMPIN_H
