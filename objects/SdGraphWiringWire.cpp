/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphWiringWire.h"
#include "SdContainerSheetNet.h"

SdGraphWiringWire::SdGraphWiringWire()
  {

  }

SdGraphWiringWire::SdGraphWiringWire(SdPoint a, SdPoint b, const SdPropLine &prp)
  {

  }




SdContainerSheetNet *SdGraphWiringWire::getNet() const
  {
  return dynamic_cast<SdContainerSheetNet*>( getParent() );
  }



//Get wire name
QString SdGraphWiringWire::getWireName() const
  {
  SdContainerSheetNet *net = getNet();
  if( net ) return net->getNetName();
  return QString();
  }




void SdGraphWiringWire::accumLinked(SdPoint a, SdPoint b, SdSelector *sel, SdUndo *undo)
  {
  if( !getSelector() ) {
    //Yet not selected, selecting
    if( mA.isOnSegment(a, b) ) selectByPoint( mA, sel );
    else if( mB.isOnSegment(a, b) ) selectByPoint( mB, sel );
    else if( a.isOnSegment( mA, mB ) )
      //Point hang middle of segment, separate segment on two parts and select both
      fragmentation( a, sel, undo );
    else if( b.isOnSegment( mA, mB ) )
      //Point hang middle of segment, separate segment on two parts and select both
      fragmentation( b, sel, undo );
    }
  else {
    //Уже выделено
    if( mFix && mB.isOnSegment( a, b ) ) mFix = false;
    }
  }




void SdGraphWiringWire::fragmentation(SdPoint p, SdSelector *sel, SdUndo *undo)
  {
  //Save state
  saveState( undo );
  //Create new segment with points p and mB
  SdGraphWiringWire *wire = new SdGraphWiringWire( p, mB, mProp );
  //Current segment cut until p
  mB = p;
  //Insert new segment into net
  getNet()->insertChild( wire, undo );
  //Select current segment by point
  selectByPoint( p, sel );
  //Select created segment by point
  wire->selectByPoint( p, sel );
  }





void SdGraphWiringWire::attach(SdUndo *undo)
  {
  }

void SdGraphWiringWire::detach(SdUndo *undo)
  {
  }

void SdGraphWiringWire::cloneFrom(const SdObject *src)
  {
  }

void SdGraphWiringWire::writeObject(QJsonObject &obj) const
  {
  }

void SdGraphWiringWire::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  }

void SdGraphWiringWire::saveState(SdUndo *undo)
  {
  }

void SdGraphWiringWire::moveComplete(SdPoint grid, SdUndo *undo)
  {
  }

void SdGraphWiringWire::move(SdPoint offset)
  {
  }

void SdGraphWiringWire::rotate(SdPoint center, SdAngle angle)
  {
  }

void SdGraphWiringWire::mirror(SdPoint a, SdPoint b)
  {
  }

void SdGraphWiringWire::setProp(SdProp &prop)
  {
  }

void SdGraphWiringWire::getProp(SdProp &prop)
  {
  }

void SdGraphWiringWire::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  }

void SdGraphWiringWire::selectByRect(const SdRect &r, SdSelector *selector)
  {
  }

void SdGraphWiringWire::select(SdSelector *selector)
  {
  }

void SdGraphWiringWire::prepareMove()
  {
  }

bool SdGraphWiringWire::canHideLayer(SdLayer *layer)
  {
  }

bool SdGraphWiringWire::isVisible()
  {
  }

SdRect SdGraphWiringWire::getOverRect() const
  {
  }

void SdGraphWiringWire::draw(SdContext *dc)
  {
  }

int SdGraphWiringWire::behindCursor(SdPoint p)
  {
  }

bool SdGraphWiringWire::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  }

bool SdGraphWiringWire::snapPoint(SdSnapInfo *snap)
  {
  }

bool SdGraphWiringWire::getNetOnPoint(SdPoint p, QString &destName)
  {
  }
