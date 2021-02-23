/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Creates copy of source model translated with vector
*/
#ifndef SDM3DFUNMODELTRANSLATE_H
#define SDM3DFUNMODELTRANSLATE_H

#include "SdM3dFunction.h"

class SdM3dFunModelTranslate : public SdM3dFunction
  {
  public:
    SdM3dFunModelTranslate();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    //!
    //! \brief modelTranslate Creates copy of source model translated with vector
    //! \param model          Source model
    //! \param offset         Offset vector
    //! \return               Translated model
    //!
    static  SdM3dModel modelTranslate( SdM3dModel model, QVector3D offset );
  };

#endif // SDM3DFUNMODELTRANSLATE_H
