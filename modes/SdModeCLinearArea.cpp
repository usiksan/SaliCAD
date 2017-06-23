/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter plate area in schematic sheet
*/
#include "SdModeCLinearArea.h"
#include "objects/SdGraphArea.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include "objects/SdPItemPlate.h"
#include "windows/SdDGetProjectObject.h"
#include <QObject>

SdModeCLinearArea::SdModeCLinearArea(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinear( editor, obj )
  {

  }

void SdModeCLinearArea::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sNextPoint ) {
    ctx->setPen( sdGlobalProp->mLineProp.mWidth.getValue(), sdEnvir->getSysColor(scEnter),
                 sdGlobalProp->mLineProp.mType.getValue() );
    ctx->region( mList, false );
    ctx->line( mList.last(), mMiddle );
    if( mMiddle != mPrevMove )
      ctx->line( mMiddle, mPrevMove );
    }
  }

void SdModeCLinearArea::enterPoint(SdPoint enter)
  {
  if( getStep() == sNextPoint ) {
    SdPoint first;
    first = mList.last();
    mPrevMove = calcMiddlePoint( first, enter, sdGlobalProp->mLineEnterType );
    if( mPrevMove == first ) mPrevMove = enter;
    //If point is close region then append region
    if( mPrevMove == mList.first() && mList.count() > 2 ) {
      addArea();
      }
    else {
      //If enter point equals last point then remove last point
      if( first == mPrevMove ) {
        mList.removeLast();
        if( mList.count() == 0 )
          setStep( sNextPoint );
        }
      else {
        mList.append( mPrevMove );
        mPrevMove = enter;
        }
      }
    }
  else {
    mList.clear();
    mList.append( enter );
    mPrevMove = enter;
    setStep( sNextPoint );
    }
  update();
  }

void SdModeCLinearArea::cancelPoint(SdPoint)
  {
  if( getStep() ) { setStep( sFirstPoint ); update(); }
  else cancelMode();
  }

void SdModeCLinearArea::movePoint(SdPoint p)
  {
  mPrevMove = p;
  if( getStep() == sNextPoint )
    mMiddle = calcMiddlePoint( mList.last(), mPrevMove, sdGlobalProp->mLineEnterType );
  }




void SdModeCLinearArea::enterPrev()
  {
  if( getStep() == sNextPoint && mList.count() > 2 ) {
    addArea();
    }
  }




QString SdModeCLinearArea::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of area region") : QObject::tr("Enter first point of area region");
  }




QString SdModeCLinearArea::getModeThema() const
  {
  return QString( MODE_HELP "ModeCLinearArea.htm" );
  }




QString SdModeCLinearArea::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCLinearArea.htm#nextPoint" ) : QString( MODE_HELP "ModeCLinearArea.htm#firstPoint" );
  }




int SdModeCLinearArea::getCursor() const
  {
  return CUR_AREA;
  }




int SdModeCLinearArea::getIndex() const
  {
  return MD_PCB_AREA;
  }



//Add area region to schematic sheet. Before adding we select plate for area
void SdModeCLinearArea::addArea()
  {
  //Open dialog with plate selection
  SdDGetProjectObject dlg( mObject->getRoot(), dctPlate, QObject::tr("Select plate this area for"), mEditor );
  if( dlg.exec() && dlg.selectedItem() != nullptr ) {
    addPic( new SdGraphArea( mList, sdGlobalProp->mLineProp, dynamic_cast<SdPItemPlate*>(dlg.selectedItem())), QObject::tr("Insert area") );
    setStep( sFirstPoint );
    }
  }
