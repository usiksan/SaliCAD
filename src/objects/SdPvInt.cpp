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
#include "SdPvInt.h"
#include "SdUtil.h"



//!
//! \brief log2Phis Convert logical value to physical string representation
//! \param ppm      Pixels per millimeter conversion factor
//! \return         Physical value as formatted string (e.g., "10.5 mm")
//!
QString SdPvInt::log2Phis(double ppm) const
  {
  return SdUtil::log2physStr(mValue,ppm);
  }




//!
//! \brief setFromPhis Set logical value from physical string representation
//! \param src        Source physical string (e.g., "10.5 mm")
//! \param ppm        Pixels per millimeter conversion factor
//!
void SdPvInt::setFromPhis(const QString &src, double ppm)
  {
  mValue = SdUtil::phys2log(src,ppm);
  }
