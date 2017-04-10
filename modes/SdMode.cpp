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
#include "windows/SdWEditorGraph.h"


SdMode::SdMode(SdWEditorGraph *editor, SdProjectItem *obj) :
  mObject(obj),
  mEditor(editor),
  mStep(0)
  {

  }

SdMode::~SdMode()
  {

  }

void SdMode::activate()
  {
  SdPulsar::pulsar->emitSetStatusMessage( getStepHelp() );
  mEditor->viewport()->setCursor( loadCursor(getCursor()) );
  }

void SdMode::reset()
  {

  }


void SdMode::drawStatic(SdContext *ctx)
  {
  Q_UNUSED(ctx)
  }

void SdMode::drawDynamic(SdContext *ctx)
  {
  Q_UNUSED(ctx)
  }


void SdMode::keyDown(int key, QChar ch)
  {
  Q_UNUSED(key)
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
  mObject->forEach( -1L, [p,&info,&ok](SdObject *obj) -> bool {
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

