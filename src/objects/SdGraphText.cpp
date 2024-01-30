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
#include "SdProjectItem.h"
#include "SdEnvir.h"
#include "SdJsonIO.h"

#include <QDebug>

SdGraphText::SdGraphText()
  {

  }

SdGraphText::SdGraphText(SdPoint org, const QString &str, SdRect r, const SdPropText &p) :
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




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphText::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraph::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphText> text(src);
  Q_ASSERT_X( text.isValid(), "SdGraphText::cloneFrom", "Cloned not SdGraphText" );
  mOrigin   = text->mOrigin;
  mProp     = text->mProp;
  mString   = text->mString;
  mOverRect = text->mOverRect;
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphText::json(SdJsonWriter &js) const
  {
  mProp.json( QStringLiteral("prop"), js );
  js.jsonString( QStringLiteral("text"), mString );
  js.jsonRect( QStringLiteral("over"), mOverRect );
  js.jsonPoint( QStringLiteral("Origin"), mOrigin );
  SdGraph::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphText::json(const SdJsonReader &js)
  {
  mProp.json( QStringLiteral("prop"), js );
  js.jsonString( QStringLiteral("text"), mString );
  js.jsonRect( QStringLiteral("over"), mOverRect );
  js.jsonPoint( QStringLiteral("Origin"), mOrigin );
  SdGraph::json( js );
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




void SdGraphText::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
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
  if( !dc->showFields() && mString.startsWith( QChar('{') ) && mString.endsWith( QChar('}') ) ) {
    //Draw as field
    //At first try draw with local param
    if( dc->paramContains(mString) ) {
      dc->text( mOrigin, mOverRect, dc->paramGet(mString), mProp );
      return;
      }
    if( getParent() ) {
//      qDebug() << mString << ":" << getParent()->paramHierarchy(mString);
      dc->text( mOrigin, mOverRect, getParent()->paramHierarchy(mString), mProp );
      return;
      }
    }
  //Draw as simple text
  dc->text( mOrigin, mOverRect, mString, mProp );
  }




int SdGraphText::behindCursor(SdPoint p)
  {
  if( mProp.mLayer.isEdited() ) {
    if( mOverRect.isPointInside(p) )
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
    }
  return 0;
  }




int SdGraphText::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  if( mProp.mLayer.isEdited() && mOverRect.isPointInside(p) ) {
    org  = mOrigin;
    dest = mString;
    prop = mProp;
    return 1;
    }
  return 0;
  }




