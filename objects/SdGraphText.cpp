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
  return QStringLiteral( SD_TYPE_TEXT );
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
  mProp.write( QStringLiteral("prop"), obj );
  obj.insert( QStringLiteral("text"), mString );
  mOverRect.write( QStringLiteral("over"), obj );
  mOrigin.write( QStringLiteral("Origin"), obj );
  }




void SdGraphText::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mProp.read( QStringLiteral("prop"), obj );
  mString = obj.value( QStringLiteral("text") ).toString();
  mOverRect.read( QStringLiteral("over"), obj );
  mOrigin.read( QStringLiteral("Origin"), obj );
  }




void SdGraphText::saveState(SdUndo *undo)
  {
  if( undo )
    undo->propTextAndText( &mProp, &mOrigin, &mOverRect, &mString );
  }




void SdGraphText::move(SdPoint offset)
  {
  mOrigin.move( offset );
  mOverRect.move( offset );
  }




void SdGraphText::rotate(SdPoint center, SdPropAngle angle)
  {
  SdPoint rc( mOverRect.center() );
  rc.rotate( center, angle );
  mOverRect.moveCenter( rc );
  mOrigin.rotate( center, angle );
  }




void SdGraphText::mirror(SdPoint a, SdPoint b)
  {
  SdPoint rc( mOverRect.center() );
  rc.mirror( a, b );
  mOverRect.moveCenter( rc );
  mOrigin.mirror( a, b );
  }




void SdGraphText::setProp(SdPropSelected &prop)
  {
  mProp = prop.mTextProp;
  }




void SdGraphText::getProp(SdPropSelected &prop)
  {
  prop.mTextProp.append( mProp );
  prop.mFilledPropMask |= spsTextProp;
  }




void SdGraphText::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(index)
  Q_UNUSED(parent)
  mString = sour;
  mProp   = prop;
  }




void SdGraphText::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && mOverRect.isPointInside(p) ) {
      selector->insert( this );
      }
    }
  }




void SdGraphText::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && r.isAccross( mOverRect ) ) {
      selector->insert( this );
      }
    }
  }





bool SdGraphText::isVisible()
  {
  return mProp.mLayer.isVisible();
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
  if( isVisible() ) {
    if( mOverRect.isPointInside(p) )
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
    }
  return 0;
  }




int SdGraphText::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  if( isVisible() ) {
    if( mOverRect.isPointInside(p) ) {
      org  = mOrigin;
      dest = mString;
      prop = mProp;
      return 1;
      }
    }
  return 0;
  }




