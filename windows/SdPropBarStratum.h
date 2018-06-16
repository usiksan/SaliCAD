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

class SdPropBarStratum : public SdPropBar
  {
    Q_OBJECT

    int          mStratumCount;
    SdLayerTrace mLayerTrace;
  protected:
    void      setSelectedStratum( SdStratum stratum );
    SdStratum getSelectedStratum();
  public:
    SdPropBarStratum(const QString title);

    void setStratumCountAndTrace( int stratumCount, SdLayerTrace trace );
  public slots:
    virtual void updateViewedLayers();
  };

#endif // SDPROPBARSTRATUM_H
