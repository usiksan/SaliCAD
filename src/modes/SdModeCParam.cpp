#include "SdModeCParam.h"
#include "objects/SdGraphParam.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>

SdModeCParam::SdModeCParam(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCText( editor, obj )
  {

  }



QString SdModeCParam::getStepHelp() const
  {
  if( getStep() == sPlace ) return QObject::tr("Enter point to insert or edit param aka (paramName = paramValue)");
  if( getStep() == sEnter ) return QObject::tr("Enter param aka (paramName = paramValue)");
  if( getStep() == sEdit ) return QObject::tr("Edit param aka (paramName = paramValue)");
  return QString();
  }




QString SdModeCParam::getModeThema() const
  {
  return QString( MODE_HELP "ModeCParam.htm" );
  }




QString SdModeCParam::getStepThema() const
  {
  if( getStep() == sPlace ) return QString( MODE_HELP "ModeCParam.htm#place" );
  if( getStep() == sEnter ) return QString( MODE_HELP "ModeCParam.htm#enter" );
  if( getStep() == sEdit ) return QString( MODE_HELP "ModeCParam.htm#edit" );
  return QString();
  }



int SdModeCParam::getIndex() const
  {
  return MD_PARAM;
  }



void SdModeCParam::applyEdit()
  {
  if( getStep() == sEnter )
    addPic( new SdGraphParam( mPrev, mString, mOverRect, sdGlobalProp->mTextProp ), QObject::tr("Param insertion") );
  else if( getStep() == sEdit && mPicAffected ) {
    //Store previous state of editable object
    mUndo->begin( QObject::tr("Text edit"), mObject, false );
    mPicAffected->saveState( mUndo );
    mPicAffected->setText( mIdAffected, mString, mEditText, mEditor );
    mPicAffected = nullptr;
    mEditor->dirtyProject();
    }
  mPropText = nullptr;
  setStep( sPlace );
  mEditor->dirtyCashe();
  update();
  }
