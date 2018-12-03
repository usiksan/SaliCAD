/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description

*/
#include "SdConverterOffset.h"


SdConverterOffset::SdConverterOffset(SdPoint offset) :
  mOffset(offset)
  {

  }


QTransform SdConverterOffset::getMatrix()
  {
  return QTransform::fromTranslate( mOffset.x(), mOffset.y() );
  }

