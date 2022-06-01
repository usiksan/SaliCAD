#ifndef SD3DRBODY_H
#define SD3DRBODY_H

#include "Sd3dRegion.h"
#include "SdJsonIO.h"
#include "Sd3drFace.h"

#include <QList>
#include <QColor>

class Sd3drBody
  {
    Sd3drFaceList mFaceList;
    QColor        mColorAmbient;
    QColor        mColorDiffuse;
    QColor        mColorSpecular;
  public:
    Sd3drBody();

    void              setAmbient( QColor c ) { mColorAmbient = c; }

    void              setDiffuse( QColor c ) { mColorDiffuse = c; }

    void              setSpecular( QColor c ) { mColorSpecular = c; }

    QColor            color() const { return mColorAmbient; }

    void              colorSet( QColor c ) { mColorAmbient = mColorDiffuse = c; }

    void              faceAppend( const Sd3drFace &ref ) { mFaceList.append( ref ); }

    void              faceAppend( const Sd3drFaceList &list ) { mFaceList.append(list); }

//    const QList<int> &faceRefList() const { return mFaceRefList; }

    void              draw( QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList, const QMatrix4x4 &map ) const;

    void              json( SdJsonWriter &js ) const;

    void              json( const SdJsonReader &js );
  };


using Sd3drBodyList = QList<Sd3drBody>;

#endif // SD3DRBODY_H
