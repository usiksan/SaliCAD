/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for distance measurement
*/
#include "SdModeTRuller.h"
#include "objects/SdEnvir.h"
#include "objects/SdProp.h"
#include "objects/SdPulsar.h"
#include "windows/SdWEditorGraph.h"

#include <math.h>

SdModeTRuller::SdModeTRuller(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeTemp( editor, obj )
  {

  }

void SdModeTRuller::drawDynamic(SdContext *ctx)
  {
  if( getStep() ) {
    ctx->setPen( 0, sdEnvir::instance()->getSysColor(scEnter), dltDotted );
    ctx->line( mFirst, mSecond );
    }
  }




void SdModeTRuller::enterPoint(SdPoint enter)
  {
  mFirst = enter;
  mSecond = enter;
  setStep(sSecond);
  }




void SdModeTRuller::cancelPoint(SdPoint)
  {
  //End mode
  cancelMode();
  }




void SdModeTRuller::movePoint(SdPoint pos)
  {
  if( getStep() && mSecond != pos ) {
    mSecond = pos;
    update();
    double dx = static_cast<double>(pos.x() - mFirst.x()) * mEditor->getPPM();
    double dy = static_cast<double>(pos.y() - mFirst.y()) * mEditor->getPPM();
    double delta = sqrt( dx*dx + dy*dy );
    SdPulsar::sdPulsar->emitSetStatusMessage( QString("dx=%1, dy=%2, distance=%3").arg(dx,0,'f',3).arg(dy,0,'f',3).arg(delta,0,'f',3) );
    }
  }




QString SdModeTRuller::getStepHelp() const
  {
  return getStep() == sSecond ? QObject::tr("Move to dest point") : QObject::tr("Enter point to measure from");
  }




QString SdModeTRuller::getModeThema() const
  {
  return QString( MODE_HELP "ModeTempRuller.htm" );
  }




QString SdModeTRuller::getStepThema() const
  {
  return getStep() == sSecond ? QString( MODE_HELP "ModeTempRuller.htm#second" ) : QString( MODE_HELP "ModeTempRuller.htm#first" );
  }




int SdModeTRuller::getCursor() const
  {
  return CUR_RULLER;
  }




int SdModeTRuller::getIndex() const
  {
  return MD_MEASUREMENT;
  }

