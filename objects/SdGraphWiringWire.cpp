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
#include "SdGraphWiringWire.h"
#include "SdContainerSheetNet.h"
#include "SdPItemSheet.h"
#include "SdContext.h"
#include "SdEnvir.h"
#include "SdSelector.h"

SdGraphWiringWire::SdGraphWiringWire() :
  SdGraphWiring(),
  mDotA(false), mDotB(false),
  mDirX(false),
  mFix(0)
  {

  }

SdGraphWiringWire::SdGraphWiringWire(SdPoint a, SdPoint b, const SdPropLine &prp) :
  SdGraphWiring(),
  mA(a), mB(b),
  mDotA(false), mDotB(false),
  mDirX(false),
  mFix(0)
  {
  mProp = prp;
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




void SdGraphWiringWire::unionSegments(SdGraphWiringWire *segment, SdUndo *undo )
  {
  Q_ASSERT( segment != nullptr && getNet() != nullptr );
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
            //Continue on X axiz
            //Продолжение по x
            int tmp = segment->mA.y();
            int y1,y2;
            y1 = qMax( qMax(mA.y(),mB.y()), qMax(segment->mA.y(),segment->mB.y()) );
            y2 = qMin( qMin(mA.y(),mB.y()), qMin(tmp,segment->mB.y()) );
            if(!((mA.y() != y1 && mA.y() != y2 && mDotA ) ||
                 (mB.y() != y1 && mB.y() != y2 && mDotB ) ||
                 (segment->mA.y() != y1 && segment->mA.y() != y2 && getNet()->getNeedDot(segment->mA,segment)) ||
                 (segment->mB.y() != y1 && segment->mB.y() != y2 && getNet()->getNeedDot(segment->mB,segment)) )) {
              segment->saveState( undo );
              segment->mA.ry() = y1;
              segment->mB.ry() = y2;
              getNet()->deleteChild( this, undo );
              getNet()->netWirePlace( segment->mA, segment->mB, undo );
              }
            }
      else if( mA.y() == mB.y() &&
          mA.y() == segment->mA.y() &&
          mA.y() == segment->mB.y() &&
          qMin(mA.x(),mB.x()) <= qMax(segment->mA.x(),segment->mB.x()) &&
          qMax(mA.x(),mB.x()) >= qMin(segment->mA.x(),segment->mB.x()) ) {
            //Continue on Y axiz
            //Продолжение по y
            int tmp = segment->mA.x();
            int x1,x2;
            x1 = qMax( qMax(mA.x(),mB.x()), qMax(segment->mA.x(),segment->mB.x()) );
            x2 = qMin( qMin(mA.x(),mB.x()), qMin(tmp,segment->mB.x()) );
            if(!((mA.x() != x1 && mA.x() != x2 && mDotA ) ||
                 (mB.x() != x1 && mB.x() != x2 && mDotB ) ||
                 (segment->mA.x() != x1 && segment->mA.x() != x2 && getNet()->getNeedDot(segment->mA,segment)) ||
                 (segment->mB.x() != x1 && segment->mB.x() != x2 && getNet()->getNeedDot(segment->mB,segment)) )) {
              segment->saveState( undo );
              segment->mA.rx() = x1;
              segment->mB.rx() = x2;
              getNet()->deleteChild( this, undo );
              getNet()->netWirePlace( segment->mA, segment->mB, undo );
              }
            }
      }
    }
  }





void SdGraphWiringWire::utilise(SdUndo *undo)
  {
  if( mA == mB )
    //Delete segment with zero length
    getNet()->deleteChild( this, undo ); //Удалить, если нулевой длины
  else
    //Test segment union
    getNet()->unionSegment( this, undo );  //Проверить объединение сегментов
  }





void SdGraphWiringWire::calcVertexPoints(SdPoint &p1, SdPoint &p2, SdPoint gridSize)
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



void SdGraphWiringWire::exchange()
  {
  mA.swap( &mB );
  bool dot = mDotA;
  mDotA = mDotB;
  mDotB = dot;
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



bool SdGraphWiringWire::getNeedDot(SdPoint p)
  {
  return getNet()->getNeedDot( p, this );
  }





void SdGraphWiringWire::attach(SdUndo *undo)
  {
  getNet()->getSheet()->netWirePlace( mA, mB, getWireName(), undo );
  //Calc dots
  mDotA = getNeedDot( mA );
  mDotB = getNeedDot( mB );
  }




void SdGraphWiringWire::detach(SdUndo *undo)
  {
  getNet()->getSheet()->netWireDelete( mA, mB, getWireName(), undo );
  }



void SdGraphWiringWire::cloneFrom(const SdObject *src)
  {
  SdGraphWiring::cloneFrom( src );
  const SdGraphWiringWire *wire = dynamic_cast<const SdGraphWiringWire*>(src);
  Q_ASSERT(wire != nullptr);
  mA    = wire->mA;
  mB    = wire->mB;       //Wire segment
  mProp = wire->mProp;    //Wire drawing properties
  mDotA = wire->mDotA;
  mDotB = wire->mDotB;    //Dots present flag
  }



void SdGraphWiringWire::writeObject(QJsonObject &obj) const
  {
  SdGraphWiring::writeObject(obj);
  mA.write( QStringLiteral("a"), obj );
  mB.write( QStringLiteral("b"), obj );        //Wire segment
  mProp.write( obj );       //Wire drawing properties
  obj.insert( QStringLiteral("dotA"), mDotA );
  obj.insert( QStringLiteral("dotB"), mDotB ); //Dots present flag
  }




void SdGraphWiringWire::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphWiring::readObject( map, obj );
  mA.read( QStringLiteral("a"), obj );
  mB.read( QStringLiteral("b"), obj );        //Wire segment
  mProp.read( obj );       //Wire drawing properties
  mDotA = obj.value( QStringLiteral("dotA") ).toBool();
  mDotB = obj.value( QStringLiteral("dotB") ).toBool(); //Dots present flag
  }




void SdGraphWiringWire::saveState(SdUndo *undo)
  {
  undo->wire( &mProp, &mA, &mB, &mDotA, &mDotB );
  }




void SdGraphWiringWire::moveComplete(SdPoint grid, SdUndo *undo)
  {
//  if( !GetNet() ) throw CadError("DWirePic::MoveComplete: нет цепи");
  //Complete moving operation
  //Завершить операцию переноса
  if( getSelector() && mFix && mDirX ) {
    SdPoint p1,p2;
    calcVertexPoints( p1, p2, grid );
    SdPoint p3(mA);
    if( mB != p2 ) {
      saveState( undo );
      mA = p2;
      if( p3 != p1 ) getNet()->insertChild( new SdGraphWiringWire( p3, p1, mProp ), undo );
      if( p1 != p2 ) getNet()->insertChild( new SdGraphWiringWire( p1, p2, mProp ), undo );
      }
    }
  //Известить о завершении перемещения
  getNet()->netWirePlace( mA, mB, undo );
  //Произвести утилизацию
  utilise( undo );
  }




void SdGraphWiringWire::move(SdPoint offset)
  {
  if( !mFix ) mB.move( offset );
  mA.move( offset );
  }




void SdGraphWiringWire::rotate(SdPoint center, SdAngle angle)
  {
  if( !mFix ) mB.rotate( center, angle );
  mA.rotate( center, angle );
  }




void SdGraphWiringWire::mirror(SdPoint a, SdPoint b)
  {
  if( !mFix ) mB.mirror( a, b );
  mA.mirror( a, b );
  }




void SdGraphWiringWire::setProp(SdPropSelected &prop)
  {
  mProp = prop.mWireProp;
  //TODO when change name of wire
  }



void SdGraphWiringWire::getProp(SdPropSelected &prop)
  {
  prop.mWireProp.append( mProp );
  prop.mFilledPropMask |= spsWireProp;
  prop.mWireName.append( getWireName() );
  }



void SdGraphWiringWire::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() ) {
      //Not selected yet
      if( p.isOnSegment(mA,mB) ) {
        //Test fixing variant
        if( mA == p ) mFix = true; //Point B is fixed, point A is fly
        else if( mB == p ) {
          //Point A is fixed, point B is fly
          //so exchange points and will be as prior
          exchange();
          mFix = true;
          }
        else mFix = false;
        //Проверить отсутствие фиксации на плавающем сегменте

        //Определить способ и направление излома
        //Define break variant
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
        if( mFix && mA != p ) mFix = false;
        }
      }
    }
  }



void SdGraphWiringWire::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() ) {
      //Not selected yet
      if( r.isAccross( mA, mB) ) {
        //Test if end catch in rect
        if( r.isPointInside(mA) && !r.isPointInside(mB) ) mFix = true;
        //В прямоугольник захвачен другой кончик
        else if( r.isPointInside(mB) && !r.isPointInside(mA) ) {
          exchange(); //Обменять точки
          mFix = true;
          }
        else mFix = false;
        //Define break variant
        if( mA.x() == mB.x() ) mDirX = vdX;
        else if( mA.y() == mB.y() ) mDirX = vdY;
        else mDirX = vdNone;
        selector->insert( this );
        }
      }
    else {
      //Already selected
      if( r.isAccross(mA,mB) ) {
        //Text fixing condition
        if( !r.isPointInside(mA) || r.isPointInside(mB) ) mFix = false;
        }
      }
    }
  }



void SdGraphWiringWire::select(SdSelector *selector)
  {
  mFix = false;
  selector->insert( this );
  }



void SdGraphWiringWire::prepareMove(SdUndo *undo)
  {
  Q_ASSERT( getNet() && getSelector() );
  //if( !GetNet() || !GetSelect() ) throw CadError("DWirePic::PrepareMove");
  //prepare moving
  if( mFix ) {
    //Calculate fix point
    SdPoint b = getNet()->getFixPoint( mA, mB );
    if( b != mB ) {
      //Break segment
      getNet()->insertChild( new SdGraphWiringWire( b, mB, mProp ), undo );
      saveState( undo );
      mB = b;
      }
    else {
      //fix = false;
      //GetNet()->AccumLinked( info.a, info.b, *GetSelect(), undo );
      }
    }
  else {
    //Select linked segments and components (symbol implementation)
    getNet()->accumLinked( mA, mB, getSelector() );
    }
  }



bool SdGraphWiringWire::canHideLayer(SdLayer *layer)
  {
  return mProp.mLayer.canHide(layer);
  }




bool SdGraphWiringWire::isVisible()
  {
  return mProp.mLayer.isVisible();
  }




SdRect SdGraphWiringWire::getOverRect() const
  {
  return SdRect( mA, mB );
  }



void SdGraphWiringWire::draw(SdContext *dc)
  {
  SdPoint p1,p2;
  calcVertexPoints( p1, p2, dc->getGrid() );
  if( mA != p1 ) dc->line( mA, p1, mProp );
  if( p1 != p2 ) dc->line( p1, p2, mProp );
  if( p2 != mB ) dc->line( p2, mB, mProp );
  //Draw dot points
  if( mDotA ) dc->circleFill( mA, sdEnvir->mDotSize, mProp );
  if( mDotB ) dc->circleFill( mB, sdEnvir->mDotSize, mProp );
  }




int SdGraphWiringWire::behindCursor(SdPoint p)
  {
  //Определить состояние объекта под курсором
  if( mProp.mLayer.isVisible() ) {
    if( p.isOnSegment( mA, mB) ) {
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
      }
    }
  return 0;
  }




bool SdGraphWiringWire::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  Q_UNUSED(extInfo)
  if( behindCursor( p ) ) {
    info = QObject::tr( "Net: " ) + getWireName();
    return true;
    }
  return false;
  }




bool SdGraphWiringWire::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match( snapNearestNet ) ) {
    SdPoint dest;
    return calcFreeNearIntersect( snap->mSour, mA, mB, dest ) && snap->test( dest, snapNearestNet );
    }
  return false;
  }




bool SdGraphWiringWire::getNetOnPoint(SdPoint p, QString &destName)
  {
  if( p.isOnSegment( mA, mB ) ) {
    destName = getWireName();
    return true;
    }
  return false;
  }
