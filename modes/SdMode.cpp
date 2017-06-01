/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdMode.h"
#include "objects/SdGraph.h"
#include "objects/SdPulsar.h"
#include "objects/SdProject.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdWCommand.h"

#include <QDebug>


SdMode::SdMode(SdWEditorGraph *editor, SdProjectItem *obj) :
  mStep(0),
  mObject(obj),
  mEditor(editor),
  mUndo(0)
  {
  if( obj ) {
    mUndo = obj->getUndo();
    }
  }

SdMode::~SdMode()
  {

  }




void SdMode::restore()
  {
  //update step definite params
  SdPulsar::pulsar->emitSetStatusMessage( getStepHelp() );
  mEditor->viewport()->setCursor( loadCursor(getCursor()) );

  //setup mode step properties bar
  SdWCommand::setModeBar( getPropBarId() );
  //setup properties in bar
  propSetToBar();

  update();
  }




void SdMode::activate()
  {
  restore();
  }




void SdMode::reset()
  {

  }



void SdMode::drawStatic(SdContext *ctx)
  {
  //By default draw all picture
  mObject->draw( ctx );
  }



void SdMode::drawDynamic(SdContext *ctx)
  {
  Q_UNUSED(ctx)
  }




void SdMode::keyDown(int key, QChar ch)
  {
  if( key == Qt::Key_Asterisk )
    mEditor->cmViewFit();
  else if( key == Qt::Key_Plus )
    mEditor->scaleStep( 2.0 );
  else if( key == Qt::Key_Minus )
    mEditor->scaleStep( 0.5 );
  Q_UNUSED(ch)
  }



void SdMode::keyUp( int key, QChar ch )
  {
  Q_UNUSED(key)
  Q_UNUSED(ch)
  }



//Режим имеет объекты для копирования
bool SdMode::enableCopy() const
  {
  return false;
  }



//Режим разрешает вставку объектов с заданной маской
bool SdMode::enablePaste(quint64 pasteMask) const
  {
  Q_UNUSED(pasteMask)
  return false;
  }



//Получить всплывающую информацию в заданной точке
bool SdMode::getInfo(SdPoint p, QString &info)
  {
  bool ok = false;

  //Пройти по всем объектам и получить информацию из объекта под точкой
  mObject->forEach( dctAll, [p,&info,&ok](SdObject *obj) -> bool {
    //Работаем только с графическими объектами
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph ) {
      //Получить информацию
      if( graph->getInfo( p, info, true ) ) ok = true;
      }
    return !ok;
    } );

  return ok;
  }



void SdMode::update() {
  mEditor->viewport()->update();
  }



void SdMode::cancelMode()
  {
  mEditor->modeCancel();
  }




SdPoint SdMode::getGrid() const
  {
  return mEditor->gridGet();
  }



void SdMode::setStep(int stp)
  {
  //assign new step
  mStep = stp;

  //restore mode step state
  restore();
  }



void SdMode::setDirty()
  {
  mEditor->dirtyCashe();
  mEditor->dirtyProject();
  }



void SdMode::setDirtyCashe()
  {
  mEditor->dirtyCashe();
  }




void SdMode::addPic( SdObject *obj, QString title )
  {
  if( mUndo )
    mUndo->begin( title );
  mObject->insertChild( obj, mUndo );
  mEditor->dirtyCashe();
  mEditor->dirtyProject();
  }

