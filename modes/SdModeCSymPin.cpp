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
#include "windows/SdPropBarTextual.h"
#include "windows/SdWCommand.h"
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
    }
  }

  switch( getStep() ) {
    case sPlaceNumber :

  }

void SdModeCSymPin::propSetToBar()
  {
  }

void SdModeCSymPin::enterPoint(SdPoint)
  {
  }

void SdModeCSymPin::cancelPoint(SdPoint)
  {
  }

void SdModeCSymPin::movePoint(SdPoint)
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
  }

void SdModeCSymPin::applyEdit()
  {
  }
