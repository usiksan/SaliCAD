/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Any param in sheet
*/
#include "SdGraphParam.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "SvJsonIO.h"

SdGraphParam::SdGraphParam() :
  SdGraph()
  {

  }

SdGraphParam::SdGraphParam(const SdStringMap &param) :
  SdGraph(),
  mParamTable(param)
  {
  }




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphParam::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraph::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphParam> ptr(src);
  Q_ASSERT_X( ptr.isValid(), "SdGraphParam::cloneFrom", "not SdGraphParam object" );
  mParamTable = ptr->mParamTable;
  }







void SdGraphParam::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject( obj );
  //Parameters
  sdStringMapWrite( QStringLiteral("Param"), mParamTable, obj );
  }



void SdGraphParam::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  //Parameters
  sdStringMapRead( QStringLiteral("Param"), mParamTable, obj );
  }

