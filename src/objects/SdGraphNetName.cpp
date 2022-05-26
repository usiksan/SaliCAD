/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Net name in schematic diagram
*/
#include "SdGraphNetName.h"
#include "SdPItemSheet.h"
#include "SdContext.h"
#include "SdEnvir.h"
#include "SdSelector.h"
#include "SdJsonIO.h"

SdGraphNetName::SdGraphNetName() :
  SdGraphNet()
  {

  }

SdGraphNetName::SdGraphNetName(SdPoint org, const QString netName, const SdPropText &prp) :
  SdGraphNet(netName),
  mOrigin(org),
  mProp(prp)
  {

  }



//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphNetName::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraphNet::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphNetName> wname(src);
  Q_ASSERT( wname.isValid() );
  mOrigin = wname->mOrigin;
  mProp   = wname->mProp;
  mOver   = wname->mOver;
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphNetName::json(SdJsonWriter &js) const
  {
  js.jsonPoint( QStringLiteral("Org"), mOrigin );
  mProp.json( QStringLiteral("Prop"), js );
  js.jsonRect( QStringLiteral("Over"), mOver );
  SdGraphNet::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphNetName::json(const SdJsonReader &js)
  {
  js.jsonPoint( QStringLiteral("Org"), mOrigin );
  mProp.json( QStringLiteral("Prop"), js );
  js.jsonRect( QStringLiteral("Over"), mOver );
  SdGraphNet::json( js );
  }








void SdGraphNetName::saveState(SdUndo *undo)
  {
  SdGraphNet::saveState(undo);
  undo->propTextAndText( &mProp, &mOrigin, &mOver, nullptr );
  }




void SdGraphNetName::move(SdPoint offset)
  {
  mOrigin.move( offset );
  }




void SdGraphNetName::rotate(SdPoint center, SdPropAngle angle)
  {
  mOrigin.rotate( center, angle );
  mProp.mDir += angle;
  }




void SdGraphNetName::mirror(SdPoint a, SdPoint b)
  {
  mOrigin.mirror( a, b );
  }




void SdGraphNetName::setProp(SdPropSelected &prop)
  {
  mProp = prop.mTextProp;
  SdGraphNet::setProp( prop );
  }




void SdGraphNetName::getProp(SdPropSelected &prop)
  {
  prop.mTextProp.append( mProp );
  prop.mFilledPropMask |= spsTextProp;
  SdGraphNet::getProp( prop );
  }




void SdGraphNetName::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
  }






bool SdGraphNetName::isVisible()
  {
  return mProp.mLayer.isVisible();
  }



SdRect SdGraphNetName::getOverRect() const
  {
  return mOver;
  }



void SdGraphNetName::draw(SdContext *dc)
  {
  dc->text( mOrigin, mOver, getNetName(), mProp );
  }





int SdGraphNetName::behindCursor(SdPoint p)
  {
  if( mProp.mLayer.isEdited() )
    if( mOver.isPointInside(p) ) return getSelector() ? ELEM_SEL : ELEM_UNSEL;
  return 0;
  }




void SdGraphNetName::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() && mOver.isPointInside(p) )
    selector->insert( this );
  }




void SdGraphNetName::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() && r.isAccross( mOver ) )
    selector->insert( this );
  }




