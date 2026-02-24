/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Default tool bar for selection
*/
#ifndef SDPROPBARDEFAULT_H
#define SDPROPBARDEFAULT_H

#include "SdPropBarLay.h"

class SdPropBarDefault : public SdPropBarLay
  {
    Q_OBJECT

    QAction *mMaskComp;    //Do not tought components
    QAction *mMaskNet;     //Do not tought nets
    QAction *mMaskPicture; //Do not tought picture elements
  public:
    SdPropBarDefault(const QString title);
  };

#endif // SDPROPBARDEFAULT_H
