#include "VrmlNode.h"
#include "SdScanerVrml.h"
#include "VrmlNodeApperance.h"
#include "VrmlNodeColor.h"
#include "VrmlNodeCoordinate.h"
#include "VrmlNodeGroup.h"
#include "VrmlNodeIndexedFaceSet.h"
#include "VrmlNodeMaterial.h"
#include "VrmlNodeNormal.h"
#include "VrmlNodeShape.h"
#include "VrmlNodeTransform.h"


VrmlNode::VrmlNode()
  {

  }

void VrmlNode::parse2Declaration(SdScanerVrml *scaner, VrmlNodePtrList *list)
  {
  QString nodeName;

  if( scaner->token() != 'n' ) {
    scaner->error( QStringLiteral("Waiting for declaration") );
    return;
    }

  if( scaner->matchTokenValue( QStringLiteral("USE") ) ) {
    if( scaner->token() != 'n' ) {
      scaner->error( QStringLiteral("Waiting for name of node") );
      return;
      }
    nodeName = scaner->tokenValue();
    VrmlNode *node = scaner->node( nodeName );
    if( node != nullptr ) {
      list->append( node->copy() );
      }
    scaner->tokenNext();
    return;
    }

  if( scaner->matchTokenValue( QStringLiteral("DEF") ) ) {
    if( scaner->token() != 'n' ) {
      scaner->error( QStringLiteral("Waiting for name of node") );
      return;
      }
    nodeName = scaner->tokenValue();
    scaner->tokenNext();
    }

  VrmlNode *node = parse2Node( scaner );
  if( node != nullptr ) {
    list->append( node );
    if( !nodeName.isEmpty() )
      scaner->insert( nodeName, node );
    }
  }



VrmlNode *VrmlNode::parse2Node(SdScanerVrml *scaner)
  {
  if( scaner->matchTokenValue( QStringLiteral("Group") ) )
    return parse2Group( list );
  if( matchTokenValue( QStringLiteral("Transform") ) )
    return parse2Transform( list );

  //Skeep undefined node
  tokenNext();
  if( mToken == '{' ) skeepBlock( '{', '}' );

  }
