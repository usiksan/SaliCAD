/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for polygon.
*/
#ifndef SDUNDORECORDPOLYGON_H
#define SDUNDORECORDPOLYGON_H

#include "SdUndoRecord.h"
#include "SdPropPolygon.h"
#include "SdPointList.h"
#include "SdPolyWindowList.h"


class SdUndoRecordPolygon : public SdUndoRecord
  {
    SdPropPolygonState mPropState;     //Polygon properties
                                       // - gap for windows creation
                                       // - net name which contains this polygon
                                       // - stratum of polygon
    SdPropPolygon     *mPropSource;
    SdPointList        mRegion;        //Contour of polygon
    SdPointList       *mRegionSource;
    SdPolyWindowList   mWindows;       //Windows of polygon
    SdPolyWindowList  *mWindowsSource;

  public:
    SdUndoRecordPolygon( SdPropPolygon *propSource, SdPointList *regionSource, SdPolyWindowList *windowsSource );

    //Undo elementary operation.
    virtual void    undo() override;
  };

#endif // SDUNDORECORDPOLYGON_H
