/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base mode for operation on objects.
*/

#include "SdMode.h"
#include "objects/SdGraph.h"
#include "objects/SdPulsar.h"
#include "objects/SdProject.h"
#include "objects/SdEnvir.h"
#include "objects/SdPItemPlate.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdWCommand.h"

#include <QDebug>


SdMode::SdMode(SdWEditorGraph *editor, SdProjectItem *obj) :
  mStep(0),
  mObject(obj),
  mEditor(editor),
  mUndo(nullptr)
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
  SdPulsar::sdPulsar->emitSetStatusMessage( getStepHelp() );
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





//Calling before mode set to inactive state
void SdMode::deactivate()
  {

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
  else if( key == Qt::Key_Plus ) {
    if( sdEnvir->mCenterCursor )
      mEditor->originSet( mEditor->cursorPosition() );
    mEditor->scaleStep( 2.0 );
    }
  else if( key == Qt::Key_Minus ) {
    if( sdEnvir->mCenterCursor )
      mEditor->originSet( mEditor->cursorPosition() );
    mEditor->scaleStep( 0.5 );
    }
  else if( key == Qt::Key_Space )
    enterPrev();
  else if( key == Qt::Key_X ) {
    //Switch cursor view
    sdEnvir->mCursorView++;
    if( sdEnvir->mCursorView >= dcvLast )
      sdEnvir->mCursorView = dcvNone;
    update();
    }
  else if( key == Qt::Key_Up )
    mEditor->cursorMove( 0, 1 );
  else if( key == Qt::Key_Down )
    mEditor->cursorMove( 0, -1 );
  else if( key == Qt::Key_Left )
    mEditor->cursorMove( -1, 0 );
  else if( key == Qt::Key_Right )
    mEditor->cursorMove( 1, 0 );
  else if( key == Qt::Key_Enter )
    enterPoint( mEditor->cursorPosition() );
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
  update();
  mEditor->modeCancel();
  }




SdPoint SdMode::getGrid() const
  {
  return mEditor->gridGet();
  }




double SdMode::getPPM() const
  {
  return mEditor->getPPM();
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



//If container is SdPlate object then set dirty rat net for its rebuild
void SdMode::setDirtyRatNet()
  {
  if( mObject->getClass() == dctPlate ) {
    SdPtr<SdPItemPlate> plate(mObject);
    if( plate.isValid() ) plate->setDirtyRatNet();
    }
  }




void SdMode::addPic( SdObject *obj, QString title )
  {
  if( mUndo )
    mUndo->begin( title, mObject );
  mObject->insertChild( obj, mUndo );
  mEditor->dirtyCashe();
  mEditor->dirtyProject();
  }

