/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for symbol implement
*/
#include "SdModeCSymImp.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdConverterImplement.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemPart.h"
#include "objects/SdPItemComponent.h"
#include "windows/SdPropBarSymImp.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdDGetObject.h"
#include <QObject>
#include <QMessageBox>

SdModeCSymImp::SdModeCSymImp(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mSection(nullptr),
  mComponent(nullptr),
  mPart(nullptr)
  {

  }

SdModeCSymImp::~SdModeCSymImp()
  {
  clear();
  }



void SdModeCSymImp::activate()
  {
  SdModeCommon::activate();
  getSection();
  }




void SdModeCSymImp::drawDynamic(SdContext *ctx)
  {
  if( mSection ) {
    SdConverterImplement imp( mOrigin, mSection->getOrigin(), sdGlobalProp->mSymImpProp.mAngle.getValue(), sdGlobalProp->mSymImpProp.mMirror.getValue() );
    ctx->setConverter( &imp );

    mSection->draw( ctx );
    }
  }




int SdModeCSymImp::getPropBarId() const
  {
  return PB_SYM_IMP;
  }




void SdModeCSymImp::propGetFromBar()
  {
  SdPropBarSymImp *sbar = dynamic_cast<SdPropBarSymImp*>( SdWCommand::getModeBar(PB_SYM_IMP) );
  if( sbar ) {
    sbar->getPropSymImp( &(sdGlobalProp->mSymImpProp) );
    update();
    }
  }




void SdModeCSymImp::propSetToBar()
  {
  SdPropBarSymImp *sbar = dynamic_cast<SdPropBarSymImp*>( SdWCommand::getModeBar(PB_SYM_IMP) );
  if( sbar ) {
    sbar->setPropSymImp( &(sdGlobalProp->mSymImpProp) );
    }
  }




void SdModeCSymImp::enterPoint(SdPoint)
  {
  addPic( new SdGraphSymImp( mComponent, mSection, mPart, mOrigin,  &(sdGlobalProp->mSymImpProp) ), QObject::tr("Insert symbol") );
  //getSection();
  }




void SdModeCSymImp::cancelPoint(SdPoint)
  {
  getSection();
  }




void SdModeCSymImp::movePoint( SdPoint p )
  {
  mOrigin = p;
  update();
  }




QString SdModeCSymImp::getStepHelp() const
  {
  return QObject::tr("Enter symbol section place point");
  }



QString SdModeCSymImp::getModeThema() const
  {
  return QString( MODE_HELP "ModeCSymImp.htm" );
  }



QString SdModeCSymImp::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCSymImp::getCursor() const
  {
  return CUR_PLACE;
  }




int SdModeCSymImp::getIndex() const
  {
  return MD_COMPONENT;
  }




void SdModeCSymImp::getSection()
  {
  clear();
  int sectionIndex = -1;
  while(1) {
    SdObject *obj = SdDGetObject::getComponent( &sectionIndex, dctComponent, QObject::tr("Select component to insert"), mEditor );
    if( obj == nullptr ) {
      cancelMode();
      return;
      }

    clear();

    mComponent = sdObjectOnly<SdPItemComponent>( obj );

    if( mComponent == nullptr ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("Can't load selected component. Select another.") );
      continue;
      }

    if( sectionIndex >= 0 ) {
      mSection = mComponent->extractSymbolFromFactory( sectionIndex, false, mEditor );
      if( mSection == nullptr ) {
        QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("Can't load selected component section. Select another.") );
        continue;
        }
      }


    if( mComponent->isEditEnable() ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("Component is in editing state. Switch it to lock state or select another.") );
      continue;
      }
    if( mSection->isEditEnable() ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("Section is in editing state. Switch it to lock state or select another.") );
      continue;
      }


    mPart = mComponent->extractPartFromFactory( false, mEditor );
    if( mPart != nullptr && mPart->isEditEnable() ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("Part is in editing state. Switch it to lock state or select another.") );
      continue;
      }

    break;
    }
  }




void SdModeCSymImp::clear()
  {
  if( mComponent != nullptr ) {
    delete mComponent;
    mComponent = nullptr;
    }
  if( mSection ) {
    delete mSection;
    mSection = nullptr;
    }
  if( mPart ) {
    delete mPart;
    mPart = nullptr;
    }
  }

