/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Line. Line is linear segment with two ends.
*/
#include "SdGraphLinearLine.h"
#include "SdSegment.h"
#include "SdContext.h"
#include "SdSelector.h"

SdGraphLinearLine::SdGraphLinearLine() :
  SdGraphLinear(),
  a(),
  b(),
  mFlyA(false),
  mFlyB(false)
  {

  }

SdGraphLinearLine::SdGraphLinearLine(SdPoint p1, SdPoint p2, const SdPropLine &propLine) :
  SdGraphLinear( propLine ),
  a(p1),
  b(p2),
  mFlyA(false),
  mFlyB(false)
  {

  }




QString SdGraphLinearLine::getType() const
  {
  return QStringLiteral(SD_TYPE_LINE);
  }



quint64 SdGraphLinearLine::getClass() const
  {
  return dctLines;
  }



void SdGraphLinearLine::cloneFrom(const SdObject *src)
  {
  SdGraphLinear::cloneFrom( src );
  const SdGraphLinearLine *line = dynamic_cast<const SdGraphLinearLine*>(src);
  if( line ) {
    a = line->a;
    b = line->b;
    }
  }



void SdGraphLinearLine::writeObject(QJsonObject &obj) const
  {
  SdGraphLinear::writeObject( obj );
  a.write( QString("a"), obj );
  b.write( QString("b"), obj );
  }




void SdGraphLinearLine::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphLinear::readObject( map, obj );
  a.read( QString("a"), obj );
  b.read( QString("b"), obj );
  }




void SdGraphLinearLine::move(SdPoint offset)
  {
  //Перенести
  if( mFlyA ) a.move( offset );
  if( mFlyB ) b.move( offset );
  }




void SdGraphLinearLine::rotate(SdPoint center, SdPropAngle angle)
  {
  //Повернуть
  if( mFlyA ) a.rotate( center, angle );
  if( mFlyB ) b.rotate( center, angle );
  }




void SdGraphLinearLine::mirror(SdPoint a, SdPoint b)
  {
  //Отразить (зеркальность) относительно линии
  a.mirror( a, b );
  b.mirror( a, b );
  }




void SdGraphLinearLine::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && SdSegment(a,b).isPointOn( p ) ) {
      if( a == p ) { mFlyA = true; mFlyB = false; }
      else if( b == p ) { mFlyA = false; mFlyB = true; }
      else mFlyA = mFlyB = true;
      selector->insert( this );
      }
    }
  }



void SdGraphLinearLine::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && r.isAccross(a,b) ) {
      mFlyA = mFlyB = true;
      selector->insert( this );
      }
    }
  }




void SdGraphLinearLine::select(SdSelector *selector)
  {
  mFlyA = mFlyB = true;
  if( selector != nullptr )
    selector->insert( this );
  }




SdRect SdGraphLinearLine::getOverRect() const
  {
  return SdRect( a, b );
  }




void SdGraphLinearLine::draw(SdContext *dc)
  {
  dc->line( a, b, mProp );
  }



//Find snap point on object
void SdGraphLinearLine::snapPoint(SdSnapInfo *snap)
  {
  if( isVisible() ) {
    if( snap->match(snapEndPoint) ) {
      snap->test( this, a, snapEndPoint );
      snap->test( this, b, snapEndPoint );
      }
    if( snap->match(snapMidPoint) )
      snap->test( this, a.getMiddle( b ), snapMidPoint );
    }
  }



int SdGraphLinearLine::behindCursor(SdPoint p)
  {
  //Определить состояние объекта под курсором
  if( isVisible() && SdSegment(a,b).isPointOn( p ) )
    return getSelector() ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }





void SdGraphLinearLine::saveState(SdUndo *undo)
  {
  undo->propLineAnd3Point( &mProp, &a, &b );
  }