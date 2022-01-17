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

  Special variable. When we assign to it then 2d elements appended to the PartItem.
*/
#ifndef SDSCRIPTPARTGRAPH_H
#define SDSCRIPTPARTGRAPH_H

#include "SdScriptValueVariable.h"
#include "master/SdMasterPart.h"

class SdPItemPart;

class SdScriptPartGraph : public SdScriptValueVariable
  {
    SdMasterPart mMasterPart; //!< Intermediate object which contains defaults for 2d graphics attributes
  public:
    SdScriptPartGraph( SdPItemPart *part );

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type() const override { return SD_SCRIPT_TYPE_GRAPH; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdScriptValuePtr src) override;
  };

#endif // SDSCRIPTPARTGRAPH_H
