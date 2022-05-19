/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCFragment.h"
#include "objects/SdEnvir.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdGraphNetWire.h"
#include "objects/SdPulsar.h"
#include "objects/SdConverterOffset.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdDGetObject.h"

#include <QMessageBox>

SdModeCFragment::SdModeCFragment(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mPastePrj(nullptr)
  {

  }




SdModeCFragment::~SdModeCFragment()
  {
  clear();
  }





void SdModeCFragment::activate()
  {
  SdModeCommon::activate();
  getFragment();
  }




void SdModeCFragment::drawDynamic(SdContext *ctx)
  {
  //Draw all copy in current point
  SdConverterOffset offset( mPrevMove.sub(mFirst) );
  ctx->setConverter( &offset );
  mPaste.draw( ctx );
  }




int SdModeCFragment::getPropBarId() const
  {
  return PB_DEFAULT;
  }




void SdModeCFragment::enterPoint(SdPoint point)
  {
  if( mPaste.count() ) {
    setDirty();
    mUndo->begin( QObject::tr("Insert fragment"), mObject, false );
    //Insert copy of pasted elements into object without selection them
    mObject->insertObjects( point.sub( mFirst ), &mPaste, mUndo, nullptr, false );
    }
  }




void SdModeCFragment::cancelPoint(SdPoint)
  {
  getFragment();
  }




void SdModeCFragment::movePoint(SdPoint p)
  {
  mPrevMove = p;
  update();
  }




QString SdModeCFragment::getStepHelp() const
  {
  return QObject::tr("Enter fragment place point");
  }




QString SdModeCFragment::getModeThema() const
  {
  return QString( MODE_HELP "ModeCFragment.htm" );
  }




QString SdModeCFragment::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCFragment::getCursor() const
  {
  return CUR_PLACE;
  }




int SdModeCFragment::getIndex() const
  {
  return MD_FRAGMENT;
  }





void SdModeCFragment::getFragment()
  {
  while(1) {
    clear();
    mPastePrj = sdObjectOnly<SdProject>( SdDGetObject::getObject( dctProject, QObject::tr("Select fragment to insert"), mEditor ) );
    if( mPastePrj == nullptr ) {
      cancelMode();
      return;
      }

    SdPItemSheet *sheet = mPastePrj->getFirstSheet();

    if( sheet == nullptr ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("No sheets to insert. Try another fragment.") );
      continue;
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
      continue;
      }

    break;
    }
  update();
  }



void SdModeCFragment::clear()
  {
  mPaste.clear();
  if( mPastePrj ) {
    delete mPastePrj;
    mPastePrj = nullptr;
    }
  }
