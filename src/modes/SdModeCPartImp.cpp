/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for part implement
*/
#include "SdModeCPartImp.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdConverterImplement.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemPart.h"
#include "objects/SdPItemComponent.h"
#include "windows/SdPropBarPartImp.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdDGetObject.h"
#include <QObject>
#include <QMessageBox>

SdModeCPartImp::SdModeCPartImp(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mPart(nullptr)
  {

  }




SdModeCPartImp::~SdModeCPartImp()
  {
  clear();
  }




void SdModeCPartImp::activate()
  {
  SdModeCommon::activate();
  getPart();
  }




void SdModeCPartImp::drawDynamic(SdContext *ctx)
  {
  if( mPart ) {
    SdConverterImplement imp( mOrigin, mPart->getOrigin(), sdGlobalProp->mPartImpProp.mAngle, sdGlobalProp->mPartImpProp.mSide.isBottom() );
    ctx->setConverter( &imp );

    mPart->draw( ctx );
    }
  }




int SdModeCPartImp::getPropBarId() const
  {
  return PB_PART_IMP;
  }




void SdModeCPartImp::propGetFromBar()
  {
  SdPropBarPartImp *sbar = dynamic_cast<SdPropBarPartImp*>( SdWCommand::getModeBar(PB_PART_IMP) );
  if( sbar ) {
    sbar->getPropPartImp( sdGlobalProp->mPartImpProp );
    update();
    }
  }




void SdModeCPartImp::propSetToBar()
  {
  SdPropBarPartImp *sbar = dynamic_cast<SdPropBarPartImp*>( SdWCommand::getModeBar(PB_PART_IMP) );
  if( sbar ) {
    sbar->setPropPartImp( sdGlobalProp->mPartImpProp );
    }
  }




void SdModeCPartImp::enterPoint(SdPoint)
  {
  addPic( new SdGraphPartImp( mOrigin, &(sdGlobalProp->mPartImpProp), mPart, nullptr, mPart->paramTable() ), QObject::tr("Insert part") );
  }




void SdModeCPartImp::cancelPoint(SdPoint)
  {
  getPart();
  }




void SdModeCPartImp::movePoint(SdPoint p)
  {
  mOrigin = p;
  update();
  }





void SdModeCPartImp::keyDown(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_F2 :
      //Component rotation [Поворот]
      sdGlobalProp->mPartImpProp.mAngle += 90000;
      propSetToBar();
      update();
      break;
    case Qt::Key_F3 :
      //Flip component to other pcb side [Перенос на другую сторону платы]
      sdGlobalProp->mPartImpProp.mSide = sdGlobalProp->mPartImpProp.mSide == stmTop ? stmBottom : stmTop;
      propSetToBar();
      update();
      break;
    }
  SdModeCommon::keyDown( key, ch );
  }




QString SdModeCPartImp::getStepHelp() const
  {
  return QObject::tr("Enter part place point");
  }




QString SdModeCPartImp::getModeThema() const
  {
  return QString( MODE_HELP "ModeCPartImp.htm" );
  }




QString SdModeCPartImp::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCPartImp::getCursor() const
  {
  return CUR_GROUP;
  }




int SdModeCPartImp::getIndex() const
  {
  return MD_PART_IMP;
  }




//Show dialog with user part selection
void SdModeCPartImp::getPart()
  {
  clear();
  while(1) {
    SdObject *obj = SdDGetObject::getObject( dctPart, QObject::tr("Select part to insert"), mEditor );
    if( obj == nullptr ) {
      cancelMode();
      return;
      }

    clear();

    mPart = sdObjectOnly<SdPItemPart>( obj );

    if( mPart == nullptr ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("Can't load selected part. Select another.") );
      continue;
      }

    if( mPart->isEditEnable() ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("Part is in editing state. Switch it to lock state or select another.") );
      continue;
      }

    break;
    }
  }




//Clear previous part
void SdModeCPartImp::clear()
  {
  if( mPart ) {
    delete mPart;
    mPart = nullptr;
    }
  }



