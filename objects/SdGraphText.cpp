/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphText.h"
#include "SdLayer.h"
#include "SdSelector.h"
#include "SdContext.h"

SdGraphText::SdGraphText()
  {

  }

SdGraphText::SdGraphText(SdPoint org, const QString str, SdRect r, SdPropText &p) :
  SdGraph(),
  mOrigin(org),
  mProp(p),
  mString(str),
  mOverRect(r)
  {

  }




QString SdGraphText::getType() const
  {
  return QString( SD_TYPE_TEXT );
  }




quint64 SdGraphText::getClass() const
  {
  return dctText;
  }




void SdGraphText::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  const SdGraphText *text = dynamic_cast<const SdGraphText*>(src);
  if( text ) {
    mProp     = text->mProp;
    mString   = text->mString;
    mOverRect = text->mOverRect;
    }
  }




void SdGraphText::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject( obj );
  mProp.write( QString("prop"), obj );
  obj.insert( QString("text"), mString );
  mOverRect.write( QString("over"), obj );
  }




void SdGraphText::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mProp.read( QString("prop"), obj );
  mString = obj.value( QString("text") ).toString();
  mOverRect.read( QString("over"), obj );
  }




void SdGraphText::saveState(SdUndo *undo)
  {
  if( undo )
    undo->propTextAndText( &mProp, &mOverRect, &mString );
  }




void SdGraphText::move(SdPoint offset)
  {
  mOverRect.move( offset );
  }




void SdGraphText::rotate(SdPoint center, SdAngle angle)
  {
  SdPoint rc( mOverRect.center() );
  rc.rotate( center, angle );
  mOverRect.moveCenter( rc );
  }




void SdGraphText::mirror(SdPoint a, SdPoint b)
  {
  SdPoint rc( mOverRect.center() );
  rc.mirror( a, b );
  mOverRect.moveCenter( rc );
  }




void SdGraphText::setProp(SdProp &prop)
  {
  mProp = prop.mTextProp;
  }




void SdGraphText::getProp(SdProp &prop)
  {
  prop.mTextProp.append( mProp );
  }




void SdGraphText::setText(int index, QString sour, SdPropText &prop)
  {
  Q_UNUSED(index)
  mString = sour;
  mProp   = prop;
  }




void SdGraphText::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( isAble() ) {
    if( !getSelector() && mOverRect.isPointInside(p) ) {
      selector->insert( this );
      }
    }
  }




void SdGraphText::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( isAble() ) {
    if( !getSelector() && r.isAccross( mOverRect ) ) {
      selector->insert( this );
      }
    }
  }




void SdGraphText::select(SdSelector *selector)
  {
  selector->insert( this );
  }




bool SdGraphText::isAble()
  {
  return mProp.mLayer.layer(false)->isEdited();
  }




SdRect SdGraphText::getOverRect() const
  {
  return mOverRect;
  }




void SdGraphText::draw(SdContext *dc)
  {
  dc->text( mOrigin, mOverRect, mString, mProp );
  }




int SdGraphText::behindCursor(SdPoint p)
  {
  if( isAble() ) {
    if( mOverRect.isPointInside(p) )
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
    }
  return 0;
  }




int SdGraphText::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  if( isAble() ) {
    if( mOverRect.isPointInside(p) ) {
      org  = mOrigin;
      dest = mString;
      prop = mProp;
      return 1;
      }
    }
  return 0;
  }




