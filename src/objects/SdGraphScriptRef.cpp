#include "SdGraphScriptRef.h"
#include "SdContainer.h"

SdGraphScriptRef::SdGraphScriptRef() :
  mRefOwner(nullptr),
  mRef(nullptr)
  {

  }



SdGraphScriptRef::SdGraphScriptRef(const QString &name) :
  mRefOwner(nullptr),
  mRef(nullptr),
  mName(name)
  {

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
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }


void SdGraphScriptRef::jsonRead(SdJsonReader &js)
  {
  mRefOwner = SdObject::readPtrClass<SdContainer>( QStringLiteral("owner"), js.property(), js.object() );
  mRef = SdObject::readPtrClass<SdGraphParam>( QStringLiteral("ref"), js.property(), js.object() );
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
