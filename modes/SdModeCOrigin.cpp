/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for symbol pin origin
*/
#include "SdModeCOrigin.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>

SdModeCOrigin::SdModeCOrigin(SdWEditorGraph *editor, SdProjectItem *obj, int osize) :
  SdModeCommon( editor, obj ),
  mOriginSize(osize),
  mModeIndex(MD_SYM_ORIGIN)
  {

  }


void SdModeCOrigin::activate()
  {
  SdModeCommon::activate();
  update();
  }



void SdModeCOrigin::drawDynamic(SdContext *ctx)
  {
  //Show current position of origin
  ctx->cross( mObject->getOrigin(), mOriginSize, sdEnvir->getSysColor(scEnter) );
  ctx->circle( mObject->getOrigin(), mOriginSize );
  }




void SdModeCOrigin::enterPoint( SdPoint enter )
  {
  mObject->setOrigin( enter, mUndo );
  mEditor->dirtyCashe();
  mEditor->dirtyProject();
  update();
  }




void SdModeCOrigin::movePoint(SdPoint)
  {
  }



QString SdModeCOrigin::getStepHelp() const
  {
  return QObject::tr("Enter new origin point");
  }



QString SdModeCOrigin::getModeThema() const
  {
  return QString( MODE_HELP "ModeCOrigin.htm" );
  }



QString SdModeCOrigin::getStepThema() const
  {
  return getModeThema();
  }



int SdModeCOrigin::getCursor() const
  {
  return CUR_ORG;
  }



int SdModeCOrigin::getIndex() const
  {
  return mModeIndex;
  }

