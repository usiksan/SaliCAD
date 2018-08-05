/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Separate pcb area of schematic sheet
*/
#include "SdGraphArea.h"
#include "SdPItemPlate.h"
#include "SdPItemSheet.h"
#include "SdGraphSymImp.h"
#include "SdProject.h"
#include "SdSelector.h"
#include "SdPropLayer.h"
#include "SdContext.h"

SdGraphArea::SdGraphArea() :
  SdGraph(),
  mPlate(nullptr)
  {

  }

SdGraphArea::SdGraphArea(const SdPointList list, const SdPropLine &propLine, SdPItemPlate *plate) :
  SdGraph(),
  mRegion(list),
  mRegionProp(propLine),
  mPlate(plate)
  {

  }




SdPItemSheet *SdGraphArea::getSheet() const
  {
  return dynamic_cast<SdPItemSheet*>( getParent() );
  }



bool SdGraphArea::isPointInside(SdPoint p) const
  {
  return mRegion.containsPoint( p, Qt::WindingFill );
  }



//Scan all components in sheet and move it in appropriate pcb if it inside this area
void SdGraphArea::attach(SdUndo *undo)
  {
  SdProject *prj = getSheet()->getProject();
  Q_ASSERT( prj != nullptr );
  //Realloc objects for this project
  mPlate = dynamic_cast<SdPItemPlate*>( prj->getFixedProjectItem(mPlate) );
  Q_ASSERT( mPlate != nullptr );

  //Reallocate all components in area
  getSheet()->forEach( dctSymImp, [undo, this] (SdObject *obj) -> bool {
    SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>( obj );
    Q_ASSERT( sym != nullptr );
    if( mRegion.containsPoint( sym->getOverRect().getTopLeft(), Qt::WindingFill ) ) {
      //Symbol fall into area. Move it to area's plate
      sym->moveToPlate( mPlate, undo );
      }
    return true;
    });
  }




void SdGraphArea::detach(SdUndo *undo)
  {
  SdPItemPlate *def = getSheet()->getProject()->getDefaultPlate();
  Q_ASSERT( def != nullptr );
  getSheet()->forEach( dctSymImp, [def, undo, this] (SdObject *obj) -> bool {
    SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>( obj );
    Q_ASSERT( sym != nullptr );
    if( mRegion.containsPoint( sym->getOverRect().getTopLeft(), Qt::WindingFill ) ) {
      //Symbol fall into area. Move it to defult plate
      sym->moveToPlate( def, undo );
      }
    return true;
    });
  }




void SdGraphArea::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  const SdGraphArea *area = dynamic_cast<const SdGraphArea*>( src );
  Q_ASSERT( area != nullptr );
  mRegion     = area->mRegion;
  mRegionProp = area->mRegionProp;
  mPlate      = area->mPlate;
  }




void SdGraphArea::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject( obj );
  mRegion.write( QStringLiteral("Region"), obj );
  mRegionProp.write( obj );
  writePtr( mPlate, QStringLiteral("Plate"), obj );
  }




void SdGraphArea::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mRegion.read( QStringLiteral("Region"), obj );
  mRegionProp.read( obj );
  mPlate = dynamic_cast<SdPItemPlate*>( readPtr( QStringLiteral("Plate"), map, obj ) );
  }




void SdGraphArea::saveState(SdUndo *undo)
  {
  undo->propLinePointTable( &mRegionProp, &mRegion );
  undo->platePointer( &mPlate );
  }




void SdGraphArea::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(grid)
  attach( undo );
  }



void SdGraphArea::move(SdPoint offset)
  {
  mRegion.move( mFlyIndex, offset );
  }



void SdGraphArea::rotate(SdPoint center, SdPropAngle angle)
  {
  mRegion.rotate( center, angle );
  }



void SdGraphArea::mirror(SdPoint a, SdPoint b)
  {
  mRegion.mirror( a, b );
  }



void SdGraphArea::setProp(SdPropSelected &prop)
  {
  mRegionProp = prop.mLineProp;
  }



void SdGraphArea::getProp(SdPropSelected &prop)
  {
  prop.mLineProp.append( mRegionProp );
  prop.mFilledPropMask |= spsLineProp;
  }



void SdGraphArea::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mRegionProp.mLayer.isEdited() ) {
    //For each pair points of list check segment made by this pair
    //and if test point p is on segment, then append two point of pair to select list
    for( int i = 0; i < mRegion.count() - 1; ++i )
      if( p.isOnSegment( mRegion.get(i), mRegion.get(i+1)) ) {
        if( !getSelector() ) {
          mFlyIndex.clear();
          selector->insert( this );
          }
        if( mRegion.get(i) == p ) mFlyIndex.insert( i );
        else if( mRegion.get(i+1) == p ) mFlyIndex.insert( i + 1 );
        else {
          mFlyIndex.insert( i );
          mFlyIndex.insert( i + 1 );
          }
        }
    }
  }



void SdGraphArea::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mRegionProp.mLayer.isEdited() ) {
    for( int i = 0; i < mRegion.count() - 1; ++i )
      if( r.isAccross( mRegion.get(i), mRegion.get(i+1)) ) {
        if( !getSelector() ) {
          mFlyIndex.clear();
          selector->insert( this );
          }
        mFlyIndex.insert( i );
        mFlyIndex.insert( i + 1 );
        }
    }
  }



void SdGraphArea::select(SdSelector *selector)
  {
  mFlyIndex.clear();
  for( int i = 0; i < mRegion.count(); ++i ) mFlyIndex.insert( i );
  if( selector != nullptr )
    selector->insert( this );
  }






void SdGraphArea::setLayerUsage()
  {
  mRegionProp.mLayer.setLayerUsage();
  }



bool SdGraphArea::isVisible()
  {
  return mRegionProp.mLayer.isVisible();
  }




SdRect SdGraphArea::getOverRect() const
  {
  return SdRect( mRegion.boundingRect() );
  }



void SdGraphArea::draw(SdContext *dc)
  {
  dc->region( mRegion, mRegionProp );
  }




int SdGraphArea::behindCursor(SdPoint p)
  {
  if( isVisible() ) {
    for( int i = 0; i < mRegion.count()-1; ++i )
      if( p.isOnSegment( mRegion.get(i), mRegion.get(i+1) ) )
        return getSelector() ? SEL_ELEM : UNSEL_ELEM;
    if( p.isOnSegment( mRegion.last(), mRegion.first() ) )
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
    }
  return 0;
  }



bool SdGraphArea::snapPoint(SdSnapInfo *snap)
  {
  Q_UNUSED(snap)
  //TODO D014 add snap to area
  return false;
  }
