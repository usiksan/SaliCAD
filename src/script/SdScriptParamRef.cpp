#include "SdScriptParamRef.h"

SdScriptParamRef::SdScriptParamRef()
  {

  }

void SdScriptParamRef::assign(SdObject *ref, QString param)
  {

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


