#include "VrmlNode1.h"
#include "SdScanerVrml.h"
#include "VrmlNode1Coordinate3.h"
#include "VrmlNode1IndexedFaceSet.h"
#include "VrmlNode1Material.h"
#include "VrmlNode1MaterialBinding.h"
#include "VrmlNode1MatrixTransform.h"
#include "VrmlNode1Normal.h"
#include "VrmlNode1NormalBinding.h"
#include "VrmlNode1Rotation.h"
#include "VrmlNode1Scale.h"
#include "VrmlNode1Separator.h"
#include "VrmlNode1Transform.h"
#include "VrmlNode1Translation.h"


VrmlNode *VrmlNode1::parse1Node(SdScanerVrml *scaner, const QString nodeType)
  {
  VrmlNode *node = build1Node( nodeType );
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


VrmlNode *VrmlNode1::build1Node(const QString nodeType)
  {
  if( nodeType == QStringLiteral("Coordinate3") )              return new VrmlNode1Coordinate3();
  if( nodeType == QStringLiteral("IndexedFaceSet") )           return new VrmlNode1IndexedFaceSet();
  if( nodeType == QStringLiteral("Material") )                 return new VrmlNode1Material();
  if( nodeType == QStringLiteral("MaterialBinding") )          return new VrmlNode1MaterialBinding();
  if( nodeType == QStringLiteral("MatrixTransform") )          return new VrmlNode1MatrixTransform();
  if( nodeType == QStringLiteral("Normal") )                   return new VrmlNode1Normal();
  if( nodeType == QStringLiteral("NormalBinding") )            return new VrmlNode1NormalBinding();
  if( nodeType == QStringLiteral("Rotation") )                 return new VrmlNode1Rotation();
  if( nodeType == QStringLiteral("Scale") )                    return new VrmlNode1Scale();
  if( nodeType == QStringLiteral("Separator") )                return new VrmlNode1Separator();
  if( nodeType == QStringLiteral("Transform") )                return new VrmlNode1Transform();
  if( nodeType == QStringLiteral("Translation") )              return new VrmlNode1Translation();
  return nullptr;
  }
