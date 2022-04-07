#include "SdGraphScript.h"
#include "SdContext.h"
#include "SdSelector.h"

SdGraphScript::SdGraphScript()
  {

  }



SdGraphScript::SdGraphScript(const QString &script, SdPoint org, const SdPropText &prp) :
  SdGraphParam(),
  mScript(script),
  mOrigin(org),
  mProp(prp)
  {

  }



void SdGraphScript::scriptSet(const QString &scr, SdUndo *undo)
  {
  if( undo != nullptr )
    undo->string2( &mScript, nullptr );

  mScript = scr;
  parse();
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
  SdJsonWriter js( obj );
  js.jsonValue<SdPropText>( "prop", mProp );
  js.jsonString( "script", mScript );
  js.jsonMap( "ref", mRefMap );
  js.jsonPoint( "origin", mOrigin );
  js.jsonObject<SdRect>( "over", mOverRect );
  }




void SdGraphScript::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mProp.read( QStringLiteral("prop"), obj );
  SdJsonReader js( obj, map );
  js.jsonValue<SdPropText>( "prop", mProp );
  js.jsonString( "script", mScript );
  js.jsonMap( "ref", mRefMap );
  js.jsonPoint( "origin", mOrigin );
  js.jsonObject<SdRect>( "over", mOverRect );
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
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && mOverRect.isPointInside(p) ) {
      selector->insert( this );
      }
    }
  }





void SdGraphScript::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && r.isAccross( mOverRect ) ) {
      selector->insert( this );
      }
    }
  }






void SdGraphScript::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
  }



bool SdGraphScript::isVisible()
  {
  return mProp.mLayer.isVisible();
  }



SdRect SdGraphScript::getOverRect() const
  {
  return mOverRect;
  }



void SdGraphScript::draw(SdContext *dc)
  {
  dc->text( mOrigin, mOverRect, mScript, mProp );
  }



int SdGraphScript::behindCursor(SdPoint p)
  {
  if( mProp.mLayer.isEdited() ) {
    if( mOverRect.isPointInside(p) )
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
    }
  return 0;
  }



int SdGraphScript::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  if( mProp.mLayer.isEdited() && mOverRect.isPointInside(p) ) {
    org  = mOrigin;
    dest = mScript;
    prop = mProp;
    return 1;
    }
  return 0;
  }
