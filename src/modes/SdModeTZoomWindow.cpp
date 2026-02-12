/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "objects/SdEnvir.h"
#include "objects/SdProp.h"
#include "SdModeTZoomWindow.h"
#include "windows/SdWEditorGraph.h"

#include <QObject>

SdModeTZoomWindow::SdModeTZoomWindow( SdWEditorGraph *editor, SdProjectItem *obj ) :
  SdModeTemp( editor, obj )
  {

  }




void SdModeTZoomWindow::drawDynamic(SdContext *ctx)
  {
  if( getStep() ) {
    ctx->setPen( 0, sdEnvir::instance()->getSysColor(scEnter), dltDotted );
    ctx->rect( SdRect(mFirst,mSecond) );
    }
  }




void SdModeTZoomWindow::enterPoint(SdPoint enter)
  {
  if( getStep() ) {
    //Прямоугольник отмечен
    SdPoint p;
    p = enter - mFirst;
    if( p.x() && p.y() ) {
      //Образовать прямоугольник
      mEditor->zoomWindow( SdRect(mFirst,enter) );
      }
    //End mode
    cancelMode();
    }
  else {
    mFirst = enter;
    mSecond = enter;
    setStep(sSecond);
    }
  }




void SdModeTZoomWindow::cancelPoint(SdPoint)
  {
  if( getStep() ) {
    setStep(sFirst);
    update();
    }
  else cancelMode();
  }




void SdModeTZoomWindow::movePoint(SdPoint p)
  {
  if( getStep() && mSecond != p) {
    mSecond = p;
    update();
    }
  }




QString SdModeTZoomWindow::getStepHelp() const
  {
  return getStep() == sSecond ? QObject::tr("Enter second corner window") : QObject::tr("Enter first corner window");
  }




QString SdModeTZoomWindow::getModeThema() const
  {
  return QString( MODE_HELP "ModeZoomWin.htm" );
  }




QString SdModeTZoomWindow::getStepThema() const
  {
  return getStep() == sSecond ? QString( MODE_HELP "ModeZoomWin.htm#second" ) : QString( MODE_HELP "ModeZoomWin.htm#first" );
  }




int SdModeTZoomWindow::getCursor() const
  {
  return CUR_ZOOM_WIN;
  }




int SdModeTZoomWindow::getIndex() const
  {
  return MD_ZOOM_WIN;
  }
