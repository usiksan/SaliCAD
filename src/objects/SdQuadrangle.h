/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Free rotating rectangle
*/

#ifndef SDQUADRANGLE_H
#define SDQUADRANGLE_H

#include "SdPoint.h"
#include "SdRect.h"

struct SdQuadrangle
  {
    SdPoint p1,p2,p3,p4;   //Quadrangle corners

    SdQuadrangle();
    SdQuadrangle( SdPoint s1, SdPoint s2, SdPoint s3, SdPoint s4 );
    SdQuadrangle( int x, int y, int w, int h );
    SdQuadrangle( SdRect r );

    void rotate( SdPoint center, SdPvAngle angle );
    void move( SdPoint offset );
    void set( SdRect r );

//    QJsonObject write() const;
//    void        write( const QString name, QJsonObject &dest ) const;
//    void        read( const QJsonObject obj );
//    void        read( const QString name, const QJsonObject src );
  };

#endif // SDQUADRANGLE_H
