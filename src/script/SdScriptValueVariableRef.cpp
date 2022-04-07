#include "SdScriptValueVariableRef.h"
#include "windows/SdDRowValue.h"

SdScriptValueVariableRef::SdScriptValueVariableRef(SdScriptRefMap *ref, const QString &name, const QString &dimension, const QString &row ) :
  SdScriptValueVariable(),
  mRef(ref),
  mName(name),
  mDimension(dimension),
  mRow(row)
  {
  mRef->varInit( mName );
  }



float SdScriptValueVariableRef::toFloat() const
  {
  //We retrive value from schematic
  return SdDRowValue::phisToDouble( mRef->varGet( mName )  );
  }




void SdScriptValueVariableRef::assign(SdScriptValuePtr src)
  {
  if( src->type() == SD_SCRIPT_TYPE_FLOAT )
    mRef->varSet( mName, SdDRowValue::doubleToPhis( src->toFloat(), mDimension, mRow )  );
  }
