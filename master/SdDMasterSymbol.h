/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base class for symbol masters.

  Contains operations for append graphics and pins to symbol when creation.
*/
#ifndef SDDMASTERSYMBOL_H
#define SDDMASTERSYMBOL_H

#include "objects/SdProjectItem.h"

#include <QDialog>


class SdDMasterSymbol : public QDialog
  {
    Q_OBJECT

  protected:
    SdProjectItem *mItem;
    SdPropLine     mLineProp;       //All graph objects append by default to "component" layer with 0-width
    SdPropSymPin   mPinProp;        //All pins
    SdPropText     mIdentProp;
    SdPropText     mPinNumberProp;
    SdPropText     mPinNameProp;


    //Add common graphics to part
    void addLine( int x1, int y1, int x2, int y2 );
    void addRect( int x1, int y1, int x2, int y2 );
    void addCircle( int cx, int cy, int r );

    //Identifier append to "id" layer
    void setId(SdPoint p, int size = 350 );

    //Pin append to "pin" layer
    void addPin( SdPoint org, int type, SdPoint pinNameOrg, const QString pinName, SdPoint pinNumberOrg );
  public:
    SdDMasterSymbol( SdProjectItem *item, QWidget *parent );

  };

#endif // SDDMASTERSYMBOL_H
