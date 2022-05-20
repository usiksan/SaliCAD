/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Referenced variable. It does not contain the value itself. Instead, it contains a reference to an
  external parameter, which contains the value. This parameter can be a line in the visual table or
  a parameter of some component, depending on the link. All links are located in the link map, the
  pointer to which contains the variable. The key in the map is the variable name.
*/
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




//!
//! \brief toFloat Convert object ot float value
//! \return        float value
//!
//! We extract value from external param referenced by this variable
float SdScriptValueVariableRef::toFloat() const
  {
  //We retrive value from schematic
  return SdDRowValue::phisToDouble( mRef->varGet( mName )  );
  }




//!
//! \brief assign Assign new value to this variable. We take source value and write it to reference
//! \param src    Source value
//!
void SdScriptValueVariableRef::assign(SdScriptValuePtr src)
  {
  if( src->type() == SD_SCRIPT_TYPE_FLOAT )
    mRef->varSet( mName, SdDRowValue::doubleToPhis( src->toFloat(), mDimension, mRow )  );
  }
