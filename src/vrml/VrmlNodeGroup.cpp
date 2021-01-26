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



bool VrmlNodeGroup::parse2GroupComponents(SdScanerVrml *scaner, const QString nodeType )
  {
  if( nodeType == QStringLiteral("children") )
    parseChildren( scaner );
  else if( nodeType == QStringLiteral("bboxCenter") )
    mBoxCenter.parse( scaner );
  else if( nodeType == QStringLiteral("bboxSize") )
    mBoxSize.parse( scaner );
  else return false;
  return true;
  }





void VrmlNodeGroup::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeed( '{', QStringLiteral("No group") ) )
    return;

  while( !scaner->matchToken('}') ) {
    if( scaner->isEndOfScan() ) {
      scaner->error( QStringLiteral("Uncompleted group") );
      return;
      }
    if( scaner->isError() )
      return;
    QString nodeType;
    if( !scaner->tokenNeedValue( 'n', nodeType, QStringLiteral("Need group node") ) )
      return;
    if( !parse2GroupComponents( scaner, nodeType ) ) {
      scaner->error( QStringLiteral("Undefined group node %1").arg(nodeType) );
      return;
      }
    }
  }
