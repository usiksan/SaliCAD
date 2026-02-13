/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDPROPBARSTRATUM_H
#define SDPROPBARSTRATUM_H

#include "objects/SdStratum.h"
#include "SdPropBar.h"

class SdPItemPlate;

class SdPropBarStratum : public SdPropBar
  {
    Q_OBJECT

  protected:
    SdPItemPlate *mPlate;      //Plate contains stratum count
    SdLayerTrace  mLayerTrace;

    void      setSelectedStratum( SdStratum stratum );
    SdStratum getSelectedStratum();
  public:
    SdPropBarStratum(const QString title);

    void setPlateAndTrace( SdPItemPlate *plate, SdLayerTrace trace );
  public slots:
    virtual void updateViewedLayers(SdLayer*selectedLayer) override;
  };

#endif // SDPROPBARSTRATUM_H
