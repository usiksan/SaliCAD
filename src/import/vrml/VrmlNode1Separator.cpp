#include "VrmlNode1Separator.h"
#include "VrmlNode1Material.h"
#include "VrmlNode1MaterialBinding.h"
#include "VrmlNode1Coordinate3.h"
#include "VrmlNode1IndexedFaceSet.h"
#include "VrmlNode1Normal.h"
#include "VrmlNodeMaterial.h"
#include "3dr/Sd3drModel.h"

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



void VrmlNode1Separator::generateFaces(Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body) const
  {
  VrmlNode1Material *nodeMaterial = dynamic_cast<VrmlNode1Material*>(mMaterial);
  VrmlNode1Coordinate3 *nodeCoordinate3 = dynamic_cast<VrmlNode1Coordinate3*>(mCoordinate3);
  VrmlNode1IndexedFaceSet *nodeIndexedFaceSet = dynamic_cast<VrmlNode1IndexedFaceSet*>(mIndexedFaceSet);

  if( nodeCoordinate3 != nullptr && nodeIndexedFaceSet != nullptr ) {
    //Append all vertexes to model
    int vertexIndexStart = model->vertexCount();
    for( int i = 0; i < nodeCoordinate3->pointCount(); i++ )
      model->vertexAppend( nodeCoordinate3->point(i).toVector3d() * 1000.0 );

    int faceIndex = 0;
    int vertexIndex = 0;
    int materialIndexCurrent = -1;
    Sd3drFace face;
    Sd3drFaceList faceList;
    VrmlNodeMaterial material;

    if( nodeMaterial != nullptr )
      material = nodeMaterial->material( 0 );

    instance.clear();

    while( nodeIndexedFaceSet->isCoordValid(vertexIndex) ) {
      int pointIndex = nodeIndexedFaceSet->coordIndex(vertexIndex++);
      if( pointIndex < 0 ) {
        //Complete with vertex accumulate
        faceList.append( face );

        if( nodeIndexedFaceSet->isMaterialValid(faceIndex) ) {
          int materialIndex = nodeIndexedFaceSet->materialIndex(faceIndex++);
          material = nodeMaterial->material( materialIndex );
          if( materialIndex != materialIndexCurrent ) {
            materialIndexCurrent = materialIndex;
            body.clear();
            body.faceAppend( faceList );
            body.colorListSet( material.to3dMaterial() );
            instance.add( body );
            faceList.clear();
            }
          }
        face.clear();
        }
      else {
        face.append( vertexIndexStart + pointIndex );
        }
      }
    //Last face list
    if( faceList.count() > 0 ) {
      Sd3drBody body;
      body.clear();
      body.faceAppend( faceList );
      body.colorListSet( material.to3dMaterial() );
      instance.add( body );
      }
    instance.addCopy( QMatrix4x4() );
    model->instanceAppend( instance );
    instance.clear();
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



