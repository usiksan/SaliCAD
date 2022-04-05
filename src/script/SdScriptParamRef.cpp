#include "SdScriptParamRef.h"

SdScriptParamRef::SdScriptParamRef()
  {

  }

QString SdScriptParamRef::valueGet() const
  {
  if( mRef == nullptr )
    return mValue;

  }

void SdScriptParamRef::assign(SdObject *ref, QString param)
  {
  mRef = ref;
  mParam = param;
  }

void SdScriptParamRef::jsonWrite(SvJsonWriter &js) const
  {
  SdObject::writePtr( mRef, QStringLiteral("ref"), js.ref() );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }

void SdScriptParamRef::jsonRead(SvJsonReaderExt<SdObjectMap> &js)
  {
  mRef = SdObject::readPtr( js.property(), js.object() );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }


