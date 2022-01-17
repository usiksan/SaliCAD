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

  Perform scan source code of script and retrive single lexemma
*/
#ifndef SDSCRIPTSCANER_H
#define SDSCRIPTSCANER_H

#include "import/SdScanerMultyline.h"

class SdScriptScaner : public SdScanerMultyline
  {
  public:
    SdScriptScaner();

    // SdScaner interface
  public:
    virtual void tokenNext() override;
  };

#endif // SDSCRIPTSCANER_H
