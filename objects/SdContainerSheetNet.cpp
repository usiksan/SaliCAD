/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdContainerSheetNet.h"
#include "SdGraphWiringWire.h"
#include "SdProject.h"
#include "SdPItemSheet.h"

SdContainerSheetNet::SdContainerSheetNet() :
  SdContainer()
  {

  }




SdContainerSheetNet::SdContainerSheetNet(const QString netName) :
  SdContainer(),
  mNetName(netName)
  {

  }




//Return sheet where in net inserted
SdPItemSheet *SdContainerSheetNet::getSheet() const
  {
  return dynamic_cast<SdPItemSheet*>(getParent());
  }




void SdContainerSheetNet::accumLinked(SdPoint a, SdPoint b, SdSelector *sel)
  {
  SdProject *prj = dynamic_cast<SdProject*>( getRoot() );
  SdUndo *undo = prj == nullptr ? nullptr : prj->getUndo();
  forEach( dctWire, [a,b,sel,undo] (SdObject *obj) -> bool {
    SdGraphWiringWire *wire = dynamic_cast<SdGraphWiringWire*>( obj );
    Q_ASSERT( wire != nullptr );
    wire->accumLinked( a, b, sel, undo );
    return true;
    });
  }




void SdContainerSheetNet::cloneFrom(const SdObject *src)
  {
  SdContainer::cloneFrom( src );
  const SdContainerSheetNet *net = dynamic_cast<const SdContainerSheetNet*>(src);
  Q_ASSERT( net != nullptr );
  mNetName = net->mNetName;
  }




void SdContainerSheetNet::writeObject(QJsonObject &obj) const
  {
  SdContainer::writeObject(obj);
  obj.insert( QStringLiteral("NetName"), mNetName );
  }



void SdContainerSheetNet::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mNetName = obj.value( QStringLiteral("NetName") ).toString();
  }

