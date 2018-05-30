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
#include "objects/SdGraphText.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>

SdModeCText::SdModeCText(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCTextual( editor, obj ),
  mPicAffected(nullptr), //Element containing edititing text [Элемент, содержащий редактируемый текст]
  mIdAffected(0),        //Text item number for editing element [Номер редактируемого текста в элементе]
  mEditText()            //Properties for edited text
  {

  }




void SdModeCText::drawStatic(SdContext *ctx)
  {
  mObject->forEach( dctAll, [=]( SdObject *obj ) {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph && graph != mPicAffected )
      graph->draw( ctx );
    return true;
    });
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
    //It's edit or enter new text [Определим цикл ввода или редактирования]
    mIdAffected = 0;
    mPicAffected = nullptr;
    mEditText.clear();
    mObject->forEach( dctAll, [=]( SdObject *obj ) {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph ) {
        mIdAffected = graph->behindText( enter, mPrev, mString, mEditText );
        if( mIdAffected ) {
          mPicAffected = graph;
          return false;
          }
        }
      return true;
      });

    if( mIdAffected && mPicAffected ) {
      //Режим редактирования
      //Установить локальные свойства
      mPropText = &mEditText;
      propSetToBar();
      //Осуществить редактирование
      setText( mString, true );
      setStep( sEdit );
      mEditor->dirtyCashe();
      update();
      }
    else {
      //Осуществить ввод текста
      mPicAffected = nullptr;
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




QString SdModeCText::getModeThema() const
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
  mPropText = nullptr;
  setStep( sPlace );
  update();
  }




void SdModeCText::applyEdit()
  {
  if( getStep() == sEnter )
    addPic( new SdGraphText( mPrev, mString, mOverRect, sdGlobalProp->mTextProp ), QObject::tr("Text insertion") );
  else if( getStep() == sEdit && mPicAffected ) {
    //Store previous state of editable object
    mUndo->begin( QObject::tr("Text edit"), mObject );
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
