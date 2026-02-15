#include "SdKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdGraphPartPin.h"





void kicadPadPart( SdScanerKiCad *scaner, SdPItemPart *part )
  {
  QString padNumber;
  if( !scaner->tokenNeedValue( 's', padNumber, QObject::tr("Need pad number") ) ) return;

  QString padType;
  if( !scaner->tokenNeedValue( 'n', padType, QObject::tr("Need pad type") ) ) return;

  QString padForm;
  if( !scaner->tokenNeedValue( 'n', padForm, QObject::tr("Need pad form") ) ) return;

  SdKiCadPad pad;
  pad.scan( scaner );

  if( padNumber.isEmpty() ) {
    //Fictive pad for silk or others
    //We simple append graphics on layer (all filled)
    //circle rect oval trapezoid roundrect
    }
  else {
    //Pin with (or without) pad
    if( !pad.mPosWithRotation.isValid() || !pad.mSize.isValid() ) return;
    //SdGraphPartPin( SdPoint org, const SdPropPartPin &pinProp, SdPoint numberPos, const SdPropText &numberProp, SdPoint namePos, const SdPropText &nameProp, const QString number );

    }

  //Build pin on component with pad
  // part->insertChild( new SdGraphLinearLine( lineInfo.mA, lineInfo.mB, lineInfo.mProp ), nullptr );
  }
