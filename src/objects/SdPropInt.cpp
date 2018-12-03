/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Int property
*/
#include "SdPropInt.h"
#include "SdUtil.h"

QString SdPropInt::log2Phis(double ppm) const
  {
  return mValue < 0 ? QString() : SdUtil::log2physStr(mValue,ppm);
  }



void SdPropInt::setFromPhis(const QString src, double ppm)
  {
  mValue = SdUtil::phys2log(src,ppm);
  }
