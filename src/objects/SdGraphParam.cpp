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




void SdGraphParam::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  SdPtrConst<SdGraphParam> ptr(src);
  Q_ASSERT( ptr.isValid() );
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

