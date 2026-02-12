/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  With this mode user fix print window
*/
#include "SdModePrintWindow.h"
#include "objects/SdEnvir.h"
#include "windows/SdWEditorGraph.h"

SdModePrintWindow::SdModePrintWindow(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeTemp( editor, obj )
  {

  }




void SdModePrintWindow::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sSecondCorner ) {
    ctx->setPen( 0, SdEnvir::instance()->getSysColor(scEnter), dltDotted );
    ctx->rect( SdRect(mFirst, mPrevMove) );
    }
  }




void SdModePrintWindow::enterPoint(SdPoint enter )
  {
  if( getStep() == sSecondCorner ) {
    SdRect r( mFirst, enter );
    SdWEditorGraph *edit = mEditor;
    update();
    //After this call object is destroied
    cancelMode();
    edit->printDialog( r );
    }
  else {
    mPrevMove = enter;
    mFirst = mPrevMove;
    setStep( sSecondCorner );
    }
  }




void SdModePrintWindow::cancelPoint(SdPoint)
  {
  if( getStep() ) setStep( sFirstCorner );
  else cancelMode();
  }




void SdModePrintWindow::movePoint(SdPoint p)
  {
  if( getStep() == sFirstCorner ) mFirst = p;
  mPrevMove = p;
  update();
  }




QString SdModePrintWindow::getStepHelp() const
  {
  return getStep() == sSecondCorner ? QObject::tr("Enter second corner print window") : QObject::tr("Enter first corner print window");
  }




QString SdModePrintWindow::getModeThema() const
  {
  return QString( MODE_HELP "ModePrintWindow.htm" );
  }



QString SdModePrintWindow::getStepThema() const
  {
  return getStep() == sSecondCorner ? QString( MODE_HELP "ModePrintWindow.htm#secondCorner" ) : QString( MODE_HELP "ModePrintWindow.htm#first" );
  }



int SdModePrintWindow::getCursor() const
  {
  return CUR_ARROW;
  }



int SdModePrintWindow::getIndex() const
  {
  return MD_PRINT_WINDOW;
  }
