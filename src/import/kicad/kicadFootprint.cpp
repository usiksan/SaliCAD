#include "SdScanerKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdProject.h"

#include <QObject>

void kicadFootprint(SdScanerKiCad *scaner, SdProject *project )
  {
  QString name;
  if( !scaner->tokenNeedValue( 's', name, QObject::tr("Need footprint name") ) ) return;

  SdPItemPart *part = new SdPItemPart();
  project->insertChild( part, project->getUndo() );

  part->setTitle( name, QObject::tr("Set part title") );


  }
