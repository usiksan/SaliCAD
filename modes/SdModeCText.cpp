/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for text edit
*/
#include "SdModeCText.h"
#include <QObject>

SdModeCText::SdModeCText(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCTextual( editor, obj )
  {

  }


void SdModeCText::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sEnter || getStep() == sEdit )
    drawText( ctx );
  }




void SdModeCText::enterPoint(SdPoint enter)
  {
  if( getStep() ) SdModeCTextual::enterPoint( enter );
  else {
    //Определим цикл ввода или редактирования
    mEditText.clear();
    mObject->forEach( 0, [=]( SdObject *obj ) {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph ) {
        mIdAffected = graph->behindText( enter, mString, mEditText );
        if( mIdAffected ) {
          mPicAffected = graph;
          return true;
          }
        }
      return false;
      });

    if( mIdAffected && mPicAffected ) {
      //Режим редактирования
      //Установить локальные свойства
      mPropText = &mEditText;
      propSetToBar();
      //Осуществить редактирование
      setText( mString, true );
      setStep( sEdit );
      update();
      }
    else {
      //Осуществить ввод текста
      mPicAffected = 0;
      mPrev = enter;
      setText( QString(""), false );
      //Установить глобальные свойства
      mPropText = &(sdGlobalProp->mTextProp);
      propSetToBar();
      //Осуществить редактирование
      setStep( sEnter );
      update();
      }
    }
  }




void SdModeCText::cancelPoint(SdPoint p)
  {
  if( getStep() ) SdModeCTextual::cancelPoint( p );
  else cancelMode();
  }




QString SdModeCText::getStepHelp() const
  {
  if( getStep() == sPlace ) return QObject::tr("Enter point to insert or edit text");
  if( getStep() == sEnter ) return QObject::tr("Enter text");
  if( getStep() == sEdit ) return QObject::tr("Edit text");
  return QString();
  }




QString SdModeCText::getModeHelp() const
  {
  return QString( MODE_HELP "ModeCText.htm" );
  }




QString SdModeCText::getStepThema() const
  {
  if( getStep() == sPlace ) return QString( MODE_HELP "ModeCText.htm#place" );
  if( getStep() == sEnter ) return QString( MODE_HELP "ModeCText.htm#enter" );
  if( getStep() == sEdit ) return QString( MODE_HELP "ModeCText.htm#edit" );
  return QString();
  }




int SdModeCText::getIndex() const
  {
  return MD_TEXT;
  }




void SdModeCText::cancelEdit()
  {
  mPropText = 0;
  setStep( sPlace );
  }




void SdModeCText::applyEdit()
  {
  if( getStep() == sEnter ) {

    }
  else if( getStep() == sEdit ) {

    }
  mPropText = 0;
  setStep( sPlace );
  }
