#include "SdScanerKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdProject.h"

#include <QObject>

void kicadFootprint(SdScanerKiCad *scaner, SdProject *project )
  {
  QString name;
  if( !scaner->tokenNeedValue( 's', name, QObject::tr("Need footprint name") ) ) return;

  SdPItemPart *part = new SdPItemPart();
  project->insertChild( part, nullptr );

  part->setTitle( name, QObject::tr("Set part title") );

  static QMap<QString,std::function<void( SdScanerKiCad*, SdPItemPart* )> >
      tokenMap( { //{ QString("fp_text"), kicadTextPart },
                  { QString("fp_line"), kicadLinePart },
                  { QString("fp_circle"), kicadCirclePart }
                } );

  scaner->parse( tokenMap, part );
  }
