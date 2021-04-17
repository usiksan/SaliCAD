#include "VrmlNode1MaterialBinding.h"
#include "SdScanerVrml.h"

VrmlNode1MaterialBinding::VrmlNode1MaterialBinding() :
  VrmlNode1(),
  mBinding(MATERIAL_BINDING_DEFAULT)
  {

  }


bool VrmlNode1MaterialBinding::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("value") ) {
    if( scaner->token() == 'n' ) {
      if( scaner->tokenValue() == QStringLiteral("DEFAULT") )
        mBinding = MATERIAL_BINDING_DEFAULT;

      else if( scaner->tokenValue() == QStringLiteral("OVERALL") )
        mBinding = MATERIAL_BINDING_OVERALL;

      else if( scaner->tokenValue() == QStringLiteral("PER_PART") )
        mBinding = MATERIAL_BINDING_PER_PART;

      else if( scaner->tokenValue() == QStringLiteral("PER_PART_INDEXED") )
        mBinding = MATERIAL_BINDING_PER_PART_INDEXED;

      else if( scaner->tokenValue() == QStringLiteral("PER_FACE") )
        mBinding = MATERIAL_BINDING_PER_FACE;

      else if( scaner->tokenValue() == QStringLiteral("PER_FACE_INDEXED") )
        mBinding = MATERIAL_BINDING_PER_FACE_INDEXED;

      else if( scaner->tokenValue() == QStringLiteral("PER_VERTEX") )
        mBinding = MATERIAL_BINDING_PER_VERTEX;

      else if( scaner->tokenValue() == QStringLiteral("PER_VERTEX_INDEXED") )
        mBinding = MATERIAL_BINDING_PER_VERTEX_INDEXED;

      else mBinding = MATERIAL_BINDING_DEFAULT;

      scaner->tokenNext();
      }
    }

  else return false;

  return true;
  }
