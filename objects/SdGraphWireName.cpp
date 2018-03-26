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
#include "SdGraphWireName.h"
#include "SdContainerSheetNet.h"
#include "SdPItemSheet.h"
#include "SdContext.h"
#include "SdEnvir.h"
#include "SdSelector.h"

SdGraphWireName::SdGraphWireName() :
  SdGraph()
  {

  }

SdGraphWireName::SdGraphWireName(SdPoint org, const SdPropText &prp) :
  SdGraph(),
  mOrigin(org),
  mProp(prp)
  {

  }



SdContainerSheetNet *SdGraphWireName::getNet() const
  {
  return dynamic_cast<SdContainerSheetNet*>( getParent() );
  }




//Get owner net name
QString SdGraphWireName::getNetName() const
  {
  return getNet()->getNetName();
  }


void SdGraphWireName::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  const SdGraphWireName *wname = dynamic_cast<const SdGraphWireName*>(src);
  Q_ASSERT(wname != nullptr);
  mOrigin = wname->mOrigin;
  mProp   = wname->mProp;
  mOver   = wname->mOver;
  }




void SdGraphWireName::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject(obj);
  mOrigin.write( QStringLiteral("Org"), obj );
  mProp.write( QStringLiteral("Prop"), obj );
  mOver.write( QStringLiteral("Over"), obj );
  }




void SdGraphWireName::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mOrigin.read( QStringLiteral("Org"), obj );
  mProp.read( QStringLiteral("Prop"), obj );
  mOver.read( QStringLiteral("Over"), obj );
  }




void SdGraphWireName::saveState(SdUndo *undo)
  {
  undo->propTextAndText( &mProp, &mOrigin, &mOver, nullptr );
  }




void SdGraphWireName::move(SdPoint offset)
  {
  mOrigin.move( offset );
  }




void SdGraphWireName::rotate(SdPoint center, SdAngle angle)
  {
  mOrigin.rotate( center, angle );
  mProp.mDir += angle;
  }




void SdGraphWireName::mirror(SdPoint a, SdPoint b)
  {
  mOrigin.mirror( a, b );
  }




void SdGraphWireName::setProp(SdPropSelected &prop)
  {
  mProp = prop.mTextProp;
  //TODO D003 when change name of wire
  }




void SdGraphWireName::getProp(SdPropSelected &prop)
  {
  prop.mTextProp.append( mProp );
  prop.mFilledPropMask |= spsTextProp;
  prop.mWireName.append( getNetName() );
  }




void SdGraphWireName::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
  }






bool SdGraphWireName::isVisible()
  {
  return mProp.mLayer.isVisible();
  }



SdRect SdGraphWireName::getOverRect() const
  {
  return mOver;
  }



void SdGraphWireName::draw(SdContext *dc)
  {
  dc->text( mOrigin, mOver, getNetName(), mProp );
  }





int SdGraphWireName::behindCursor(SdPoint p)
  {
  if( mProp.mLayer.isEdited() )
    if( mOver.isPointInside(p) ) return getSelector() ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }




void SdGraphWireName::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() && mOver.isPointInside(p) )
    selector->insert( this );
  }




void SdGraphWireName::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() && r.isAccross( mOver ) )
    selector->insert( this );
  }




