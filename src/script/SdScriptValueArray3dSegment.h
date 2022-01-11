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

  Array of 3d segments.
*/
#ifndef SDSCRIPTVALUEARRAY3DSEGMENT_H
#define SDSCRIPTVALUEARRAY3DSEGMENT_H

#include "SdScriptValueArray.h"

class SdScriptValueArray3dSegment : public SdScriptValueArray
  {
  public:
    SdScriptValueArray3dSegment() {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char              type() const override { return SD_SCRIPT_TYPE_PATH; }

    //!
    //! \brief toPath Convert object to path of 3d segments which may be separated
    //! \return       Path of 3d segments which may be separated
    //!
    virtual SdScriptVal3dPath toPath() const override;

    // SdM3dArray interface
  public:
    //!
    //! \brief typeOfElement Return type of element of array
    //! \return              Type of element of array
    //!
    virtual char              typeOfElement() const override  { return SD_SCRIPT_TYPE_SEGMENT; }
  };

#endif // SDSCRIPTVALUEARRAY3DSEGMENT_H
