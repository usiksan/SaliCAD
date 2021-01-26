#include "VrmlNode.h"
#include "SdScanerVrml.h"
#include "VrmlNodeAppearance.h"
#include "VrmlNodeColor.h"
#include "VrmlNodeCoordinate.h"
#include "VrmlNodeGroup.h"
#include "VrmlNodeIndexedFaceSet.h"
#include "VrmlNodeMaterial.h"
#include "VrmlNodeNormal.h"
#include "VrmlNodeShape.h"
#include "VrmlNodeTransform.h"


VrmlNode *VrmlNode::parse2Declaration(SdScanerVrml *scaner)
  {
  QString nodeName, nodeType;

  if( !scaner->tokenNeedValue( 'n', nodeType, QStringLiteral("Waiting for declaration") ) )
    return nullptr;

  if( nodeType == QStringLiteral("USE") ) {
    if( !scaner->tokenNeedValue( 'n', nodeName, QStringLiteral("Waiting for name of node") ) )
      return nullptr;
    VrmlNode *node = scaner->node( nodeName );
    if( node != nullptr )
      return node->copy();
    return nullptr;
    }

  if( nodeType == QStringLiteral("DEF") ) {
    if( !scaner->tokenNeedValue( 'n', nodeName, QStringLiteral("Waiting for name of node") ) )
      return nullptr;
    if( !scaner->tokenNeedValue( 'n', nodeType, QStringLiteral("Waiting for node type") ) )
      return nullptr;
    }

  VrmlNode *node = parse2Node( scaner, nodeType );

  if( node != nullptr && !nodeName.isEmpty() )
    scaner->insert( nodeName, node );

  return node;
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
  if( nodeType == QStringLiteral("Appearance") )              return new VrmlNodeAppearance();
  if( nodeType == QStringLiteral("Color") )                   return new VrmlNodeColor();
  if( nodeType == QStringLiteral("Coordinate") )              return new VrmlNodeCoordinate();
  if( nodeType == QStringLiteral("Group") )                   return new VrmlNodeGroup();
  if( nodeType == QStringLiteral("IndexedFaceSet") )          return new VrmlNodeIndexedFaceSet();
  if( nodeType == QStringLiteral("Material") )                return new VrmlNodeMaterial();
  if( nodeType == QStringLiteral("Normal") )                  return new VrmlNodeNormal();
  if( nodeType == QStringLiteral("Shape") )                   return new VrmlNodeShape();
  if( nodeType == QStringLiteral("Transform") )               return new VrmlNodeTransform();
  return nullptr;
  }

VrmlNode *VrmlNode::makeCopy(const VrmlNode *node)
  {
  if( node != nullptr )
    return node->copy();
  return nullptr;
  }
