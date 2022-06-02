#ifndef SD3DRBODY_H
#define SD3DRBODY_H

#include "Sd3dRegion.h"
#include "SdJsonIO.h"
#include "Sd3drFace.h"
#include "Sd3dMaterial.h"

#include <QList>
#include <QColor>




class Sd3drBody
  {
    Sd3drFaceList  mFaceList;
    Sd3dMaterial   mColor;
  public:
    Sd3drBody();

    void              colorListGet( Sd3ColorList &dst ) const { dst.append( mColor ); }

    void              colorListSet( const Sd3dMaterial &src ) { mColor = src; }

   // QColor            color() const { return mColorAmbient; }

    void              colorSet( QColor c ) { mColor.mAmbient = mColor.mDiffuse = c; }

    void              faceAppend( const Sd3drFace &ref ) { mFaceList.append( ref ); }

    void              faceAppend( const Sd3drFaceList &list ) { mFaceList.append(list); }

//    const QList<int> &faceRefList() const { return mFaceRefList; }

    void              draw( QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList, const QMatrix4x4 &map ) const;

    void              json( SdJsonWriter &js ) const;

    void              json( const SdJsonReader &js );
  };


using Sd3drBodyList = QList<Sd3drBody>;

#endif // SD3DRBODY_H
