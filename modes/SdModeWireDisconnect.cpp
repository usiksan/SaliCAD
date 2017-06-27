/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeWireDisconnect.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdGraphSymImp.h"
#include <QObject>

SdPItemSheet *SdModeWireDisconnect::getSheet() const
  {
  return dynamic_cast<SdPItemSheet*>(mObject);
  }

SdModeWireDisconnect::SdModeWireDisconnect(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdMode( editor, obj )
  {

  }


void SdModeWireDisconnect::enterPoint(SdPoint enter)
  {
  getSheet()->forEach( dctSymImp, [this, enter] (SdObject *obj) -> bool {
    SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>(obj);
    Q_ASSERT( sym != nullptr );
    sym->unconnectPinInPoint( enter, mUndo, QObject::tr("Unconnect pin") );
    return true;
    });
  setDirty();
  setDirtyCashe();
  update();
  }



void SdModeWireDisconnect::cancelPoint(SdPoint)
  {
  cancelMode();
  }



QString SdModeWireDisconnect::getStepHelp() const
  {
  return QObject::tr("Enter connected pin to disconnect");
  }



QString SdModeWireDisconnect::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeWireDisconnect.htm" );
  }



QString SdModeWireDisconnect::getStepThema() const
  {
  return QStringLiteral( MODE_HELP "ModeWireDisconnect.htm" );
  }



int SdModeWireDisconnect::getCursor() const
  {
  return CUR_UCOM;
  }



int SdModeWireDisconnect::getIndex() const
  {
  return MD_DISCONNECT;
  }

