/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDTEXTIMPLEMENT_H
#define SDTEXTIMPLEMENT_H

#include "SdPoint.h"
#include "SdPropText.h"

#include <QJsonObject>

struct SdTextImplement
  {
    SdPropText mProp;     //Text properties
    SdPoint    mOrigin;   //Reference position in part or symbol context

    //Read-write
    void write( const QString name, QJsonObject &obj ) const;
    void read( const QString name, const QJsonObject &obj );
  };

#endif // SDTEXTIMPLEMENT_H
