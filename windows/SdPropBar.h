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

    QComboBox *mLayer; //List of available layers

  protected:
    void     setSelectedLayer( SdLayer *layer );
    SdLayer *getSelectedLayer();
  public:
    SdPropBar(const QString title);

  signals:
    //On property changed by user
    void propChanged();

  public slots:
    void updateViewedLayers();
    void onLayerChanged(int index);
  };

#endif // SDPROPBAR_H
