/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for part pin
*/
#include "SdModeCPartPin.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include "objects/SdGraphPartPin.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdPropBarPartPin.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>
#include <QMessageBox>

#define sPlacePin    0
#define sPlaceNumber 1
#define sEnterNumber 2
#define sPlaceName   3
#define sLast        4

SdPoint
  SdModeCPartPin::mSmartNumber,       //Offset from previous number
  SdModeCPartPin::mSmartName;         //Offset from previous name
QString SdModeCPartPin::mPrevNumber;  //Previous number

SdModeCPartPin::SdModeCPartPin(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCTextual( editor, obj ),
  mSmartType(0)
  {

  }




void SdModeCPartPin::drawDynamic(SdContext *ctx)
  {
  SdRect r;
  switch( getStep() ) {
    case sPlaceName :
      //Rectangle for name
      ctx->text( mNamePos, r, QString("XX"), sdGlobalProp->mPartPinNameProp );
      ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltDashed );
      ctx->rect( r );
      //Pin number
      ctx->setFont( sdGlobalProp->mPartPinNumberProp );
      ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltSolid );
      ctx->textEx( mNumberPos, r, mNumber, sdGlobalProp->mPartPinNumberProp.mDir, sdGlobalProp->mPartPinNumberProp.mHorz, sdGlobalProp->mPartPinNumberProp.mVert );

      [[fallthrough]];
    case sEnterNumber :
      drawText( ctx );

      [[fallthrough]];
    case sPlaceNumber :
      ctx->cross( mOrigin, sdEnvir->mPartPinSize, sdEnvir->getSysColor(scEnter) );
    }

  //Draw smart point
  if( sdEnvir->mIsSmart ) {
    if( (mSmartType & snapEndPoint) && (getStep() == sPlacePin) )
      ctx->smartPoint( mSmartPoint, snapEndPoint );
    if( (mSmartType & snapPrev) && (getStep() == sPlaceName || getStep() == sPlaceNumber) )
      ctx->smartPoint( mSmartPoint, snapPrev );
    }
  }




int SdModeCPartPin::getPropBarId() const
  {
  switch( getStep() ) {
    case sPlaceNumber :
    case sPlaceName   :
    case sEnterNumber :
      return PB_TEXT;
    }
  return PB_PART_PIN;
  }




void SdModeCPartPin::propGetFromBar()
  {
  if( getStep() == sPlaceName ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) {
      tbar->getPropText( &(sdGlobalProp->mPartPinNameProp) );
      mEditor->setFocus();
      update();
      }
    }
  else if( getStep() == sEnterNumber ) SdModeCTextual::propGetFromBar();
  else if( getStep() == sPlaceNumber ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) {
      tbar->getPropText( &(sdGlobalProp->mPartPinNumberProp) );
      mEditor->setFocus();
      update();
      }
    }
  else if( getStep() == sPlacePin ) {
    SdPropBarPartPin *sbar = dynamic_cast<SdPropBarPartPin*>( SdWCommand::getModeBar(PB_PART_PIN) );
    if( sbar ) {
      sbar->getPropPartPin( &(sdGlobalProp->mPartPinProp) );
      mEditor->setFocus();
      update();
      }
    }
  }




void SdModeCPartPin::propSetToBar()
  {
  if( getStep() == sPlaceName ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) tbar->setPropText( &(sdGlobalProp->mPartPinNameProp), mEditor->getPPM() );
    }
  else if( getStep() == sEnterNumber ) SdModeCTextual::propSetToBar();
  else if( getStep() == sPlaceNumber ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) tbar->setPropText( &(sdGlobalProp->mPartPinNumberProp), mEditor->getPPM() );
    }
  else if( getStep() == sPlacePin ) {
    SdPropBarPartPin *sbar = dynamic_cast<SdPropBarPartPin*>( SdWCommand::getModeBar(PB_PART_PIN) );
    if( sbar ) sbar->setPropPartPin( &(sdGlobalProp->mPartPinProp) );
    }
  }




void SdModeCPartPin::enterPoint(SdPoint enter)
  {
  switch( getStep() ) {
    case sPlacePin :
      //Enter pin place
      mOrigin = enter;
      //Switch to pin number place
      mSmartPoint = mOrigin + mSmartNumber;
      setStep( sPlaceNumber );
      break;
    case sPlaceNumber :
      mNumber = nextText( mPrevNumber );
      mNumberPos = enter;
      mPrev = enter;
      //Enter pin number place is completed
      //Text editor on
      mPropText = &(sdGlobalProp->mPartPinNumberProp);
      setText( mNumber, true );
      setStep( sEnterNumber );
      break;
    case sEnterNumber :
      applyEdit();
      break;
    case sPlaceName :
      //Enter pin name place is completed
      mNamePos = enter;
      addPic( new SdGraphPartPin( mOrigin, sdGlobalProp->mPartPinProp, mNumberPos, sdGlobalProp->mPartPinNumberProp,  mNamePos, sdGlobalProp->mPartPinNameProp, mNumber ), QObject::tr("Enter sym pin") );
      //Calculate smart-params
      mSmartNumber = mNumberPos - mOrigin;
      mSmartName   = mNamePos   - mOrigin;
      mPrevNumber  = mNumber;
      setStep( sPlacePin );
      mSmartType  |= snapPrev;
      break;
    }
  }




void SdModeCPartPin::cancelPoint(SdPoint)
  {
  //Reset step to initial
  setStep(sPlacePin);
  //Cancel mode
  cancelMode();
  }




void SdModeCPartPin::movePoint(SdPoint p)
  {
  if( getStep() == sPlaceName ) {
    mNamePos = p;
    update();
    }
  else if( getStep() == sPlacePin ) {
    //Get smart point
    //Find end of lines
    SdSnapInfo info;
    info.mSour = p;
    info.mSnapMask = snapEndPoint;
    info.scan( mObject, dctPicture );
    if( info.isFound() )
      mSmartType |= snapEndPoint;
    else
      mSmartType &= ~snapEndPoint;
    mSmartPoint = info.mDest;
    update();
    }
  }




SdPoint SdModeCPartPin::enterPrev()
  {
  if( sdEnvir->mIsSmart && mSmartType )
    enterPoint( mSmartPoint );
  return mSmartPoint;
  }




QString SdModeCPartPin::getStepHelp() const
  {
  switch( getStep() ) {
    case sPlacePin    : return QObject::tr("Enter pin place point"); // "Укажите местрасположение вывода",
    case sPlaceName   : return QObject::tr("Enter pin name place"); // "Укажите местрасположение имени вывода",
    case sEnterNumber : return QObject::tr("Type text");  // "Набирайте текст",
    case sPlaceNumber : return QObject::tr("Enter pin number place"); // "Укажите местрасположение номера вывода"
    }
  return QString();
  }




QString SdModeCPartPin::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCPartPin.htm" );
  }




QString SdModeCPartPin::getStepThema() const
  {
  switch( getStep() ) {
    case sPlacePin    : return QStringLiteral( MODE_HELP "ModeCPartPin.htm#sPlacePin" );
    case sPlaceName   : return QStringLiteral( MODE_HELP "ModeCPartPin.htm#sPlaceName" );
    case sEnterNumber : return QStringLiteral( MODE_HELP "ModeCPartPin.htm#sEnterName" );
    case sPlaceNumber : return QStringLiteral( MODE_HELP "ModeCPartPin.htm#sPlaceNumber" );
    }
  return QString();
  }




int SdModeCPartPin::getCursor() const
  {
  switch( getStep() ) {
    case sPlacePin    : return CUR_PIN;
    case sPlaceName   : return CUR_PINNAM;
    case sEnterNumber : return CUR_TEXT;
    case sPlaceNumber : return CUR_PINNUM;
    }
  return 0;
  }



int SdModeCPartPin::getIndex() const
  {
  return MD_PART_PIN;
  }




void SdModeCPartPin::cancelEdit()
  {
  mPropText = nullptr;
  setStep( sPlaceNumber );
  }




void SdModeCPartPin::applyEdit()
  {
  if( mString.isEmpty() ) {
    QMessageBox::warning( mEditor, QObject::tr("Error"), QObject::tr("Pin number can't be empty") );
    return;
    }
  //Check if pin number unical
  bool unical = true;
  mObject->forEach( dctPartPin, [&unical, this] (SdObject *obj) -> bool {
    SdGraphPartPin *pin = dynamic_cast<SdGraphPartPin*>(obj);
    if( pin ) {
      if( pin->getPinNumber() == mString ) unical = false;
      }
    return unical;
    } );
  if( !unical ) {
    QMessageBox::warning( mEditor, QObject::tr("Error"), QObject::tr("Duplicate pin number. Change number or cancel.") );
    return;
    }

  mPropText = nullptr;
  mNumber = mString;
  setStep( sPlaceName );
  //Switch to pin name place
  mSmartPoint = mOrigin + mSmartName;
  }


