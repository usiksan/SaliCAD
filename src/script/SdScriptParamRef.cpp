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



void SdScriptParamRef::json( SdJsonWriter &js) const
  {
  js.jsonObjectPtr( QStringLiteral("owner"), mRefOwner );
  js.jsonObjectPtr( QStringLiteral("ref"), mRef );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }



void SdScriptParamRef::json(SdJsonReader &js)
  {
  js.jsonObjectPtr( QStringLiteral("owner"), mRefOwner );
  js.jsonObjectPtr( QStringLiteral("ref"), mRef );
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

