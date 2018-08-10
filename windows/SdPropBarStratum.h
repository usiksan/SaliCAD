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

#include "SdPropBar.h"

class SdPItemPlate;

class SdPropBarStratum : public SdPropBar
  {
    Q_OBJECT

    SdPItemPlate *mPlate;      //Plate contains stratum count
    SdLayerTrace  mLayerTrace;
  protected:
    void      setSelectedStratum( SdStratum stratum );
    SdStratum getSelectedStratum();
  public:
    SdPropBarStratum(const QString title);

    void setPlateAndTrace( SdPItemPlate *plate, SdLayerTrace trace );
  public slots:
    virtual void updateViewedLayers();
  };

#endif // SDPROPBARSTRATUM_H
