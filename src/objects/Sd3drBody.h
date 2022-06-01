#ifndef SD3DRBODY_H
#define SD3DRBODY_H

#include "Sd3dRegion.h"
#include "SdJsonIO.h"
#include "Sd3drFace.h"

#include <QList>
#include <QColor>

class Sd3drBody
  {
    QList<int> mFaceRefList;
    QColor     mColorAmbient;
    QColor     mColorDiffuse;
    QColor     mColorSpecular;
  public:
    Sd3drBody();

    void              setAmbient( QColor c ) { mColorAmbient = c; }

    void              setDiffuse( QColor c ) { mColorDiffuse = c; }

    void              setSpecular( QColor c ) { mColorSpecular = c; }

    QColor            color() const { return mColorAmbient; }

    void              colorSet( QColor c ) { mColorAmbient = mColorDiffuse = c; }

    void              addRef( int ref ) { mFaceRefList.append( ref ); }

    void              addRef( const QList<int> &list ) { mFaceRefList.append(list); }

//    const QList<int> &faceRefList() const { return mFaceRefList; }

    void              draw( QOpenGLFunctions_2_0 *f, const Sd3drFaceList &faceList, const Sd3dRegion &vertexList, const QMatrix4x4 &map ) const;

    void              json( SdJsonWriter &js ) const;

    void              json( const SdJsonReader &js );
  };


using Sd3drBodyList = QList<Sd3drBody>;

#endif // SD3DRBODY_H
