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
#include "objects/SdConverterOffset.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemPart.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemSheet.h"
#include "library/SdLibraryStorage.h"
#include "windows/SdPropBarSymImp.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdDGetObject.h"
#include "windows/SdValueSelector.h"
#include <QObject>
#include <QMessageBox>

SdModeCSymImp::SdModeCSymImp(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mSection(nullptr),
  mComponent(nullptr),
  mPart(nullptr),
  mPastePrj(nullptr)
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
  if( mPaste.count() ) {
    //There fragment to insertion
    //Draw all copy in current point
    SdConverterOffset offset( mPrevMove.sub(mFirst) );
    ctx->setConverter( &offset );
    mPaste.draw( ctx );
    }
  else if( mSection ) {
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




void SdModeCSymImp::enterPoint( SdPoint point )
  {
  if( mPaste.count() ) {
    //There fragment
    setDirty();
    mUndo->begin( QObject::tr("Insert fragment"), mObject, false );
    //Insert copy of pasted elements into object without selection them
    mObject->insertObjects( point.sub( mFirst ), &mPaste, mUndo, nullptr, false );
    }
  else {
    //Select value if present
    SdValueSelector::select( &mParams, mEditor );
    //Insert component with params
    addPic( new SdGraphSymImp( mComponent, mSection, mPart, mParams, mOrigin,  &(sdGlobalProp->mSymImpProp) ), QObject::tr("Insert symbol") );
    }
  }




void SdModeCSymImp::cancelPoint(SdPoint)
  {
  getSection();
  }




void SdModeCSymImp::movePoint( SdPoint p )
  {
  mOrigin = p;
  mPrevMove = p;
  update();
  }




void SdModeCSymImp::keyDown(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_F2 :
      //Component rotation [Поворот]
      sdGlobalProp->mSymImpProp.mAngle += 90000;
      propSetToBar();
      update();
      break;
    case Qt::Key_F4 :
      //Mirror component
      sdGlobalProp->mSymImpProp.mMirror = sdGlobalProp->mSymImpProp.mMirror.getValue() ? 0 : 1;
      propSetToBar();
      update();
      break;
    }
  SdModeCommon::keyDown( key, ch );
  }




QString SdModeCSymImp::getStepHelp() const
  {
  if( mPaste.count() )
    return QObject::tr("Enter fragment place point");
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
  return CUR_COMP;
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
    SdObject *obj = SdDGetObject::getComponent( &sectionIndex, &mParams, QObject::tr("Select component to insert"), mEditor );
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

    if( checkSchematicFragment() )
      break;

    if( sectionIndex >= 0 ) {
      mSection = mComponent->extractSymbolFromFactory( sectionIndex );
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


    mPart = mComponent->extractPartFromFactory();
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
  if( mSection != nullptr ) {
    delete mSection;
    mSection = nullptr;
    }
  if( mPart != nullptr ) {
    delete mPart;
    mPart = nullptr;
    }
  mPaste.clear();
  if( mPastePrj != nullptr ) {
    delete mPastePrj;
    mPastePrj = nullptr;
    }
  }




//!
//! \brief checkSchematicFragment Check presence of schematic fragment for mComponent,
//!                               prompt user for select fragment
//! \return                       true - if user selected fragment insertion
//!
bool SdModeCSymImp::checkSchematicFragment()
  {
  QString componentValueParam = mParams.value( stdParamValue ).toLower().trimmed();
  if( !componentValueParam.isEmpty() ) {
    //Accumulate headers matched to filter
    bool presense = false;
    SdLibraryStorage::instance()->forEachHeader( [&presense, componentValueParam] ( SdLibraryHeader &hdr ) -> bool {
      //test class
      if( hdr.mClass & dctProject ) {
        //Split uid to name type and author
        QString uid( hdr.uid() );
        QString name = uid.contains(sdUidDelimiter) ? uid.split( sdUidDelimiter ).at(1) : uid;
        if( name.trimmed().toLower().startsWith( componentValueParam ) )
          return presense = true;
        }
      return false;
      });
    if( presense ) {
      //There at least one fragment with this component
      //Prompt user for select it
      if( QMessageBox::question( mEditor, QObject::tr("Information"), QObject::tr("There at least one fragment with this component. Do You want to select it?") ) == QMessageBox::Yes ) {
        //Show dialog to select fragment
        mPastePrj = sdObjectOnly<SdProject>( SdDGetObject::getObject( dctProject, QObject::tr("Select fragment to insert"), mEditor, componentValueParam ) );
        if( mPastePrj == nullptr ) {
          cancelMode();
          return false;
          }

        SdPItemSheet *sheet = mPastePrj->getFirstSheet();

        if( sheet == nullptr ) {
          QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("No sheets to insert. Try another fragment.") );
          return false;
          }

        //Select all objects in sheet
        sheet->forEach( dctAll, [this] (SdObject *obj) -> bool {
          SdGraph *graph = dynamic_cast<SdGraph*>(obj);
          if( graph != nullptr )
            graph->select( &mPaste );
          return true;
          });


        if( !mPaste.count() ) {
          QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("No objects to insert. Source sheet is empty. Select another fragment.") );
          return false;
          }
        return true;
        }
      }
    }
  return false;
  }

