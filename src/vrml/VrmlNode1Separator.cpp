#include "VrmlNode1Separator.h"

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



