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
#include "objects/SdEnvir.h"
#include <QObject>

SdPItemSheet *SdModeWireDisconnect::getSheet() const
  {
  return dynamic_cast<SdPItemSheet*>(mObject);
  }

SdModeWireDisconnect::SdModeWireDisconnect(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdMode( editor, obj )
  {

  }




void SdModeWireDisconnect::reset()
  {
  setStep( sSiglePin );
  }





void SdModeWireDisconnect::drawDynamic(SdContext *ctx)
  {
  if( getStep() ) {
    //Draw selecting rectangle
    ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltDotted );
    ctx->rect( SdRect(mFirst, mCurPoint) );
    }
  }




void SdModeWireDisconnect::enterPoint(SdPoint enter)
  {
  unconnect( SdRect(enter,enter) );
  }



void SdModeWireDisconnect::cancelPoint(SdPoint)
  {
  cancelMode();
  }




void SdModeWireDisconnect::beginDrag(SdPoint point)
  {
  mFirst = mCurPoint = point;
  setStep( sRectangle );
  }




void SdModeWireDisconnect::dragPoint(SdPoint point)
  {
  if( getStep() ) {
    mCurPoint = point;
    update();
    }
  }




void SdModeWireDisconnect::stopDrag(SdPoint point)
  {
  if( getStep() ) {
    unconnect( SdRect(mFirst,point) );
    setStep( sSiglePin );
    }
  }



QString SdModeWireDisconnect::getStepHelp() const
  {
  return getStep() ? QObject::tr("Drag cursor to over need disconnected pins") : QObject::tr("Enter connected pin to disconnect");
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




void SdModeWireDisconnect::unconnect(SdRect over)
  {
  getSheet()->forEach( dctSymImp, [this, over] (SdObject *obj) -> bool {
    SdPtr<SdGraphSymImp> sym(obj);
    if( sym.isValid() )
      sym->unconnectPinOverRect( over, mUndo, QObject::tr("Unconnect pin") );
    return true;
    });
  setDirty();
  setDirtyCashe();
  update();
  }

