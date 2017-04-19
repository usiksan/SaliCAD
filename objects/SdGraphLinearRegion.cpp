/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Line Countur - Region.
*/
#include "SdGraphLinearRegion.h"
#include "SdSegment.h"
#include "SdContext.h"
#include "SdSelector.h"

SdGraphLinearRegion::SdGraphLinearRegion() :
  SdGraphLinear(),
  mList(),
  mFlyIndex()
  {

  }




SdGraphLinearRegion::SdGraphLinearRegion(const SdPointList list, const SdPropLine &propLine) :
  SdGraphLinear( propLine ),
  mList( list ),
  mFlyIndex()
  {
  mFly.reserve( list.count() );
  bool fly = false;
  for( int i = 0; i < list.count(); i++ )
    mFly.append( fly );
  }




void SdGraphLinearRegion::cloneFrom(const SdObject *src)
  {
  SdGraphLinear::cloneFrom( src );
  const SdGraphLinearRegion *region = dynamic_cast<const SdGraphLinearRegion*>(src);
  if( region ) {
    mList = region->mList;
    mFly  = region->mFly;
    }
  }




void SdGraphLinearRegion::writeObject(QJsonObject &obj) const
  {
  SdGraphLinear::writeObject( obj );
  mList.write( QString("Vertex"), obj );
  }




void SdGraphLinearRegion::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphLinear::readObject( map, obj );
  mList.read( QString("Vertex"), obj );

  //Create array for Fly vertex flags
  mFly.clear();
  mFly.reserve( list.count() );
  bool fly = false;
  for( int i = 0; i < list.count(); i++ )
    mFly.append( fly );
  }



void SdGraphLinearRegion::move(SdPoint offset)
  {
  for( int i = 0; )
  }


