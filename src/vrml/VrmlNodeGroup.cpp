#include "VrmlNodeGroup.h"
#include "SdScanerVrml.h"

VrmlNodeGroup::VrmlNodeGroup()
  {

  }

VrmlNodeGroup::VrmlNodeGroup(const VrmlNodeGroup *group) :
  VrmlNodeCompound( group )
  {
  mBoxCenter = group->mBoxCenter;
  mBoxSize   = group->mBoxSize;
  }



bool VrmlNodeGroup::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("children") )
    parseChildren( scaner );
  else if( fieldType == QStringLiteral("bboxCenter") )
    mBoxCenter.parse( scaner );
  else if( fieldType == QStringLiteral("bboxSize") )
    mBoxSize.parse( scaner );
  else return false;
  return true;
  }
