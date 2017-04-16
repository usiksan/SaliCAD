/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for rectangle
*/
#include "SdModeCLinearRect.h"
#include "objects/SdGraphLinearRect.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include <QObject>

SdPoint SdModeCLinearRect::mOffset; //Предположительная очередная точка

SdModeCLinearRect::SdModeCLinearRect(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinear(editor, obj)
  {

  }



void SdModeCLinearRect::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sSecondCorner ) {
    ctx->setPen( sdGlobalProp->mLineProp.mWidth.getValue(), sdEnvir->getSysColor(scEnter),
                 sdGlobalProp->mLineProp.mType.getValue() );
    ctx->rect( SdRect(mFirst,mPrevMove) );
    }
  if( sdEnvir->mIsSmart && mSmartType )
    ctx->smartPoint( mSmartPoint, mSmartType );
  }



void SdModeCLinearRect::enterPoint(SdPoint enter )
  {
  if( getStep() == sSecondCorner ) {
    mPrevMove = enter;
    addRect();
    mOffset = mPrevMove - mFirst;
    setStep( sFirstCorner );
    update();
    }
  else {
    mPrevMove = enter;
    mFirst = mPrevMove;
    setStep( sSecondCorner );
    }
  //Установить разумную точку как относительное смещение
  mSmartPoint = mPrevMove + mOffset;
  mSmartType  = snapPrev;
  }



void SdModeCLinearRect::cancelPoint(SdPoint)
  {
  if( getStep() ) setStep( sFirstCorner );
  else cancelMode();
  }



void SdModeCLinearRect::movePoint(SdPoint p)
  {
  if( getStep() == sFirstCorner ) mFirst = p;
  mPrevMove = p;
  //Вычислить предполагаемую точку вывода
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = sdEnvir->mSmartMask;
  snap.mExclude  = mFirst;
  snap.calculate( mObject );
  mSmartType  = snap.mDestMask;
  mSmartPoint = snap.mDest;
  update();
  }




void SdModeCLinearRect::enterPrev()
  {
  if( sdEnvir->mIsSmart && mSmartType ) enterPoint( mSmartPoint );
  }




QString SdModeCLinearRect::getStepHelp() const
  {
  return getStep() == sSecondCorner ? QObject::tr("Enter second corner rect") : QObject::tr("Enter first corner rect");
  }




QString SdModeCLinearRect::getModeHelp() const
  {
  return QString( MODE_HELP "ModeCLinearRect.htm" );
  }




QString SdModeCLinearRect::getStepThema() const
  {
  return getStep() == sSecondCorner ? QString( MODE_HELP "ModeCLinearRect.htm#secondCorner" ) : QString( MODE_HELP "ModeCLinearRect.htm#first" );
  }




int SdModeCLinearRect::getCursor() const
  {
  return CUR_RECT;
  }




int SdModeCLinearRect::getIndex() const
  {
  return MD_RECT;
  }




//Insert rectangle into graph object
void SdModeCLinearRect::addRect()
  {
  addPic( new SdGraphLinearRect( mFirst, mPrevMove, sdGlobalProp->mLineProp ), QObject::tr("Insert rect") );
  }
