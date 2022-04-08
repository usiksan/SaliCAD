#include "SdGraphScriptRef.h"
#include "SdContainer.h"
#include "SdContext.h"
#include "SdGraphSymImp.h"

SdGraphScriptRef::SdGraphScriptRef() :
  mRefOwner(nullptr),
  mRef(nullptr)
  {

  }



SdGraphScriptRef::SdGraphScriptRef(const QString &nm) :
  mRefOwner(nullptr),
  mRef(nullptr),
  mName(nm)
  {

  }



QString SdGraphScriptRef::refName() const
  {
  if( checkRef() ) {
    SdPtr<SdGraphSymImp> sym(mRef);
    if( sym.isValid() )
      return sym->ident() + QStringLiteral(".") + mParam;
    return mParam;
    }
  return mName;
  }



void SdGraphScriptRef::draw(SdPoint p, const SdPropText &prop, SdContext *dc, bool drawValue)
  {
  //Build name part
  QString namePart = refName() + QStringLiteral(" = ");
  //Draw name part
  dc->text( p, mOverName, namePart, prop );
  if( drawValue ) {
    //Draw value part
    //Calc value position at right of name part
    p.rx() = mOverName.right();
    mOriginValue = p;
    QString valuePart = valueGet();
    if( valuePart.simplified().isEmpty() )
      valuePart = QStringLiteral("none");
    dc->text( p, mOverValue, valuePart, prop );
    }
  }



QString SdGraphScriptRef::valueGet() const
  {
  if( checkRef() )
    return mRef->paramGet( mParam );
  return mValue;
  }



void SdGraphScriptRef::valueSet(const QString &v)
  {
  if( checkRef() )
    mRef->paramSet( mParam, v );
  else
    mValue = v;
  }



void SdGraphScriptRef::assign(SdContainer *refOwner, SdGraphParam *ref, QString param)
  {
  mRefOwner = refOwner;
  mRef = ref;
  mParam = param;
  checkRef();
  }



void SdGraphScriptRef::jsonWrite(SdJsonWriter &js) const
  {
  SdObject::writePtr( mRefOwner, QStringLiteral("owner"), js.ref() );
  SdObject::writePtr( mRef, QStringLiteral("ref"), js.ref() );
  js.jsonString( "name", mName );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }




void SdGraphScriptRef::jsonRead(SdJsonReader &js)
  {
  mRefOwner = SdObject::readPtrClass<SdContainer>( QStringLiteral("owner"), js.property(), js.object() );
  mRef = SdObject::readPtrClass<SdGraphParam>( QStringLiteral("ref"), js.property(), js.object() );
  js.jsonString( "name", mName );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }




bool SdGraphScriptRef::checkRef() const
  {
  if( mRef == nullptr )
    return false;
  if( mRefOwner->isChild(mRef) )
    return true;
  mRefOwner = nullptr;
  mRef = nullptr;
  return false;
  }
