/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object rectangle.
*/
#include "SdGraphLinearRect.h"
#include "SdSelector.h"
#include "SdSnapInfo.h"
#include "SdContext.h"

SdGraphLinearRect::SdGraphLinearRect() :
  SdGraphLinear(),
  mFly(0)
  {

  }

SdGraphLinearRect::SdGraphLinearRect(SdPoint p1, SdPoint p2, const SdPropLine &propLine) :
  SdGraphLinear(propLine),
  a(p1),
  b(p2),
  mFly(0)
  {
  }




QString SdGraphLinearRect::getType() const
  {
  return QString(SD_TYPE_RECT);
  }




quint64 SdGraphLinearRect::getClass() const
  {
  return dctLines;
  }




void SdGraphLinearRect::cloneFrom(const SdObject *src)
  {
  SdGraphLinear::cloneFrom( src );
  const SdGraphLinearRect *rect = dynamic_cast<const SdGraphLinearRect*>(src);
  if( rect ) {
    a = rect->a;
    b = rect->b;
    }
  }




void SdGraphLinearRect::writeObject(QJsonObject &obj) const
  {
  SdGraphLinear::writeObject( obj );
  a.write( QString("a"), obj );
  b.write( QString("b"), obj );
  }




void SdGraphLinearRect::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphLinear::readObject( map, obj );
  a.read( QString("a"), obj );
  b.read( QString("b"), obj );
  }




void SdGraphLinearRect::move(SdPoint offset)
  {
  a.move( offset );
  if( !mFly ) b.move( offset );
  }




void SdGraphLinearRect::rotate(SdPoint center, SdAngle angle)
  {
  a.rotate( center, angle );
  b.rotate( center, angle );
  }




void SdGraphLinearRect::mirror(SdPoint p1, SdPoint p2)
  {
  a.mirror( p1, p2 );
  b.mirror( p1, p2 );
  }




void SdGraphLinearRect::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( isAble() ) {
    SdRect rect(a,b);
    SdPoint p1(p);
    if( !getSelector() && (p1.isOnSegment( rect.topLeft(), rect.bottomLeft() ) ||
                           p1.isOnSegment( rect.bottomLeft(), rect.bottomRight() ) ||
                           p1.isOnSegment( rect.bottomRight(), rect.topRight() ) ||
                           p1.isOnSegment( rect.topRight(), rect.topLeft() ) ) ) {
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




void SdGraphLinearRect::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( isAble() ) {
    if( !getSelector() && r.isAccross( SdRect(a,b) ) ) {
      mFly = 0;
      selector->insert( this );
      }
    }
  }




void SdGraphLinearRect::select(SdSelector *selector)
  {
  mFly = 0;
  selector->insert( this );
  }




SdRect SdGraphLinearRect::getOverRect() const
  {
  return SdRect(a,b);
  }




void SdGraphLinearRect::draw(SdContext *dc)
  {
  dc->rect( SdRect(a,b), mProp );
  }




int SdGraphLinearRect::behindCursor(SdPoint p)
  {
  if( isAble() ) {
    SdRect rect(a,b);
    if( p.isOnSegment( rect.topLeft(), rect.bottomLeft() ) ||
        p.isOnSegment( rect.bottomLeft(), rect.bottomRight() ) ||
        p.isOnSegment( rect.bottomRight(), rect.topRight() ) ||
        p.isOnSegment( rect.topRight(), rect.topLeft() ) ) {
          return getSelector() ? SEL_ELEM : UNSEL_ELEM;
          }
    }
  return 0;
  }




bool SdGraphLinearRect::snapPoint(SdSnapInfo *snap)
  {
  if( isAble() ) {
    SdRect r(a,b);
    if( snap->match(snapEndPoint) ) {
      snap->test( r.topLeft(), snapEndPoint );
      snap->test( r.bottomLeft(), snapEndPoint );
      snap->test( r.topRight(), snapEndPoint );
      snap->test( r.bottomRight(), snapEndPoint );
      }
    if( snap->match(snapMidPoint) ) {
      snap->test( SdPoint(r.topLeft()).getMiddle( r.topRight() ), snapMidPoint );
      snap->test( SdPoint(r.topRight()).getMiddle( r.bottomRight() ), snapMidPoint );
      snap->test( SdPoint(r.bottomRight()).getMiddle( r.bottomLeft() ), snapMidPoint );
      snap->test( SdPoint(r.bottomLeft()).getMiddle( r.topLeft() ), snapMidPoint );
      }
    return true;
    }
  return false;
  }




void SdGraphLinearRect::saveState(SdUndo *undo)
  {
  SdGraphLinear::saveState( undo );
  undo->point( &a );
  undo->point( &b );
  }
