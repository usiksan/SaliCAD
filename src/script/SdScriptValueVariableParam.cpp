#include "SdScriptValueVariableParam.h"
#include "SdScriptValueFunParam.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdGraphParam.h"
#include "windows/SdDRowValue.h"

SdScriptValueVariableParam::SdScriptValueVariableParam(SdPItemSheet *sheet) :
  mSheet(sheet),
  mComp(nullptr),
  mFunParam(nullptr)
  {

  }




//!
//! \brief toFloat Convert object ot float value
//! \return        float value
//!
float SdScriptValueVariableParam::toFloat() const
  {
  //We retrive value from visual table
  if( mFunParam == nullptr )
    return 0.0;
  return SdDRowValue::phisToDouble( mFunParam->toString() );
  }






void SdScriptValueVariableParam::assign(SdScriptValuePtr src)
  {
  if( src->type() == SD_SCRIPT_TYPE_REF ) {
    //Fill table row
    if( mFunParam == nullptr ) {

      //Retrive value from sheet param
      mFunParam = dynamic_cast<SdScriptValueFunParam*>(src);
      if( mFunParam != nullptr ) {
        //Get textual representation of param
        QString paramValue;

        //We scan all sheet object and test symImp and netParam
        mSheet->forEachConst( dctSymImp | dctParam, [&] ( SdObject *obj ) ->bool {
          SdPtrConst<SdGraphSymImp> sym(obj);
          SdPtrConst<SdGraphParam> sheetParam(obj);
          if( sym.isValid() && sym->ident() == mFunParam->componentId() ) {
            //Found symbol with needed ref id
            paramValue = sym->paramGet( mFunParam->paramId() );
            mComp = obj;
            return true;
            }
          else if( sheetParam.isValid() && sheetParam->paramName() == mFunParam->paramId() ) {
            paramValue = sheetParam->paramValueGet();
            mComp = obj;
            return true;
            }
          return false;
          } );

        if( paramValue.isEmpty() )
          //Parametr not found
          mFunParam->valueSet( QStringLiteral("0.0") );
        else
          //Set param as default
          mFunParam->valueSet( paramValue );
        }
      else return;
      }
    }
  else {
    if( mFunParam != nullptr ) {
      //Convert to string value with row ajust
      QString value = SdDRowValue::doubleToPhis( src->toFloat(), mFunParam->dimensions(), mFunParam->row() );

      //Update in table
      mFunParam->valueSet( value );

      //Update in schematic
      if( mComp != nullptr ) {
        SdPtr<SdGraphSymImp> sym(mComp);
        SdPtr<SdGraphParam> sheetParam(mComp);
        if( sym.isValid() ) {
          //Update component
          sym->paramSet( mFunParam->paramId(), value );
          }
        else if( sheetParam.isValid() ) {
          sheetParam->paramValueSet( value );
          }
        }
      }
    }
  }



