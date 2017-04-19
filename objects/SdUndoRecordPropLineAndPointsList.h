/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for line properties and point list.
*/
#ifndef SDUNDORECORDPROPLINEANDPOINTSLIST_H
#define SDUNDORECORDPROPLINEANDPOINTSLIST_H


#include "SdUndoRecord.h"
#include "SdPropLine.h"
#include "SdPoint.h"
#include "SdPointList.h"

class SdUndoRecordPropLineAndPointsList : public SdUndoRecord
  {
    SdPropLine      *mProp;
    SdPropLineState  mPropState;
    SdPointList     *mSrc;
    SdPointList      mPointList;
  public:
    SdUndoRecordPropLineAndPointsList( SdPropLine *prp, SdPointList *src );

    //Undo elementary operation.
    virtual void    undo() override;
  };

#endif // SDUNDORECORDPROPLINEANDPOINTSLIST_H
