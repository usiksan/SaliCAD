#include "SdGraphScript.h"

SdGraphScript::SdGraphScript()
  {

  }

void SdGraphScript::parse()
  {

  }

QString SdGraphScript::getType() const
  {
  return QStringLiteral(SD_TYPE_SCRIPT);
  }




SdClass SdGraphScript::getClass() const
  {
  return dctGraphScript;
  }



void SdGraphScript::cloneFrom(const SdObject *src)
  {
  SdGraphParam::cloneFrom( src );
  SdPtrConst<SdGraphScript> ps(src);
  Q_ASSERT_X( ps.isValid(), "SdGraphScript::cloneForm", "Cloned not script" );
  mScript = ps->mScript;
  mOrigin = ps->mOrigin;
  mProp   = ps->mProp;
  parse();
  }



void SdGraphScript::writeObject(QJsonObject &obj) const
  {
  SdGraphParam::writeObject( obj );
  mProp.write( QStringLiteral("prop"), obj );
  obj.insert( QStringLiteral("text"), mScript );
  mOverRect.write( QStringLiteral("over"), obj );
  mOrigin.write( QStringLiteral("Origin"), obj );
  }




void SdGraphScript::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mProp.read( QStringLiteral("prop"), obj );
  mScript = obj.value( QStringLiteral("text") ).toString();
  mOverRect.read( QStringLiteral("over"), obj );
  mOrigin.read( QStringLiteral("Origin"), obj );
  }



void SdGraphScript::saveState(SdUndo *undo)
  {
  if( undo )
    undo->propTextAndText( &mProp, &mOrigin, &mOverRect, &mScript );
  }




void SdGraphScript::move(SdPoint offset)
  {
  mOrigin.move( offset );
  mOverRect.move( offset );
  }




void SdGraphScript::setProp(SdPropSelected &prop)
  {
  mProp = prop.mTextProp;
  }



void SdGraphScript::getProp(SdPropSelected &prop)
  {
  prop.mTextProp.append( mProp );
  prop.mFilledPropMask |= spsTextProp;
  }




void SdGraphScript::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  }



void SdGraphScript::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  }



void SdGraphScript::selectByRect(const SdRect &r, SdSelector *selector)
  {
  }



void SdGraphScript::select(SdSelector *selector)
  {
  }

void SdGraphScript::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
  }

bool SdGraphScript::isVisible()
  {
  }

SdRect SdGraphScript::getOverRect() const
  {
  }

void SdGraphScript::draw(SdContext *dc)
  {
  }

int SdGraphScript::behindCursor(SdPoint p)
  {
  }

int SdGraphScript::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  }
