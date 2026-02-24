/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base tool bar for property bars with layers
*/
#ifndef SDPROPBARLAY_H
#define SDPROPBARLAY_H

#include "objects/SdLayer.h"
#include "objects/SdProjectItem.h"
#include "SdPropBar.h"

#include <QToolBar>
#include <QComboBox>

class SdPropBarLay : public SdPropBar
  {
    Q_OBJECT

    SdClass     mEditObjectClass; //!< Class of edit object (one of derived from SdProjectItem)
    SdPvStratum mStratum;         //!< Plate stratum otherwise stmThrough
  protected:
    QComboBox    *mLayer;         //!< Visual list of available layers

    void     setSelectedLayer( SdLayer *layer );
    SdLayer *getSelectedLayer();
  public:
    SdPropBarLay(const QString title);

  signals:
    //On property changed by user
    void propChanged();

  public slots:
    virtual void updateViewedLayers( SdLayer *currentLayer );

    virtual void updateEditObjectProp(SdProjectItem *pitem, SdLayer *currentLayer) override;

    void         updateEditObjectProp(SdClass theClass, SdPvStratum stratum , SdLayer *currentLayer);

  private:
    void         refillLayers();
  };

#endif // SDPROPBARLAY_H
