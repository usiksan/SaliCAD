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
#include "objects/SdGraphObject.h"
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

  }

void SdMode::reset()
  {

  }

void SdMode::draw(SdContext &)
  {

  }

void SdMode::show(SdContext &)
  {

  }

void SdMode::hide(SdContext &)
  {

  }

bool SdMode::keyDown(int, SdContext &)
  {

  }

bool SdMode::keyUp(int, SdContext &)
  {

  }


//Режим имеет объекты для копирования
bool SdMode::enableCopy() const
  {
  return false;
  }



//Режим разрешает вставку объектов с заданной маской
bool SdMode::enablePaste(quint64 pasteMask) const
  {
  return false;
  }


//Получить всплывающую информацию в заданной точке
bool SdMode::getInfo(SdPoint p, QString &info)
  {
  bool ok = false;

  //Пройти по всем объектам и получить информацию из объекта под точкой
  mObject->forEach( -1L, [p,&info,&ok](SdObject *obj) -> bool {
    //Работаем только с графическими объектами
    SdGraphObject *graph = dynamic_cast<SdGraphObject*>( obj );
    if( graph ) {
      //Получить информацию
      if( graph->getInfo( p, info, true ) ) ok = true;
      }
    return !ok;
    } );

  return ok;
  }


void SdMode::update() { mEditor->viewport()->update(); }

