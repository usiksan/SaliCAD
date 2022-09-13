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

  Special variable. When we assign to it then model appended to the PartItem.
*/
#include "SdScriptPartModel.h"
#include "objects/SdPItemPart.h"
#include "objects/Sd3dGraphModel.h"

#include <QDebug>

SdScriptPartModel::SdScriptPartModel(Sd3drModel *model) :
  SdScriptValueVariable(),
  mModel(model)
  {

  }



//!
//! \brief assign Assignment value to variable
//! \param src    source of value
//!
void SdScriptPartModel::assign(SdScriptValuePtr src)
  {
  mModel->instanceAppend( src->toModel() );
  }
