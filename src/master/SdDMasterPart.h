/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base class for part masters.

  Contains operations for append graphics and pins to part when creation.
*/
#ifndef SDDMASTERPART_H
#define SDDMASTERPART_H

#include "objects/SdProjectItem.h"
#include "SdMasterPart.h"
#include <QDialog>

class SdDMasterPart : public QDialog
  {
    Q_OBJECT

  protected:
    SdMasterPart mMasterPart;

    //Add common graphics to part
    void addLine( int x1, int y1, int x2, int y2 ) { mMasterPart.addLine( x1, y1, x2, y2 ); }
    void addRect( int x1, int y1, int x2, int y2 ) { mMasterPart.addRect( x1, y1, x2, y2 ); }
    void addCircle( int cx, int cy, int r ) { mMasterPart.addCircle( cx, cy, r ); }

    //Identifier append to "id" layer
    void setId(SdPoint p, int size = 1000 ) { mMasterPart.setId( p, size ); }

    //Value placeholder append to "value" layer
    void setValue(SdPoint p, int size = 1000 ) { mMasterPart.setValue( p, size ); }

    //Pin append to "pin" layer
    void setupSmdPin() { mMasterPart.setupSmdPin(); }
    void setupThrouPin() { mMasterPart.setupThrouPin(); }
    void addPin( SdPoint org, const QString type, SdPoint pinNumberOrg, const QString pinNumber, SdPoint pinNameOrg ) {
      mMasterPart.addPin( org, type, pinNumberOrg, pinNumber, pinNameOrg );
      }
  public:
    SdDMasterPart( SdProjectItem *item, QWidget *parent );

  protected:
  };

#endif // SDDMASTERPART_H
