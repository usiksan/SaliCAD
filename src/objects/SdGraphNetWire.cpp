/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  One segment of wire in schematic diagram
*/
#include "SdGraphNetWire.h"
#include "SdPItemSheet.h"
#include "SdContext.h"
#include "SdEnvir.h"
#include "SdSelector.h"
#include "SdSegment.h"
#include "SdJsonIO.h"

SdGraphNetWire::SdGraphNetWire() :
  SdGraphNet(),
  mDotA(false),
  mDotB(false),
  mDirX(false),
  mFixA(false),
  mFixB(false)
  {

  }

SdGraphNetWire::SdGraphNetWire(SdPoint a, SdPoint b, const QString netName, const SdPropLine &prp) :
  SdGraphNet(netName),
  mA(a),
  mB(b),
  mDotA(false),
  mDotB(false),
  mDirX(false),
  mFixA(false),
  mFixB(false)
  {
  mProp = prp;
  }







void SdGraphNetWire::accumLinked(SdPoint a, SdPoint b, SdSelector *sel, SdUndo *undo)
  {
  if( !getSelector() || mFixA != mFixB ) {
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
    if( mFixB && mB.isOnSegment( a, b ) ) mFixB = false;
    else if( mFixA && mA.isOnSegment( a, b ) ) mFixA = false;
    }
  }




void SdGraphNetWire::unionSegments(SdGraphNetWire *segment, SdUndo *undo )
  {
  Q_ASSERT( segment != nullptr );
//  if( !segment ) throw( CadError("DWirePic::Union: нет сегмента для объединения") );
//  if( !GetNet() ) throw( CadError("DWirePic::Union: нет цепи") );
  //Do not union with ownself
  if( segment != this ) {
    //Union only with same properties
    if( segment->mProp == mProp ) { //Объединять только содинаковыми свойствами
      if( mA.x() == mB.x() &&
          mA.x() == segment->mA.x() &&
          mA.x() == segment->mB.x() &&
          qMin(mA.y(),mB.y()) <= qMax(segment->mA.y(),segment->mB.y()) &&
          qMax(mA.y(),mB.y()) >= qMin(segment->mA.y(),segment->mB.y()) ) {
            //Continue on Y axiz
            //Продолжение по y
            int y1,y2;
            y1 = qMax( qMax(mA.y(),mB.y()), qMax(segment->mA.y(),segment->mB.y()) );
            y2 = qMin( qMin(mA.y(),mB.y()), qMin(segment->mA.y(),segment->mB.y()) );
            if(!((mA.y() != y1 && mA.y() != y2 && mDotA ) ||
                 (mB.y() != y1 && mB.y() != y2 && mDotB ) ||
                 (segment->mA.y() != y1 && segment->mA.y() != y2 && getNeedDot(segment->mA,segment->mB)) ||
                 (segment->mB.y() != y1 && segment->mB.y() != y2 && getNeedDot(segment->mB,segment->mA)) )) {
              segment->saveState( undo );
              segment->mA.ry() = y1;
              segment->mB.ry() = y2;
              getSheet()->deleteChild( this, undo );
              getSheet()->netWirePlace( segment, undo );
              }
            }
      else if( mA.y() == mB.y() &&
          mA.y() == segment->mA.y() &&
          mA.y() == segment->mB.y() &&
          qMin(mA.x(),mB.x()) <= qMax(segment->mA.x(),segment->mB.x()) &&
          qMax(mA.x(),mB.x()) >= qMin(segment->mA.x(),segment->mB.x()) ) {
            //Continue on X axiz
            //Продолжение по x
            int x1,x2;
            x1 = qMax( qMax(mA.x(),mB.x()), qMax(segment->mA.x(),segment->mB.x()) );
            x2 = qMin( qMin(mA.x(),mB.x()), qMin(segment->mA.x(),segment->mB.x()) );
            if(!((mA.x() != x1 && mA.x() != x2 && mDotA ) ||
                 (mB.x() != x1 && mB.x() != x2 && mDotB ) ||
                 (segment->mA.x() != x1 && segment->mA.x() != x2 && getNeedDot(segment->mA,segment->mB)) ||
                 (segment->mB.x() != x1 && segment->mB.x() != x2 && getNeedDot(segment->mB,segment->mA)) )) {
              segment->saveState( undo );
              segment->mA.rx() = x1;
              segment->mB.rx() = x2;
              getSheet()->deleteChild( this, undo );
              getSheet()->netWirePlace( segment, undo );
              }
            }
      }
    }
  }





void SdGraphNetWire::utilise(SdUndo *undo)
  {
  if( mA == mB )
    //Delete segment with zero length
    getSheet()->deleteChild( this, undo ); //Удалить, если нулевой длины
  else {
    //Test segment union
    getSheet()->forEach( dctNetWire, [this,undo] (SdObject *obj) -> bool {
      SdGraphNetWire *wire = dynamic_cast<SdGraphNetWire*>( obj );
      if( wire != nullptr && wire->getNetName() == getNetName() && wire != this )
        wire->unionSegments( this, undo );
      return true;
      });
    //Update dot nets
    mDotA = getNeedDot( mA, mB );
    mDotB = getNeedDot( mB, mA );
    }
  }





void SdGraphNetWire::calcVertexPoints(SdPoint &p1, SdPoint &p2, SdPoint gridSize) const
  {
  switch( mDirX ) {
    //Нет излома
    case vdNone :
      p1 = mA;
      p2 = p1;
      break;
    //Излом по X
    case vdX :
#ifdef NO_VERTEX
      p1.Set( info.a.x, info.b.y );
      p2.Set( info.b.x, p1.y );
#else
      p1.set( mA.x(), sred(mA.y(),mB.y(),gridSize.y()) );
      p2.set( mB.x(), p1.y() );
#endif
      break;
    //Излом по Y
    case vdY :
#ifdef NO_VERTEX
      p1.Set( info.b.x, info.a.y );
      p2.Set( p1.x, info.b.y );
#else
      p1.set( sred(mA.x(),mB.x(),gridSize.x()), mA.y() );
      p2.set( p1.x(), mB.y() );
#endif
      break;
    }
  }






void SdGraphNetWire::fragmentation(SdPoint p, SdSelector *sel, SdUndo *undo)
  {
  //Save state
  saveState( undo );
  //Create new segment with points p and mB
  SdGraphNetWire *wire = new SdGraphNetWire( p, mB, getNetName(), mProp );
  //Current segment cut until p
  mB = mA;
  //Insert new segment into net
  getSheet()->insertChild( wire, undo );
  mB = p;
  //Select current segment by point
  selectByPoint( p, sel );
  //Select created segment by point
  wire->selectByPoint( p, sel );
  }





bool SdGraphNetWire::getNeedDot(SdPoint a, SdPoint b)
  {
  int  nodeInCount = 0; //Счетчик сегментов, входящих в узел
  bool thereDot = false;    //Наличие точки в данном узле
  QString netName = getNetName();
  getSheet()->forEach( dctNetWire, [this,a,b,netName,&nodeInCount,&thereDot] (SdObject *obj) ->bool {
    SdGraphNetWire *wire = dynamic_cast<SdGraphNetWire*>(obj);
    if( wire != nullptr && wire != this && wire->getNetName() == netName && wire->isPointOnSection(a) ) {
      //Point is on testing segment [Точка на тестируемом сегменте]
      if( wire->getA() == a ) {
        //Point is on end A [Точка на конце a]
        if( wire->getDotA() ) thereDot = true;
        else nodeInCount++;
        }
      else if( wire->getB() == a ) {
        //Point is on end B [Точка на конце b]
        if( wire->getDotB() ) thereDot = true;
        else nodeInCount++;
        }
      else {
        SdSegment s1(wire->getA(),wire->getB()), s2( a, b );
        if( !s1.isContinue( s2 ) ) nodeInCount += 2;
        }
      }
    return !thereDot; //Блокировать дальнейшую итерацию, если точка уже есть
    });
  return nodeInCount > 1 && !thereDot;
  }




SdPoint SdGraphNetWire::getFixPoint(SdPoint a, SdPoint b)
  {
  QString netName(getNetName());
  getSheet()->forEach( dctNetWire, [a,&b,netName,this] (SdObject *obj) -> bool {
    SdGraphNetWire *wire = dynamic_cast<SdGraphNetWire*>( obj );
    if( wire != nullptr && wire->getNetName() == netName && wire != this ) {
      if( a != wire->getA() && b != wire->getA() && wire->getA().isOnSegment(a,b) ) b = wire->getA();
      if( a != wire->getB() && b != wire->getB() && wire->getB().isOnSegment(a,b) ) b = wire->getB();
      }
    return true;
    });
  return b;
  }







void SdGraphNetWire::attach(SdUndo *undo)
  {
  //Information about wire segment moving to make connection to pin
  getSheet()->netWirePlace( this, undo );
  //Utilise segment
  utilise(undo);
  //Calc dots
  //mDotA = getNeedDot( mA, mB );
  //mDotB = getNeedDot( mB, mA );
  }




void SdGraphNetWire::detach(SdUndo *undo)
  {
  //Information about wire segment delete to remove connection from pin
  getSheet()->netWireDelete( this, undo );
  }





//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphNetWire::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraphNet::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphNetWire> wire(src);
  Q_ASSERT( wire.isValid() );
  mA    = wire->mA;
  mB    = wire->mB;       //Wire segment
  mProp = wire->mProp;    //Wire drawing properties
  mDotA = wire->mDotA;
  mDotB = wire->mDotB;    //Dots present flag
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphNetWire::json(SdJsonWriter &js) const
  {
  //Wire segment
  js.jsonPoint( QStringLiteral("a"), mA );
  js.jsonPoint( QStringLiteral("b"), mB );

  //Wire drawing properties
  mProp.json( js );

  //Dots present flag
  js.jsonBool( QStringLiteral("dotA"), mDotA );
  js.jsonBool( QStringLiteral("dotB"), mDotB );

  SdGraphNet::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphNetWire::json(const SdJsonReader &js)
  {
  //Wire segment
  js.jsonPoint( QStringLiteral("a"), mA );
  js.jsonPoint( QStringLiteral("b"), mB );

  //Wire drawing properties
  mProp.json( js );

  //Dots present flag
  js.jsonBool( QStringLiteral("dotA"), mDotA );
  js.jsonBool( QStringLiteral("dotB"), mDotB );

  SdGraphNet::json( js );
  }








void SdGraphNetWire::saveState(SdUndo *undo)
  {
  undo->prop( &mProp, &mA, &mB, &mDotA, &mDotB );
  SdGraphNet::saveState(undo);
  }




void SdGraphNetWire::moveComplete(SdPoint grid, SdUndo *undo)
  {
  //Complete moving operation
  //Завершить операцию переноса
  if( getSelector() && mFixB != mFixA && mDirX ) {
    SdPoint p0(mA);
    SdPoint p1,p2;
    calcVertexPoints( p1, p2, grid );
    if( mB != p2 ) {
      saveState( undo );
      mA = p2;
      if( p0 != p1 ) {
        SdGraphNetWire *w = new SdGraphNetWire( p0, p1, getNetName(), mProp );
        getSheet()->insertChild( w, undo );
//        if( !w->isDeleted() )
//          w->select( getSelector() );
        }
      if( p1 != p2 ) {
        SdGraphNetWire *w = new SdGraphNetWire( p1, p2, getNetName(), mProp );
        getSheet()->insertChild( w, undo );
//        if( !w->isDeleted() )
//          w->select( getSelector() );
        }
      }
    }

  if( !isDeleted() ) {
    //Известить о завершении перемещения
    getSheet()->netWirePlace( this, undo );
    //Произвести утилизацию
    utilise( undo );
    }
  }




void SdGraphNetWire::transform(const QTransform &map, SdPvAngle)
  {
  if( !mFixB ) mB = map.map(mB);
  if( !mFixA ) mA = map.map(mA);
  }






//Set new properties to wire
void SdGraphNetWire::setProp(SdPropSelected &prop)
  {
  prop.mWireProp.store(mProp);
  SdGraphNet::setProp(prop);
  }



//Get properties from wire
void SdGraphNetWire::getProp(SdPropSelected &prop)
  {
  prop.mWireProp.append( mProp );
  prop.mFilledPropMask |= spsWireProp;
  SdGraphNet::getProp( prop );
  }



void SdGraphNetWire::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() ) {
      //Not selected yet
      if( p.isOnSegment(mA,mB) ) {
        //Test fixing variant
        if( mA == p ) { mFixB = true; mFixA = false; } //Point B is fixed, point A is fly
        else if( mB == p ) { mFixA = true; mFixB = false; }  //Point A is fixed, point B is fly
        else mFixB = mFixA = false;
        //Проверить отсутствие фиксации на плавающем сегменте

        //Определить способ и направление излома
        //Define vertex direction variant
        if( mA.x() == mB.x() ) mDirX = vdX;
        else if( mA.y() == mB.y() ) mDirX = vdY;
        else mDirX = vdNone;
        selector->insert( this );
        }
      }
    else {
      //Already selected
      if( p.isOnSegment(mA,mB) ) {
        //If one point if fixed, then check new fix condition
        //Если одна точка фиксирована, проверяем новое условие фиксации
        if( mFixB && mA != p ) mFixB = false;
        if( mFixA && mB != p ) mFixA = false;
        }
      }
    }
  }



void SdGraphNetWire::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() ) {
      //Not selected yet
      if( r.isPointInside(mA) || r.isPointInside(mB) ) {
        //Test if end catch in rect
        if( r.isPointInside(mA) && !r.isPointInside(mB) ) { mFixB = true; mFixA = false; }
        //В прямоугольник захвачен другой кончик
        else if( r.isPointInside(mB) && !r.isPointInside(mA) ) { mFixA = true; mFixB = false; }
        else mFixB = mFixA = false;
        //Define vertex direction variant
        if( mA.x() == mB.x() ) mDirX = vdX;
        else if( mA.y() == mB.y() ) mDirX = vdY;
        else mDirX = vdNone;
        selector->insert( this );
        }
      else if( r.isAccross(mA,mB) ) {
        //Rectangle over wire segment at middle. We select all segment
        selector->insert( this );
        mFixA = mFixB = false;
        }
      }
    else {
      //Already selected
      if( r.isPointInside(mA) || r.isPointInside(mB) ) {
        //Text fixing condition
        if( mFixB && r.isPointInside(mB) ) mFixB = false;
        if( mFixA && r.isPointInside(mA) ) mFixA = false;
        }
      }
    }
  }



void SdGraphNetWire::select(SdSelector *selector)
  {
  mFixA = mFixB = false;
  mDirX = vdNone;
  if( selector != nullptr )
    selector->insert( this );
  }



void SdGraphNetWire::prepareMove(SdUndo *undo)
  {
  Q_ASSERT( getSelector() );
  //prepare moving
  if( mFixB ) {
    //Calculate fix point
    //We need stretch from nearest dot
    SdPoint b = getFixPoint( mA, mB );
    if( b != mB ) {
      //Break segment
      saveState( undo );
      SdPoint p = mB;
      mB = mA;
      getSheet()->insertChild( new SdGraphNetWire( b, p, getNetName(), mProp ), undo );
      mB = b;
      }
    else {
      //fix = false;
      //GetNet()->AccumLinked( info.a, info.b, *GetSelect(), undo );
      }
    }
  else if( mFixA ) {
    //Calculate fix point
    SdPoint a = getFixPoint( mB, mA );
    if( a != mA ) {
      //Break segment
      saveState( undo );
      SdPoint p = mA;
      mA = mB;
      getSheet()->insertChild( new SdGraphNetWire( a, p, getNetName(), mProp ), undo );
      mA = a;
      }
    else {
      //fix = false;
      //GetNet()->AccumLinked( info.a, info.b, *GetSelect(), undo );
      }
    }
  else {
    //Select linked segments and components (symbol implementation)
    getSheet()->accumLinked( mA, mB, getNetName(), getSelector(), undo );
    }
  }





void SdGraphNetWire::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
  }






bool SdGraphNetWire::isVisible()
  {
  return mProp.mLayer.isVisible();
  }




SdRect SdGraphNetWire::getOverRect() const
  {
  return SdRect( mA, mB );
  }



void SdGraphNetWire::draw(SdContext *dc)
  {
  SdPoint p1,p2;
  calcVertexPoints( p1, p2, dc->getGrid() );
  if( mA != p1 ) dc->line( mA, p1, mProp );
  if( p1 != p2 ) dc->line( p1, p2, mProp );
  if( p2 != mB ) dc->line( p2, mB, mProp );
  //Draw dot points
  if( mDotA ) dc->circleFill( mA, SdEnvir::instance()->mDotSize, mProp.mLayer.layer() );
  if( mDotB ) dc->circleFill( mB, SdEnvir::instance()->mDotSize, mProp.mLayer.layer() );
  }




int SdGraphNetWire::behindCursor(SdPoint p)
  {
  //Определить состояние объекта под курсором
  if( mProp.mLayer.isVisible() ) {
    if( p.isOnSegment( mA, mB) ) {
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
      }
    }
  return 0;
  }




bool SdGraphNetWire::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  Q_UNUSED(extInfo)
  if( behindCursor( p ) ) {
    info = QObject::tr( "Net: " ) + getNetName();
    return true;
    }
  return false;
  }




//Find snap point on object
void SdGraphNetWire::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match( snapNearestNet ) ) {
    SdPoint dest;
    if( calcFreeNearIntersect( snap->mSour, mA, mB, dest ) )
      snap->test( this, dest, snapNearestNet );
    }
  }





void SdGraphNetWire::setNetName(const QString netName, SdUndo *undo)
  {
  if( undo ) {
    undo->prop( &mProp, &mA, &mB, &mDotA, &mDotB );
    //Information about wire segment delete to remove connection from pin
    getSheet()->netWireDelete( this, undo );
    SdGraphNet::setNetName( netName, undo );
    attach( undo );
    }
  else
    SdGraphNet::setNetName( netName, undo );
  }

