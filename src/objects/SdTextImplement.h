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

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( const QString &prefix, SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const QString &prefix, const SdJsonReader &js);
  };

#endif // SDTEXTIMPLEMENT_H
