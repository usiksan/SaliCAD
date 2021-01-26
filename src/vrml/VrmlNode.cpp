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
  QString nodeName, nodeType;

  if( !scaner->tokenNeedValue( 'n', nodeType, QStringLiteral("Waiting for declaration") ) )
    return;

  if( nodeType == QStringLiteral("USE") ) {
    if( !scaner->tokenNeedValue( 'n', nodeName, QStringLiteral("Waiting for name of node") ) )
      return;
    VrmlNode *node = scaner->node( nodeName );
    if( node != nullptr )
      list->append( node->copy() );
    return;
    }

  if( nodeType == QStringLiteral("DEF") ) {
    if( !scaner->tokenNeedValue( 'n', nodeName, QStringLiteral("Waiting for name of node") ) )
      return;
    if( !scaner->tokenNeedValue( 'n', nodeType, QStringLiteral("Waiting for node type") ) )
      return;
    }

  VrmlNode *node = parse2Node( scaner, nodeType );

  if( node != nullptr ) {
    list->append( node );
    if( !nodeName.isEmpty() )
      scaner->insert( nodeName, node );
    }

  }

VrmlNode *VrmlNode::parse2Node(SdScanerVrml *scaner, const QString nodeType)
  {
  VrmlNode *node = buildNode( nodeType );
  if( node != nullptr ) node->parse( scaner );
  else {
    //Skeep undefined node
    if( scaner->token() == '{' ) scaner->skeepBlock( '{', '}' );
    else scaner->error( QStringLiteral("Undefined context when skeep node %1").arg(scaner->token()) );
    }
  return node;
  }




VrmlNode *VrmlNode::buildNode(const QString nodeType)
  {
  if( nodeType == QStringLiteral("Apperance") )               return new VrmlNodeApperance();
  if( nodeType == QStringLiteral("Color") )                   return new VrmlNodeColor();
  if( nodeType == QStringLiteral("Coordinate") )              return new VrmlNodeCoordinate();
  if( nodeType == QStringLiteral("Group") )                   return new VrmlNodeGroup();
  if( nodeType == QStringLiteral("IndexedFaceSet") )          return new VrmlNodeIndexedFaceSet();
  if( nodeType == QStringLiteral("Material") )                return new VrmlNodeMaterial();
  if( nodeType == QStringLiteral("Normal") )                  return new VrmlNodeNormal();
  if( nodeType == QStringLiteral("Transform") )               return new VrmlNodeTransform();
  return nullptr;
  }
