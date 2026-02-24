/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base tool bar for all property bars
*/
#ifndef SDPROPBAR_H
#define SDPROPBAR_H

#include "objects/SdProjectItem.h"

#include <QToolBar>

class SdPropBar : public QToolBar
  {
    Q_OBJECT

  public:
    SdPropBar(const QString title) : QToolBar( title ) {}

  signals:
    //On property changed by user
    void propChanged();

  public slots:
    virtual void updateEditObjectProp(SdProjectItem *pitem, SdLayer *currentLayer) { Q_UNUSED(pitem); Q_UNUSED(currentLayer); }
  };


#endif // SDPROPBAR_H
