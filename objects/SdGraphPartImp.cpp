/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphPartImp.h"
#include "SdPItemPlate.h"

SdGraphPartImp::SdGraphPartImp()
  {

  }

void SdGraphPartImp::pinConnectionSet(int pinIndex, const QString wireName, bool com)
  {
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  mPins[pinIndex].setConnection( wireName, com );
  }

void SdGraphPartImp::pinConnectionSet(const QString pinNumber, const QString wireName, bool com)
  {

  }




SdPItemPlate *SdGraphPartImp::getPlate() const
  {
  return dynamic_cast<SdPItemPlate*>( getParent() );
  }
