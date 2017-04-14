/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object filled rectangle.
*/
#include "SdGraphLinearRectFilled.h"
#include "SdSelector.h"
#include "SdSnapInfo.h"
#include "SdContext.h"

SdGraphLinearRectFilled::SdGraphLinearRectFilled() :
  SdGraphLinearRect()
  {

  }

SdGraphLinearRectFilled::SdGraphLinearRectFilled(SdPoint p1, SdPoint p2, const SdPropLine &propLine) :
  SdGraphLinearRect( p1, p2, propLine )
  {

  }

QString SdGraphLinearRectFilled::getType() const
  {
  return QString(SD_TYPE_RECT_FILLED);
  }




quint64 SdGraphLinearRectFilled::getClass() const
  {
  return dctLines;
  }

void SdGraphLinearRectFilled::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( isAble() ) {
    SdRect rect(a,b);
    SdPoint p1(p);
    if( !getSelector() && (p1.isOnSegment( rect.topLeft(), rect.bottomLeft() ) ||
                           p1.isOnSegment( rect.bottomLeft(), rect.bottomRight() ) ||
                           p1.isOnSegment( rect.bottomRight(), rect.topRight() ) ||
                           p1.isOnSegment( rect.topRight(), rect.topLeft() ) ||
                           rect.contains(p)) ) {
      //Выбор за одну вершину (нужно производить растяжение/сжатие)
      // перемещаем вершины таким образом, чтобы изменяемой была вершина a
      mFly = 1;
      if( rect.topLeft() == p ) {
        a = rect.topLeft();
        b = rect.bottomRight();
        }
      else if( rect.bottomLeft() == p ) {
        a = rect.bottomLeft();
        b = rect.topRight();
        }
      else if( rect.bottomRight() == p ) {
        a = rect.bottomRight();
        b = rect.topLeft();
        }
      else if( rect.topRight() == p ) {
        a = rect.topRight();
        b = rect.bottomLeft();
        }
      else mFly = 0;
      selector->insert( this );
      }
    }
  }




void SdGraphLinearRectFilled::draw(SdContext *dc)
  {
  dc->fillRect( SdRect(a,b), mProp );
  }




int SdGraphLinearRectFilled::behindCursor(SdPoint p)
  {
  if( isAble() ) {
    SdRect rect(a,b);
    if( rect.contains(p,false) ) {
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
      }
    }
  return 0;
  }

