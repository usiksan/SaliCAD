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
    SdConverterImplement imp( mOrigin, mSection->mOrigin, sdGlobalProp->mSymImpProp.mAngle.getValue(), sdGlobalProp->mSymImpProp.mMirror.getValue() );
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
  getSection();
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



QString SdModeCSymImp::getModeHelp() const
  {
  return QString( MODE_HELP "ModeCSymImp.htm" );
  }



QString SdModeCSymImp::getStepThema() const
  {
  return getModeHelp();
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
  int sectionIndex = -1, partIndex = -1;
  do {
    SdObject *obj = SdDGetObject::getComponent( &sectionIndex, &partIndex, dctSymbol | dctComponent, QObject::tr("Select component to insert"), mEditor );
    if( obj == nullptr ) {
      cancelMode();
      return;
      }

    clear();

    mSection = dynamic_cast<SdPItemSymbol*>( obj );

    if( sectionIndex >= 0 ) {
      mComponent = mSection;
      mSection = mComponent->extractSymbolFromFactory( sectionIndex, false, mEditor );
      }

    if( partIndex >= 0 ) {
      mPart = mComponent->extractPartFromFactory( partIndex, false, mEditor );
      }
    }
  while( mSection == nullptr );
  }




void SdModeCSymImp::clear()
  {
  if( mComponent ) {
    if( mSection == mComponent )
      mSection = nullptr;
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

