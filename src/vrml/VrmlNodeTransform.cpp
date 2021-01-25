#include "VrmlNodeTransform.h"
#include "SdScanerVrml.h"

VrmlNodeTransform::VrmlNodeTransform()
  {

  }

VrmlNodeTransform *VrmlNodeTransform::parse2Transform(SdScanerVrml *scaner)
  {
  if( scaner->matchToken('{') ) {
    VrmlNodeTransform *transform = new VrmlNodeTransform();
    while( !scaner->matchToken('}') ) {
      if( scaner->isEndOfScan() ) {
        delete transform;
        scaner->error( QStringLiteral("Uncompleted transform") );
        return nullptr;
        }
      if( scaner->matchToken('n') ) {
        if( !parse2GroupComponents( scaner, transform ) ) {
          delete group;
          scaner->error( QStringLiteral("Undefined group node %1").arg(scaner->tokenValue()) );
          return nullptr;
          }
        }
      else {
        delete transform;
        scaner->error( QStringLiteral("Undefined token") );
        return nullptr;
        }
      }
    //Transform completed
    return transform;
    }
  return nullptr;
  }


VrmlNode *VrmlNodeTransform::copy() const
    {
    }
