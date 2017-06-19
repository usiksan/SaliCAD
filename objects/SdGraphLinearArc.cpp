/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Arc.
*/
#include "SdGraphLinearArc.h"

SdGraphLinearArc::SdGraphLinearArc()
  {

  }




void SdGraphLinearArc::cloneFrom(const SdObject *src)
  {
  SdGraphLinear::cloneFrom( src );
  const SdGraphLinearArc *arc = dynamic_cast<const SdGraphLinearArc*>(src);
  mArcRect = arc->mArcRect;
  mStart   = arc->mStart;
  mStop    = arc->mStop;
  }




void SdGraphLinearArc::writeObject(QJsonObject &obj) const
  {
  SdGraphLinear::writeObject( obj );
  mArcRect.write( QStringLiteral("Rect"), obj );
  obj.insert( QStringLiteral("Start"), mStart );
  obj.insert( QStringLiteral("Stop"),  mStop );
  }




void SdGraphLinearArc::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphLinear::readObject( map, obj );
  mArcRect.read( QStringLiteral("Rect"), obj );
  mStart = obj.value( QStringLiteral("Start") ).toInt();
  mStop  = obj.value( QStringLiteral("Stop") ).toInt();
  }




void SdGraphLinearArc::saveState(SdUndo *undo)
  {
  undo->propLineRect2Int( &mProp, &mArcRect, &mStart, &mStop );
  }




void SdGraphLinearArc::move(SdPoint offset)
  {
  SdPoint center = mArcRect.center();
  center.move( offset );
  mArcRect.moveCenter( center );
  }




void SdGraphLinearArc::rotate(SdPoint center, SdAngle angle)
  {
  SdPoint cnt = mArcRect.center();
  cnt.rotate( center, angle );
  mArcRect.moveCenter( cnt );
  if( mStart >= 0 && mStop >= 0 ) {
    //Counter-clock wise
    mStart += angle.getValue();
    mStart %= 360000;
    mStop  += angle.getValue();
    mStop  %= 360000;
    }
  else {
    mStart += angle.getValue();
    mStart %= 360000;
    if( mStart >= 0 )
      mStart -= 360000;
    mStop  += angle.getValue();
    mStop  %= 360000;
    if( mStop >= 0 )
      mStop -= 360000;
    }
  }




void SdGraphLinearArc::mirror(SdPoint a, SdPoint b)
  {
  SdPoint cnt = mArcRect.center();
  cnt.mirror( a, b );
  mArcRect.moveCenter( cnt );
  mStart -= 360000;
  mStop  -= 360000;
  }



void SdGraphLinearArc::selectByPoint(const SdPoint p, SdSelector *selector)
  {

  }



void SdGraphLinearArc::selectByRect(const SdRect &r, SdSelector *selector)
  {
  }

void SdGraphLinearArc::select(SdSelector *selector)
  {
  }

SdRect SdGraphLinearArc::getOverRect() const
  {
  }

void SdGraphLinearArc::draw(SdContext *dc)
  {
  }

int SdGraphLinearArc::behindCursor(SdPoint p)
  {
  }

bool SdGraphLinearArc::snapPoint(SdSnapInfo *snap)
  {
  }
