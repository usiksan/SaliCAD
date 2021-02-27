/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Special variable. When we assign to it then model appended to the PartItem.
*/
#ifndef SDM3DPARTMODEL_H
#define SDM3DPARTMODEL_H

#include "SdM3dVariable.h"

class SdPItemPart;

class SdM3dPartModel : public SdM3dVariable
  {
    SdPItemPart *mPart;
  public:
    SdM3dPartModel( SdPItemPart *part );

    // SdM3dValue interface
  public:
    virtual char type() const override { return SDM3D_TYPE_MODEL; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override;
  };

#endif // SDM3DPARTMODEL_H
