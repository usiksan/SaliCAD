/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDMASTERPART_H
#define SDDMASTERPART_H

#include "objects/SdProjectItem.h"
#include <QDialog>

class SdDMasterPart : public QDialog
  {
    Q_OBJECT

  protected:
    SdProjectItem *mItem;
    SdPropLine     mLineProp;       //All graph objects append by default to "component" layer with 0-width
    SdPropPartPin  mPinProp;        //All pins
    SdPropText     mIdentProp;
    SdPropText     mPinNumberProp;
    SdPropText     mPinNameProp;


    void addLine( int x1, int y1, int x2, int y2 );
    void addRect( int x1, int y1, int x2, int y2 );
    void addCircle( int cx, int cy, int r );

    //Identifier append to "id" layer
    void setId( SdPoint p, const QString id, int size = 1000 );

    //Pin append to "pin" layer
    void setupSmdPin();
    void setupThrouPin();
    void addPin( SdPoint org, const QString type, SdPoint pinNumberOrg, const QString pinNumber, SdPoint pinNameOrg );
  public:
    SdDMasterPart( SdProjectItem *item, QWidget *parent );

  protected:
  };

#endif // SDDMASTERPART_H
