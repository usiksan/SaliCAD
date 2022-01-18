#include "SdScriptValueVariableParam.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdGraphNetParam.h"
#include "windows/SdDRowValue.h"

SdScriptValueVariableParam::SdScriptValueVariableParam(SdPItemSheet *sheet, const QString &refId, const QString &param, const QString &dimension) :
  mSheet(sheet),
  mComp(nullptr),
  mParam(param),
  mDimension(dimension)
  {
  //Retrive value from sheet param

  //Get textual representation of param
  QString paramValue;

  //We scan all sheet object and test symImp and netParam
  mSheet->forEachConst( dctSymImp | dctNetParam, [&] ( SdObject *obj ) ->bool {
    SdPtrConst<SdGraphSymImp> sym(obj);
    SdPtrConst<SdGraphNetParam> netParam(obj);
    if( sym.isValid() && sym->ident() == refId ) {
      //Found symbol with needed ref id
      paramValue = sym->paramGet( mParam );
      mComp = obj;
      return true;
      }
    else if( netParam.isValid() && netParam->getNetName() == refId && netParam->paramName() == mParam ) {
      paramValue = netParam->paramValueGet();
      mComp = obj;
      return true;
      }
    return false;
    } );

  if( paramValue.isEmpty() )
    //Parametr not found
    mValue = 0.0;
  else
    //Parse param
    mValue = SdDRowValue::phisToDouble( paramValue );
  }




void SdScriptValueVariableParam::assign(SdScriptValuePtr src)
  {
  //Convert to string value with row ajust
  QString value = SdDRowValue::doubleToPhis( src->toFloat(), mDimension, mRow );
  //At now mValue contains row ajusted value
  mValue = SdDRowValue::phisToDouble( value );

  //Update component
  if( mComp != nullptr ) {
    SdPtr<SdGraphSymImp> sym(mComp);
    SdPtr<SdGraphNetParam> netParam(mComp);
    if( sym.isValid() ) {
      sym->paramSet( mParam, value );
      }
    else if( netParam.isValid() ) {
      netParam->paramValueSet( value );
      }
    }
  }



