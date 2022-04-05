#include "SdScriptParamRef.h"
#include "objects/SdContainer.h"

SdScriptParamRef::SdScriptParamRef()
  {

  }

QString SdScriptParamRef::valueGet()
  {
  if( checkRef() )
    return mRef->paramGet( mParam );
  return mValue;
  }



void SdScriptParamRef::valueSet(const QString &v)
  {
  if( checkRef() )
    mRef->paramSet( mParam, v );
  else
    mValue = v;
  }


void SdScriptParamRef::assign(SdContainer *refOwner, SdGraphParam *ref, QString param)
  {
  mRefOwner = refOwner;
  mRef = ref;
  mParam = param;
  checkRef();
  }

void SdScriptParamRef::jsonWrite(SvJsonWriter &js) const
  {
  SdObject::writePtr( mRefOwner, QStringLiteral("owner"), js.ref() );
  SdObject::writePtr( mRef, QStringLiteral("ref"), js.ref() );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }



void SdScriptParamRef::jsonRead(SvJsonReaderExt<SdObjectMap> &js)
  {
  mRefOwner = SdObject::readPtrClass<SdContainer>( QStringLiteral("owner"), js.property(), js.object() );
  mRef = SdObject::readPtrClass<SdGraphParam>( QStringLiteral("ref"), js.property(), js.object() );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }



bool SdScriptParamRef::checkRef()
  {
  if( mRef == nullptr )
    return false;
  if( mRefOwner->isChild(mRef) )
    return true;
  mRefOwner = nullptr;
  mRef = nullptr;
  return false;
  }


