#include "SdGraphNetParam.h"
#include "SvJsonIO.h"

SdGraphNetParam::SdGraphNetParam() :
  SdGraphNet()
  {

  }

SdGraphNetParam::SdGraphNetParam(SdPoint org, const QString &netName, const QString &param, const SdPropText &prp) :
  SdGraphNet(netName),
  mOrigin(org),
  mProp(prp)
  {
  paramParse(param);
  }




QString SdGraphNetParam::paramName() const
  {
  QStringList lst( mParam.split( QChar('=') ) );

  if( lst.count() == 2 )
    return lst.at(0).simplified();

  return QString{};
  }




QString SdGraphNetParam::paramValueGet() const
  {
  QStringList lst( mParam.split( QChar('=') ) );

  if( lst.count() == 2 )
    return lst.at(1).simplified();

  return QString{};
  }



void SdGraphNetParam::paramValueSet(const QString &value)
  {
  paramParse( paramName() + QStringLiteral(" = ") + value );
  }




void SdGraphNetParam::paramParse(const QString &param)
  {
  QStringList lst( param.split( QChar('=') ) );

  if( lst.count() == 2 )
    mParam = param;

  if( mParam.isEmpty() )
    mParam = QStringLiteral("none : 0");
  }




void SdGraphNetParam::cloneFrom(const SdObject *src)
  {
  SdGraphNet::cloneFrom( src );
  SdPtrConst<SdGraphNetParam> srcNetParam(src);
  Q_ASSERT( srcNetParam.isValid() );
  mOrigin = srcNetParam->mOrigin;
  mParam  = srcNetParam->mParam;
  mProp   = srcNetParam->mProp;
  }





void SdGraphNetParam::writeObject(QJsonObject &obj) const
  {
  SdGraphNet::writeObject(obj);
  mProp.write( QStringLiteral("tp."), obj );
  SvJsonWriter js(obj);
  js.jsonString( "param", mParam );
  js.jsonObject( "org", mOrigin );
  }




void SdGraphNetParam::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphNet::readObject( map, obj );
  mProp.read( QStringLiteral("tp."), obj );
  SvJsonReader js(obj);
  js.jsonString( "param", mParam );
  js.jsonObject( "org", mOrigin );
  }




void SdGraphNetParam::saveState(SdUndo *undo)
  {
  SdGraphNet::saveState(undo);
  undo->propTextAndText( &mProp, &mOrigin, &mOver, &mParam );
  }




void SdGraphNetParam::move(SdPoint offset)
  {
  mOrigin.move( offset );
  }




void SdGraphNetParam::rotate(SdPoint center, SdPropAngle angle)
  {
  mOrigin.rotate( center, angle );
  mProp.mDir += angle;
  }




void SdGraphNetParam::mirror(SdPoint a, SdPoint b)
  {
  mOrigin.mirror( a, b );
  }




void SdGraphNetParam::setProp(SdPropSelected &prop)
  {
  mProp = prop.mTextProp;
  SdGraphNet::setProp( prop );
  }




void SdGraphNetParam::getProp(SdPropSelected &prop)
  {
  prop.mTextProp.append( mProp );
  prop.mFilledPropMask |= spsTextProp;
  SdGraphNet::getProp( prop );
  }




void SdGraphNetParam::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
  }




void SdGraphNetParam::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(index)
  Q_UNUSED(parent)
  paramParse( sour );
  mProp   = prop;
  }




void SdGraphNetParam::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() && mOver.isPointInside(p) )
    selector->insert( this );
  }




void SdGraphNetParam::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() && r.isAccross( mOver ) )
    selector->insert( this );
  }




bool SdGraphNetParam::isVisible()
  {
  return mProp.mLayer.isVisible();
  }




int SdGraphNetParam::behindCursor(SdPoint p)
  {
  if( mProp.mLayer.isEdited() )
    if( mOver.isPointInside(p) ) return getSelector() ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }




int SdGraphNetParam::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  if( mProp.mLayer.isEdited() && mOver.isPointInside(p) ) {
    org  = mOrigin;
    dest = mParam;
    prop = mProp;
    return 1;
    }
  return 0;
  }


