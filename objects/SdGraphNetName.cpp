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





void SdGraphNetName::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  const SdGraphNetName *wname = dynamic_cast<const SdGraphNetName*>(src);
  Q_ASSERT(wname != nullptr);
  mOrigin = wname->mOrigin;
  mProp   = wname->mProp;
  mOver   = wname->mOver;
  }




void SdGraphNetName::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject(obj);
  mOrigin.write( QStringLiteral("Org"), obj );
  mProp.write( QStringLiteral("Prop"), obj );
  mOver.write( QStringLiteral("Over"), obj );
  }




void SdGraphNetName::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mOrigin.read( QStringLiteral("Org"), obj );
  mProp.read( QStringLiteral("Prop"), obj );
  mOver.read( QStringLiteral("Over"), obj );
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
    if( mOver.isPointInside(p) ) return getSelector() ? SEL_ELEM : UNSEL_ELEM;
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




