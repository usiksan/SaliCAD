/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base tool bar for property bars
*/
#ifndef SDPROPBAR_H
#define SDPROPBAR_H

#include "objects/SdProp.h"
#include <QToolBar>
#include <QComboBox>

class SdPropBar : public QToolBar
  {
    Q_OBJECT

  protected:
    QComboBox *mLayer; //List of available layers

    void     setSelectedLayer( SdLayer *layer );
    SdLayer *getSelectedLayer();
  public:
    SdPropBar(const QString title);

  signals:
    //On property changed by user
    void propChanged();

  public slots:
    virtual void updateViewedLayers();
  };

#endif // SDPROPBAR_H
