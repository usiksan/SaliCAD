/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDPASCADIMPORT_H
#define SDPASCADIMPORT_H

#include "SdProject.h"

class SdPasCadImport
  {
    SdProject *mProject;
  public:
    SdPasCadImport( SdProject *prj );

    //Import entry point
    void import( const QString fname );

  private:
  };

#endif // SDPASCADIMPORT_H
