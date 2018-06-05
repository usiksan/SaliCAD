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
#include "SdGraphNetWire.h"
#include "SdProject.h"
#include "SdPItemSheet.h"
#include "SdSegment.h"

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





//Return nesessity of dot on net vertex
bool SdContainerSheetNet::getNeedDot(SdPoint p, SdGraphNetWire *own)
  {
  int  nodeInCount = 0; //Счетчик сегментов, входящих в узел
  bool thereDot = false;    //Наличие точки в данном узле
  forEach( dctWire, [p,own,&nodeInCount,&thereDot] (SdObject *obj) ->bool {
    SdGraphNetWire *wire = dynamic_cast<SdGraphNetWire*>(obj);
    if( obj != own && wire && wire->isPointOnSection(p) ) {
      //Точка на тестируемом сегменте
      if( wire->getA() == p ) {
        //Точка на конце a
        if( wire->getDotA() ) thereDot = true;
        else nodeInCount++;
        }
      else if( wire->getB() == p ) {
        //Точка на конце b
        if( wire->getDotB() ) thereDot = true;
        else nodeInCount++;
        }
      else {
        SdSegment s1(wire->getA(),wire->getB()), s2( own->getA(), own->getB() );
        if( !s1.isContinue( s2 ) ) nodeInCount += 2;
        }
      }
    return !thereDot; //Блокировать дальнейшую итерацию, если точка уже есть
    });
  return nodeInCount > 1 && !thereDot;
  }




//Return fix point of movig segment
SdPoint SdContainerSheetNet::getFixPoint(SdPoint a, SdPoint b)
  {
  forEach( dctWire, [a,&b] (SdObject *obj) -> bool {
    SdGraphNetWire *wire = dynamic_cast<SdGraphNetWire*>( obj );
    Q_ASSERT( wire != nullptr );
    if( a != wire->getA() && b != wire->getA() && wire->getA().isOnSegment(a,b) ) b = wire->getA();
    if( a != wire->getB() && b != wire->getB() && wire->getB().isOnSegment(a,b) ) b = wire->getB();
    return true;
    });
  return b;
  }




void SdContainerSheetNet::accumLinked(SdPoint a, SdPoint b, SdSelector *sel)
  {
  SdProject *prj = dynamic_cast<SdProject*>( getRoot() );
  SdUndo *undo = prj == nullptr ? nullptr : prj->getUndo();
  forEach( dctWire, [a,b,sel,undo] (SdObject *obj) -> bool {
    SdGraphNetWire *wire = dynamic_cast<SdGraphNetWire*>( obj );
    Q_ASSERT( wire != nullptr );
    wire->accumLinked( a, b, sel, undo );
    return true;
    });
  }




//Union segments
void SdContainerSheetNet::unionSegment(SdGraphNetWire *sour, SdUndo *undo)
  {
  forEach( dctWire, [sour,undo] (SdObject *obj) -> bool {
    SdGraphNetWire *wire = dynamic_cast<SdGraphNetWire*>( obj );
    Q_ASSERT( wire != nullptr );
    if( wire != sour ) wire->unionSegments( sour, undo );
    return true;
    });
  }




//Notification about segment position is changed
void SdContainerSheetNet::netWirePlace(SdPoint a, SdPoint b, SdUndo *undo )
  {
  getSheet()->netWirePlace( a, b, mNetName, undo );
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

