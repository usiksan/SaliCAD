#include "VrmlNode1Separator.h"
#include "VrmlNode1Material.h"
#include "VrmlNode1MaterialBinding.h"
#include "VrmlNode1Coordinate3.h"
#include "VrmlNode1IndexedFaceSet.h"
#include "VrmlNode1Normal.h"
#include "VrmlNodeMaterial.h"

VrmlNode1Separator::VrmlNode1Separator() :
  VrmlNode1(),
  mCoordinate3(nullptr),
  mIndexedFaceSet(nullptr),
  mMaterial(nullptr),
  mMaterialBinding(nullptr),
  mMatrixTransform(nullptr),
  mNormal(nullptr),
  mNormalBinding(nullptr),
  mRotation(nullptr),
  mScale(nullptr),
  mTransform(nullptr),
  mTranslation(nullptr)
  {

  }

VrmlNode1Separator::~VrmlNode1Separator()
  {
  deleteNode( mCoordinate3 );
  deleteNode( mIndexedFaceSet );
  deleteNode( mMaterial );
  deleteNode( mMaterialBinding );
  deleteNode( mMatrixTransform );
  deleteNode( mNormal );
  deleteNode( mNormalBinding );
  deleteNode( mRotation );
  deleteNode( mScale );
  deleteNode( mTransform );
  deleteNode( mTranslation );
  }



void VrmlNode1Separator::generateFaces(std::function<void (const QVector3DList &, const QVector3DList &, const VrmlNodeMaterial *)> appendFace) const
  {
  VrmlNode1MaterialBinding *nodeMaterialBinding = dynamic_cast<VrmlNode1MaterialBinding*>(mMaterialBinding);
  int materialBinding = nodeMaterialBinding == nullptr ? MATERIAL_BINDING_DEFAULT : nodeMaterialBinding->binding();

  VrmlNode1Material *nodeMaterial = dynamic_cast<VrmlNode1Material*>(mMaterial);
  VrmlNode1Coordinate3 *nodeCoordinate3 = dynamic_cast<VrmlNode1Coordinate3*>(mCoordinate3);
  VrmlNode1IndexedFaceSet *nodeIndexedFaceSet = dynamic_cast<VrmlNode1IndexedFaceSet*>(mIndexedFaceSet);
  VrmlNode1Normal *nodeNormal = dynamic_cast<VrmlNode1Normal*>(mNormal);

  if( nodeCoordinate3 != nullptr && nodeIndexedFaceSet != nullptr ) {
    int faceIndex = 0;
    int vertexIndex = 0;
    int normalIndex = 0;
    QVector3DList vertexList;
    QVector3DList normalList;
    VrmlNodeMaterial material;
    while( nodeIndexedFaceSet->isCoordValid(vertexIndex) ) {
      int pointIndex = nodeIndexedFaceSet->coordIndex(vertexIndex++);
      if( pointIndex < 0 ) {
        //Complete with vertex accumulate

        if( nodeNormal != nullptr ) {
          while( nodeIndexedFaceSet->isNormalValid(normalIndex) ) {
            int normal = nodeIndexedFaceSet->normalIndex(normalIndex++);
            if( normal < 0 )
              break;
            if( nodeNormal->isVectorValid(normal) )
              normalList.append( nodeNormal->vector(normal).toVector3d().normalized() );
            }
          }

        if( nodeIndexedFaceSet->isMaterialValid(faceIndex) ) {
          int materialIndex = nodeIndexedFaceSet->materialIndex(faceIndex++);
          material = nodeMaterial->material( materialIndex );
          }

        appendFace( vertexList, normalList, &material );
        vertexList.clear();
        normalList.clear();
        }
      else {
        if( nodeCoordinate3->isPointValid(pointIndex) )
          vertexList.append( nodeCoordinate3->point(pointIndex).toVector3d() * 1000.0 );
        }
      }
    }
  }



bool VrmlNode1Separator::parse(SdScanerVrml *scaner, const QString &fieldType)
  {

  if( fieldType == QStringLiteral("Coordinate3") )
    mCoordinate3 = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("IndexedFaceSet") )
    mIndexedFaceSet = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("Material") )
    mMaterial = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("MaterialBinding") )
    mMaterialBinding = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("MatrixTransform") )
    mMatrixTransform = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("Normal") )
    mNormal = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("NormalBinding") )
    mNormalBinding = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("Rotation") )
    mRotation = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("Scale") )
    mScale = parse1Node( scaner, fieldType );

  else if( fieldType == QStringLiteral("Transform") )
    mTransform = parse1Node( scaner, fieldType );


  else if( fieldType == QStringLiteral("Translation") )
    mTranslation = parse1Node( scaner, fieldType );

  else return false;

  return true;
  }


