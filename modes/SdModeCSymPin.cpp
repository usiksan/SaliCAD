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
#include <QMessageBox>

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
  SdModeCTextual( editor, obj ),
  mSmartType(0)
  {

  }







void SdModeCSymPin::drawDynamic(SdContext *ctx)
  {
  SdRect r;
  switch( getStep() ) {
    case sPlaceNumber :
      //Rectangle for number
      ctx->text( mNumberPos, r, QString("XX"), sdGlobalProp->mSymPinNumberProp );
      ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltDashed );
      ctx->rect( r );
      //Pin name
      ctx->setFont( sdGlobalProp->mSymPinNameProp );
      ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltSolid );
      ctx->textEx( mNamePos, r, mName, sdGlobalProp->mSymPinNameProp.mDir, sdGlobalProp->mSymPinNameProp.mHorz, sdGlobalProp->mSymPinNameProp.mVert );

    case sEnterName :
      drawText( ctx );

    case sPlaceName :
      ctx->cross( mOrigin, sdEnvir->mSymPinSize, sdEnvir->getSysColor(scEnter) );
    }

  //Draw smart point
  if( sdEnvir->mIsSmart ) {
    if( (mSmartType & snapEndPoint) && (getStep() == sPlacePin) )
      ctx->smartPoint( mSmartPoint, snapEndPoint );
    if( (mSmartType & snapPrev) && (getStep() == sPlaceName || getStep() == sPlaceNumber) )
      ctx->smartPoint( mSmartPoint, snapPrev );
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
      tbar->getPropText( &(sdGlobalProp->mSymPinNumberProp) );
      mEditor->setFocus();
      update();
      }
    }
  else if( getStep() == sEnterName ) SdModeCTextual::propGetFromBar();
  else if( getStep() == sPlaceName ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) {
      tbar->getPropText( &(sdGlobalProp->mSymPinNameProp) );
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
    if( tbar ) tbar->setPropText( &(sdGlobalProp->mSymPinNumberProp), mEditor->getPPM() );
    }
  else if( getStep() == sEnterName ) SdModeCTextual::propSetToBar();
  else if( getStep() == sPlaceName ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) tbar->setPropText( &(sdGlobalProp->mSymPinNameProp), mEditor->getPPM() );
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
      setStep( sPlaceName );
      break;
    case sPlaceName :
      mName = nextText( mPrevName );
      mNamePos = enter;
      mPrev = enter;
      //Enter pin name place is completed
      //Text editor on
      mPropText = &(sdGlobalProp->mSymPinNameProp);
      setText( mName, true );
      setStep( sEnterName );
      break;
    case sEnterName :
      applyEdit();
      //Switch to pin number place
      mSmartPoint = mOrigin + mSmartNumber;
      break;
    case sPlaceNumber :
      //Enter pin number place is completed
      mNumberPos = enter;
      addPic( new SdGraphSymPin( mOrigin, sdGlobalProp->mSymPinProp, mNumberPos, sdGlobalProp->mSymPinNumberProp, mNamePos, sdGlobalProp->mSymPinNameProp, mName ), QObject::tr("Enter sym pin") );
      //Calculate smart-params
      mSmartNumber = mNumberPos - mOrigin;
      mSmartName   = mNamePos   - mOrigin;
      mPrevName    = mName;
      setStep( sPlacePin );
      mSmartType  |= snapPrev;
      break;
    }
  }




void SdModeCSymPin::cancelPoint(SdPoint)
  {
  //Set step to start step. When mode restored - we begin from start
  setStep(sPlacePin);
  //Cancel mode
  cancelMode();
  }




void SdModeCSymPin::movePoint( SdPoint p )
  {
  if( getStep() == sPlaceNumber ) {
    mNumberPos = p;
    update();
    }
  else if( getStep() == sPlacePin ) {
    //Get smart point
    //Find end of lines
    SdSnapInfo info;
    info.mSour = p;
    info.mSnapMask = snapEndPoint;
    mObject->forEach( dctPicture, [&info] (SdObject *obj) -> bool {
      SdPtr<SdGraph> graph(obj);
      if( graph.isValid() )
        graph->snapPoint( &info );
      return true;
      });
    if( info.mDestMask )
      mSmartType |= snapEndPoint;
    else
      mSmartType &= ~snapEndPoint;
    mSmartPoint = info.mDest;
    update();
    }
  }




SdPoint SdModeCSymPin::enterPrev()
  {
  if( sdEnvir->mIsSmart && mSmartType )
    enterPoint( mSmartPoint );
  return mSmartPoint;
  }




QString SdModeCSymPin::getStepHelp() const
  {
  switch( getStep() ) {
    case sPlacePin    : return QObject::tr("Enter pin place point"); // "Укажите местрасположение вывода",
    case sPlaceName   : return QObject::tr("Enter pin name place"); // "Укажите местрасположение имени вывода",
    case sEnterName   : return QObject::tr("Type text");  // "Набирайте текст",
    case sPlaceNumber : return QObject::tr("Enter pin number place"); // "Укажите местрасположение номера вывода"
    }
  return QString();
  }




QString SdModeCSymPin::getModeThema() const
  {
  return QString( MODE_HELP "ModeCSymPin.htm" );
  }




QString SdModeCSymPin::getStepThema() const
  {
  switch( getStep() ) {
    case sPlacePin    : return QString( MODE_HELP "ModeCSymPin.htm#sPlacePin" );
    case sPlaceName   : return QString( MODE_HELP "ModeCSymPin.htm#sPlaceName" );
    case sEnterName   : return QString( MODE_HELP "ModeCSymPin.htm#sEnterName" );
    case sPlaceNumber : return QString( MODE_HELP "ModeCSymPin.htm#sPlaceNumber" );
    }
  return QString();
  }




int SdModeCSymPin::getCursor() const
  {
  switch( getStep() ) {
    case sPlacePin    : return CUR_PIN;
    case sPlaceName   : return CUR_PINNAM;
    case sEnterName   : return CUR_TEXT;
    case sPlaceNumber : return CUR_PINNUM;
    }
  return 0;
  }




int SdModeCSymPin::getIndex() const
  {
  return MD_SYM_PIN;
  }



void SdModeCSymPin::cancelEdit()
  {
  mPropText = 0;
  setStep( sPlaceName );
  }



//Name editing is finished
void SdModeCSymPin::applyEdit()
  {
  if( mString.isEmpty() ) {
    QMessageBox::warning( mEditor, QObject::tr("Error"), QObject::tr("Pin name can't be empty") );
    return;
    }
  //Check if pin name unical
  bool unical = true;
  mObject->forEach( dctSymPin, [&unical, this] (SdObject *obj) -> bool {
    SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>(obj);
    if( pin ) {
      if( pin->getPinName() == mString ) unical = false;
      }
    return unical;
    } );
  if( !unical ) {
    QMessageBox::warning( mEditor, QObject::tr("Error"), QObject::tr("Duplicate pin name. Change name or cancel.") );
    return;
    }
  mPropText = 0;
  mName = mString;
  setStep( sPlaceNumber );
  }
