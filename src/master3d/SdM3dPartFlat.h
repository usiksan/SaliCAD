/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Special variable. When we assign to it then 2d elements appended to the PartItem.
*/
#ifndef SDM3DPARTFLAT_H
#define SDM3DPARTFLAT_H

#include "SdM3dVariable.h"
#include "master/SdMasterPart.h"

class SdPItemPart;

class SdM3dPartFlat : public SdM3dVariable
  {
    SdMasterPart mMasterPart;
  public:
    SdM3dPartFlat( SdPItemPart *part );

    // SdM3dValue interface
  public:
    virtual char type() const override { return SDM3D_TYPE_FLAT; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override;
  };

#endif // SDM3DPARTFLAT_H
