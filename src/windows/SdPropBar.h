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

#include "objects/SdLayer.h"
#include "objects/SdProjectItem.h"

#include <QToolBar>
#include <QComboBox>

class SdPropBar : public QToolBar
  {
    Q_OBJECT

    SdClass   mEditObjectClass; //!< Class of edit object (one of derived from SdProjectItem)
    SdStratum mStratum;         //!< Plate stratum otherwise stmThrough
  protected:
    QComboBox    *mLayer;     //!< Visual list of available layers

    void     setSelectedLayer( SdLayer *layer );
    SdLayer *getSelectedLayer();
  public:
    SdPropBar(const QString title);

  signals:
    //On property changed by user
    void propChanged();

  public slots:
    virtual void updateViewedLayers( SdLayer *currentLayer );

    void         updateEditObjectProp(SdProjectItem *pitem, SdLayer *currentLayer);

    void         updateEditObjectProp(SdClass theClass, SdStratum stratum , SdLayer *currentLayer);

  private:
    void         refillLayers();
  };

#endif // SDPROPBAR_H
