/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for symbol pin
*/
#include "SdModeCSymPin.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include "objects/SdGraphSymPin.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdPropBarSymPin.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>

SdPoint
  SdModeCSymPin::mSmartNumber,       //Offset from previous number
  SdModeCSymPin::mSmartName;         //Offset from previous name
QString SdModeCSymPin::mPrevName;    //Previous name

#define sPlacePin    0
#define sPlaceName   1
#define sEnterName   2
#define sPlaceNumber 3
#define sLast        4

SdModeCSymPin::SdModeCSymPin(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCTextual( editor, obj )
  {

  }





void SdModeCSymPin::drawDynamic(SdContext *ctx)
  {
  SdRect r;
  switch( getStep() ) {
    case sPlaceNumber :
      //Rectangle for number
      ctx->text( mNumberPos, r, QString("  "), sdGlobalProp->mPinNumberProp );
      ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltDashed );
      ctx->rect( r );
      //Pin name
      ctx->setFont( sdGlobalProp->mPinNameProp );
      ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltSolid );
      ctx->textEx( mNamePos, r, mName, sdGlobalProp->mPinNameProp.mDir, sdGlobalProp->mPinNameProp.mHorz, sdGlobalProp->mPinNameProp.mVert );

    case sEnterName :
      drawText( ctx );

    case sPlaceName :
      ctx->symPin( mOrigin, sdEnvir->getSysColor(scEnter) );
    }
  }




int SdModeCSymPin::getPropBarId() const
  {
  switch( getStep() ) {
    case sPlaceNumber :
    case sPlaceName   :
    case sEnterName   :
      return PB_TEXT;
    }
  return PB_SYM_PIN;
  }



void SdModeCSymPin::propGetFromBar()
  {
  if( getStep() == sPlaceNumber ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) {
      tbar->getPropText( &(sdGlobalProp->mPinNumberProp) );
      mEditor->setFocus();
      update();
      }
    }
  else if( getStep() == sEnterName ) SdModeCTextual::propGetFromBar();
  else if( getStep() == sPlaceName ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) {
      tbar->getPropText( &(sdGlobalProp->mPinNameProp) );
      mEditor->setFocus();
      update();
      }
    }
  else if( getStep() == sPlacePin ) {
    SdPropBarSymPin *sbar = dynamic_cast<SdPropBarSymPin*>( SdWCommand::getModeBar(PB_SYM_PIN) );
    if( sbar ) {
      sbar->getPropSymPin( &(sdGlobalProp->mSymPinProp) );
      mEditor->setFocus();
      update();
      }
    }
  }




void SdModeCSymPin::propSetToBar()
  {
  if( getStep() == sPlaceNumber ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) tbar->setPropText( &(sdGlobalProp->mPinNumberProp), mEditor->getPPM() );
    }
  else if( getStep() == sEnterName ) SdModeCTextual::propSetToBar();
  else if( getStep() == sPlaceName ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) tbar->setPropText( &(sdGlobalProp->mPinNameProp), mEditor->getPPM() );
    }
  else if( getStep() == sPlacePin ) {
    SdPropBarSymPin *sbar = dynamic_cast<SdPropBarSymPin*>( SdWCommand::getModeBar(PB_SYM_PIN) );
    if( sbar ) sbar->setPropSymPin( &(sdGlobalProp->mSymPinProp) );
    }
  }




void SdModeCSymPin::enterPoint( SdPoint enter )
  {
  switch( getStep() ) {
    case sPlacePin :
      //Enter pin place
      mOrigin = enter;
      //Switch to pin name place
      mSmartPoint = mOrigin + mSmartName;
      mSmartType  = snapPrev;
      setStep( sPlaceName );
      break;
    case sPlaceName :
      mName = nextText( mPrevName );
      mNamePos = enter;
      //Enter pin name place is completed
      //Text editor on
      mPropText = &(sdGlobalProp->mPinNameProp);
      setText( mName, true );
      setStep( sEnterName );
      break;
    case sEnterName :
      applyEdit();
      break;
    case sPlaceNumber :
      //Enter pin number place is completed
      mNumberPos = enter;
      addPic( new SdGraphSymPin( mOrigin, mNumberPos, mNamePos, mName ), QObject::tr("Enter sym pin") );
      //Calculate smart-params
      mSmartNumber = mNumberPos - mOrigin;
      mSmartName   = mNamePos   - mOrigin;
      mPrevName    = mName;
      setStep( sPlacePin );
      mSmartType = 0;
      break;
    }
  }




void SdModeCSymPin::cancelPoint(SdPoint)
  {
  cancelMode();
  }




void SdModeCSymPin::movePoint( SdPoint p )
  {
  }

void SdModeCSymPin::enterPrev()
  {
  }

QString SdModeCSymPin::getStepHelp() const
  {
  }

QString SdModeCSymPin::getModeHelp() const
  {
  }

QString SdModeCSymPin::getStepThema() const
  {
  }

int SdModeCSymPin::getCursor() const
  {
  }

int SdModeCSymPin::getIndex() const
  {
  }



void SdModeCSymPin::cancelEdit()
  {
  mPropText = 0;
  setStep( sPlaceName );
  }



//Name editing is finished
void SdModeCSymPin::applyEdit()
  {
  mPropText = 0;
  mName = mString;
  setStep( sPlaceNumber );
  }
