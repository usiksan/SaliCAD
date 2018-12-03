/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Browse part in schematic sheet to place in plate
*/
#include "SdModeTBrowseSheetPart.h"
#include "objects/SdPulsar.h"
#include "windows/SdWEditorGraph.h"

SdModeTBrowseSheetPart::SdModeTBrowseSheetPart(SdWEditorGraph *editor, SdProjectItem *obj, SdProjectItem *plate ) :
  SdModeTemp( editor, obj ),
  mSymImp(nullptr),
  mPlate(plate)
  {

  }





QString SdModeTBrowseSheetPart::getStepHelp() const
  {
  if( mSymImp ) {
    int logNumber, logSection;
    QString prefix = mSymImp->identInfoGet( logNumber, logSection );
    return QObject::tr("Select part to place. On left button will selected: %1%2").arg(prefix).arg(logNumber);
    }
  return QObject::tr("Select part to place.");
  }




QString SdModeTBrowseSheetPart::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeTBrowseSheetPart.htm" );
  }




QString SdModeTBrowseSheetPart::getStepThema() const
  {
  return getModeThema();
  }




int SdModeTBrowseSheetPart::getCursor() const
  {
  return mSymImp == nullptr ? CUR_ARROW : CUR_TAKE;
  }





int SdModeTBrowseSheetPart::getIndex() const
  {
  return MD_BROWSE_PART;
  }





void SdModeTBrowseSheetPart::movePoint(SdPoint p)
  {
  int prevCursor = getCursor();

  //Find symbol implement behind cursor
  mSymImp = nullptr;
  mObject->forEach( dctSymImp, [this,p] (SdObject *obj) -> bool {
    SdPtr<SdGraphSymImp> sym(obj);
    if( sym && sym->behindCursor(p) ) {
      mSymImp = sym.ptr();
      return false;
      }
    return true;
    });

  //Update cursor if needed
  if( prevCursor != getCursor() )
    mEditor->viewport()->setCursor( loadCursor(getCursor()) );
  }





void SdModeTBrowseSheetPart::enterPoint(SdPoint p)
  {
  movePoint(p);
  if( mSymImp ) {
    //Prepare component list
    QStringList list;
    int logNumber, logSection;
    QString prefix = mSymImp->identInfoGet( logNumber, logSection );
    list.append( prefix + QString::number(logNumber) );
    SdProjectItem *plate = mPlate;
    //Stop browse mode
    cancelMode();
    //Signal to switch to plate
    SdPulsar::sdPulsar->emitSelectedParts( plate, list );
    }
  }





void SdModeTBrowseSheetPart::cancelPoint(SdPoint)
  {
  //After cancel mode object deleted. Because this undefined
  SdProjectItem *plate = mPlate;
  //Stop browse mode
  cancelMode();
  //Signal to switch to plate
  SdPulsar::sdPulsar->emitSelectedParts( plate, QStringList() );
  }
