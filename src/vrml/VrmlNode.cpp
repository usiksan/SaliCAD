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




void VrmlNode::generateFaces(std::function<void (const QVector3DList &, const QVector3DList &, const VrmlNodeMaterial *)> appendFace ) const
  {
  Q_UNUSED(appendFace)
  }




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
  if( node != nullptr ) {
    //Open bracket
    if( !scaner->tokenNeed( '{', QStringLiteral("Need open bracket for '%1'").arg(nodeType) ) )
      return node;

    //Block contents
    while( !scaner->matchToken( '}' ) ) {
      if( scaner->isEndOfScan() ) {
        scaner->error( QStringLiteral("Uncompleted block for '%1'").arg(nodeType) );
        return node;
        }
      if( scaner->isError() )
        return node;

      //Scan one field of block
      QString fieldType;
      if( !scaner->tokenNeedValue( 'n', fieldType, QStringLiteral("Need field type") ) )
        return node;

      if( !node->parse( scaner, fieldType ) ) {
        scaner->error( QStringLiteral("Undefined field type '%1' in '%2'").arg( fieldType, nodeType ) );
        return node;
        }
      }
    //Successfull completion
    }
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

void VrmlNode::deleteNode(VrmlNode *node)
  {
  if( node != nullptr )
    delete node;
  }
