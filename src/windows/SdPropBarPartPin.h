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

    QComboBox   *mPinSide;          //Part pin side (top, bottom or through)
    QComboBox   *mPinType;          //Part pin type
  public:
    SdPropBarPartPin( const QString title );

    void setPropPartPin( const SdPropPartPin &propPartPin );
    void getPropPartPin( SdPropPartPin &propPartPin );

    void setPropPartPin( const SdPropComposerPartPin &propPartPin );
    void getPropPartPin( SdPropComposerPartPin &propPartPin );

  private slots:
    void setPinType();
  };

#endif // SDPROPBARPARTPIN_H
