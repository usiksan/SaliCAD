#include "VrmlNodeGroup.h"
#include "SdScanerVrml.h"

VrmlNodeGroup::VrmlNodeGroup()
  {

  }

VrmlNodeGroup *VrmlNodeGroup::parse2Group(SdScanerVrml *scaner)
  {
  if( scaner->matchToken('{') ) {
    VrmlNodeGroup *group = new VrmlNodeGroup();
    while( !scaner->matchToken('}') ) {
      if( scaner->isEndOfScan() ) {
        delete group;
        scaner->error( QStringLiteral("Uncompleted group") );
        return nullptr;
        }
      if( scaner->matchToken('n') ) {
        if( !parse2GroupComponents( scaner, group ) ) {
          delete group;
          scaner->error( QStringLiteral("Undefined group node %1").arg(scaner->tokenValue()) );
          return nullptr;
          }
        }
      else {
        delete group;
        scaner->error( QStringLiteral("Undefined token") );
        return nullptr;
        }
      }
    //Group completed
    return group;
    }
  return nullptr;
  }

bool VrmlNodeGroup::parse2GroupComponents(SdScanerVrml *scaner, VrmlNodeGroup *group)
  {
  if( scaner->matchTokenValue( QStringLiteral("children") ) )
    group->parseChildren( scaner );
  else if( scaner->matchTokenValue( QStringLiteral("bboxCenter") ) )
    group->mBoxCenter.parse( scaner );
  else if( scaner->matchTokenValue( QStringLiteral("bboxSize") ) )
    group->mBoxSize.parse( scaner );
  else return false;
  return true;
  }

void VrmlNodeGroup::cloneNodeGroup(VrmlNodeGroup *destNode) const
  {
  cloneNodeCompound( destNode );
  destNode->mBoxCenter = mBoxCenter;
  destNode->mBoxSize   = mBoxSize;
  }


VrmlNode *VrmlNodeGroup::copy() const
  {
  VrmlNodeGroup *group = new VrmlNodeGroup();
  cloneNodeGroup( group );
  return group;
  }

