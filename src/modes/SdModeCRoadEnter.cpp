/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter plate trace road
*/
#include "SdModeCRoadEnter.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdProp.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphTracedRoad.h"
#include "objects/SdGraphTracedVia.h"
#include "objects/SdPulsar.h"
#include "windows/SdPropBarRoad.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

#include <QDebug>
#include <math.h>

SdModeCRoadEnter::SdModeCRoadEnter(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mRoadMiddle(nullptr)
  {
  mProp = sdGlobalProp->mRoadProp;
  mViaProp = sdGlobalProp->mViaProp;
  }




void SdModeCRoadEnter::reset()
  {
  setDirtyCashe();
  setStep( sFirstPoint );
  mEnterPath.clear();
  }


void SdModeCRoadEnter::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mProp.mStratum, getStep() ? mProp.mNetName.str() : QString() );
  }






void SdModeCRoadEnter::drawDynamic(SdContext *ctx)
  {
  //Draw active segment
  if( getStep() ) {
    SdLayer *layer = sdEnvir->mCacheForRoad.getVisibleLayer(mProp.mStratum);
    if( layer != nullptr ) ctx->setPen( mProp.mWidth, layer, dltSolid );
    else ctx->setPen( mProp.mWidth, sdEnvir->getSysColor(scEnter), dltSolid );
    if( mFirst != mBarMiddle )
      ctx->line( mFirst, mBarMiddle );
    if( mBarMiddle != mBarLast )
      ctx->line( mBarMiddle, mBarLast );

    //When enter path smart point is nearest unconnected pin
    if( sdEnvir->mIsWireSmart && !mTargetPoint.isFar() ) {
      ctx->smartPoint( mTargetPoint, snapEndPoint );

      //Show smart path if present
      if( mSmartPath.count() > 1 ) {
        ctx->setPen( mProp.mWidth, sdEnvir->getSysColor(scSmart), dltSolid );
        for( int i = 1; i < mSmartPath.count(); i++ )
          ctx->line( mSmartPath.at(i-1), mSmartPath.at(i) );
        }
      }

    //Draw catch point if present
    int catchSize = mEditor->gridGet().x() * 2;
    int w1_2 = mProp.mWidth.getValue() * 12 / 20;
    int w_diag = static_cast<int>( sqrt( static_cast<double>(w1_2) * static_cast<double>(w1_2) * 2.0 )  );
    switch( mCatch ) {
      case catchFinish :
        ctx->setPen( 0, sdEnvir->getSysColor(scCatchPoint), dltDashed );
        //ctx->smartPoint( mLast, snapCenter );
        break;
      case catchOrthoX :
        //Draw 3x grid line
        ctx->setPen( 0, sdEnvir->getSysColor(scCatchPoint), dltDashed );
        ctx->line( SdPoint( mLast.x() - catchSize, mLast.y() + w1_2 ), SdPoint( mLast.x() + catchSize, mLast.y() + w1_2 ) );
        ctx->line( SdPoint( mLast.x() - catchSize, mLast.y() - w1_2 ), SdPoint( mLast.x() + catchSize, mLast.y() - w1_2 ) );
        break;
      case catchOrthoY :
        //Draw 3x grid line
        ctx->setPen( 0, sdEnvir->getSysColor(scCatchPoint), dltDashed );
        ctx->line( SdPoint( mLast.x() + w1_2, mLast.y() - catchSize ), SdPoint( mLast.x() + w1_2, mLast.y() + catchSize ) );
        ctx->line( SdPoint( mLast.x() - w1_2, mLast.y() - catchSize ), SdPoint( mLast.x() - w1_2, mLast.y() + catchSize ) );
        break;
      case catchDiagQuad1 :
        ctx->setPen( 0, sdEnvir->getSysColor(scCatchPoint), dltDashed );
        ctx->line( SdPoint( mLast.x() - catchSize, mLast.y() - catchSize + w_diag ), SdPoint( mLast.x() + catchSize, mLast.y() + catchSize + w_diag ) );
        ctx->line( SdPoint( mLast.x() - catchSize, mLast.y() - catchSize - w_diag ), SdPoint( mLast.x() + catchSize, mLast.y() + catchSize - w_diag ) );
        break;
      case catchDiagQuad2 :
        ctx->setPen( 0, sdEnvir->getSysColor(scCatchPoint), dltDashed );
        ctx->line( SdPoint( mLast.x() + catchSize, mLast.y() - catchSize + w_diag ), SdPoint( mLast.x() - catchSize, mLast.y() + catchSize + w_diag ) );
        ctx->line( SdPoint( mLast.x() + catchSize, mLast.y() - catchSize - w_diag ), SdPoint( mLast.x() - catchSize, mLast.y() + catchSize - w_diag ) );
        break;
      case catchNone :
        break;
      }

    //Overdraw loop path
    if( mLoopPath.count() ) {
      ctx->setOverColor( sdEnvir->getSysColor(scSelected) );
      mLoopPath.draw( ctx );
      }
    }
  else {
    //When start enter smart point is nearest unconnected pin
    if( sdEnvir->mIsWireSmart )
      ctx->smartPoint( mFirst, snapEndPoint );
    }
  }





int SdModeCRoadEnter::getPropBarId() const
  {
  return PB_ROAD;
  }





void SdModeCRoadEnter::propGetFromBar()
  {
  SdPropBarRoad *bar = dynamic_cast<SdPropBarRoad*>( SdWCommand::mbarTable[PB_ROAD] );
  if( bar ) {
    bar->getPropRoad( &(sdGlobalProp->mRoadProp), &mViaProp, &(sdGlobalProp->mWireEnterType) );
    sdGlobalProp->mViaProp = mViaProp;
    if( getStep() && sdGlobalProp->mRoadProp.mStratum != mProp.mStratum ) {
      //Change stratum
      changeToTraceLayer( sdGlobalProp->mRoadProp.mStratum );
      mProp = sdGlobalProp->mRoadProp;
      mEditor->setFocus();
      setDirtyCashe();
      }
    else {
      mProp = sdGlobalProp->mRoadProp;
      mEditor->setFocus();
      setDirtyCashe();
      }
    //Rebuild all barriers
    if( getStep() )
      rebuildBarriers();
    update();
    }
  }




void SdModeCRoadEnter::propSetToBar()
  {
  SdPropBarRoad *bar = dynamic_cast<SdPropBarRoad*>( SdWCommand::mbarTable[PB_ROAD] );
  if( bar ) {
    //Setup tracing layer count and trace type
    bar->setPlateAndTrace( plate(), layerTraceRoad );
    bar->setPropRoad( &mProp, &mViaProp, mEditor->getPPM(), sdGlobalProp->mWireEnterType );
    sdGlobalProp->mRoadProp = mProp;
    sdGlobalProp->mViaProp = mViaProp;
    }
  }




void SdModeCRoadEnter::enterPoint(SdPoint p)
  {
  Q_UNUSED(p);
  if( getStep() ) {
    //Entered next road path point
    if( mFirst == mBarMiddle ) {
      //Circle stratum change
      changeTraceLayer();
      }
    else {
      if( mCatch == catchFinish && mBarMiddle == mMiddle && mBarLast == mLast ) {
        //Connection complete. Append both segments and stop enter
        if( mFirst != mMiddle )
          addTrace( new SdGraphTracedRoad( mProp, mFirst, mMiddle ), QObject::tr("Insert trace road") );
        if( mMiddle != mLast )
          addTrace( new SdGraphTracedRoad( mProp, mMiddle, mLast ), QObject::tr("Insert trace road") );
        setStep( sFirstPoint );
        mEnterPath.clear();
        }
      else {
        //Append new segment only if it nonzero
        if( mFirst != mBarMiddle ) {
          addTrace( new SdGraphTracedRoad( mProp, mFirst, mBarMiddle ), QObject::tr("Insert trace road") );
          plate()->setDirtyRatNet();
          mFirst = mBarMiddle;
          rebuildBarriers();
          }
        }
      //After road enter stratum stack will equivalent to road stratum
      mStack = mProp.mStratum;
      //Signal plate to rebuild ratNets
      plate()->setDirtyRatNet();
      }
    }
  else {
    //Entered first road path point
    firstPointEnter( true );
    }
  setDirtyCashe();
  update();
  }




void SdModeCRoadEnter::cancelPoint(SdPoint)
  {
  if( getStep() ) {
    setDirtyCashe();
    setStep( sFirstPoint );
    mEnterPath.clear();
    }
  else cancelMode();
  }





SdPoint SdModeCRoadEnter::enterPrev()
  {
  if( getStep() && mCatch == catchFinish && mBarMiddle == mMiddle && mBarLast == mLast ) {
    //Connection complete. Append both segments and stop enter
    if( mFirst != mMiddle )
      addTrace( new SdGraphTracedRoad( mProp, mFirst, mMiddle ), QObject::tr("Insert trace road") );
    if( mMiddle != mLast )
      addTrace( new SdGraphTracedRoad( mProp, mMiddle, mLast ), QObject::tr("Insert trace road") );
    setStep( sFirstPoint );
    mEnterPath.clear();
    //Signal plate to rebuild ratNets
    plate()->setDirtyRatNet();
    setDirtyCashe();
    setDirty();
    //After road enter stratum stack will equivalent to road stratum
    mStack = mProp.mStratum;
    }
  else if( mSmartPath.count() > 1 ) {
    if( mFirst != mMiddle )
      addTrace( new SdGraphTracedRoad( mProp, mFirst, mMiddle ), QObject::tr("Insert trace road") );
    if( mMiddle != mLast )
      addTrace( new SdGraphTracedRoad( mProp, mMiddle, mLast ), QObject::tr("Insert trace road") );
    for( int i = 1; i < mSmartPath.count(); i++ )
      if( mSmartPath.at(i-1) != mSmartPath.at(i) )
        addTrace( new SdGraphTracedRoad( mProp, mSmartPath.at(i-1), mSmartPath.at(i) ), QObject::tr("Insert trace road") );
    mSmartPath.clear();
    setStep( sFirstPoint );
    mEnterPath.clear();
    //Signal plate to rebuild ratNets
    plate()->setDirtyRatNet();
    setDirtyCashe();
    setDirty();
    }
  return mPrevMove;
  }



//Catch technology. When we move cursor, we test catch point.
//It is point where press left button is optimal case.
void SdModeCRoadEnter::movePoint(SdPoint p)
  {
  if( getStep() == sNextPoint ) {
    mPrevMove = p;
    mLast = p;
    SdPoint grid = mEditor->gridGet();
    //Find destignate point
    calcNextSmartPoint( p );
    if( !mTargetPoint.isFar() ) {
      //Calculate catch
      SdPoint offset = mTargetPoint - p;
      int adx = abs(offset.x());
      int ady = abs(offset.y());
      //Check finish
      if( adx < grid.x() && ady < grid.y() ) {
        mLast = mTargetPoint;
        mCatch = catchFinish;
        }
      //Check orthogonal x axiz
      else if( ady < grid.y() ) {
        mLast.setY( mTargetPoint.y() );
        mCatch = catchOrthoX;
        }
      //Check orthogonal y axiz
      else if( adx < grid.y() ) {
        mLast.setX( mTargetPoint.x() );
        mCatch = catchOrthoY;
        }
      //Check diagonal
      else if( abs( adx - ady ) < qMin(grid.x(),grid.y()) ) {
        //Align through axiz X
        mLast.setX( mTargetPoint.x() - (offset.x() > 0 ? ady : -ady) );
        if( (offset.x() >= 0 && offset.y() >= 0) || (offset.x() < 0 && offset.y() < 0) )
          mCatch = catchDiagQuad1;
        else
          mCatch = catchDiagQuad2;
        }
      else
        mCatch = catchNone;
      }
    mMiddle = calcMiddlePoint( mFirst, mLast, sdGlobalProp->mWireEnterType );
    //Check if current point available
    mBarMiddle = checkRoad( mFirst, mMiddle );
    if( mMiddle == mBarMiddle )
      //Vertex available, check last point
      mBarLast = checkRoad( mMiddle, mLast );
    else
      //Vertex not available. Last point is middle barriered
      mBarLast = mBarMiddle;

    if( mBarLast == mLast )
      //Try build smart path
      buildSmartPath( mLast, mTargetPoint );
    else
      //No smart path available
      mSmartPath.clear();
    }
  else {
    mPrevMove = p;
    calcFirstSmartPoint();
    //Find destignate point
//    calcNextSmartPoint();
//    if( !mTargetPoint.isFar() )
//      //Try build smart path
//      buildSmartPath( mFirst, mTargetPoint );
    }
  update();
  }



QString SdModeCRoadEnter::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of road polyline") : QObject::tr("Select point to start road. Left button '%1' net.").arg( mProp.mNetName.str() );
  }



QString SdModeCRoadEnter::getModeThema() const
  {
  return  QString( MODE_HELP "ModeCRoadEnter.htm" );
  }




QString SdModeCRoadEnter::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCRoadEnter.htm#nextPoint" ) : QString( MODE_HELP "ModeCRoadEnter.htm#firstPoint" );
  }



int SdModeCRoadEnter::getCursor() const
  {
  return CUR_WIRE;
  }



int SdModeCRoadEnter::getIndex() const
  {
  return MD_ROAD_ENTER;
  }




void SdModeCRoadEnter::keyDown(int key, QChar ch)
  {
  if( key == Qt::Key_Backspace ) {
    //Remove last segment
    if( mEnterPath.count() ) {
      SdPtr<SdGraphTracedRoad> road( mEnterPath.last() );
      if( road.isValid() ) {
        mFirst = road->segment().getP1();
        //mStack = mViaProp.mStratum;
        //mProp.mStratum = mStack.stratumNext(mProp.mStratum);
        }
      else {
        SdPtr<SdGraphTracedVia> via( mEnterPath.last() );
        if( via.isValid() ) {
          mFirst = via->position();
          }
        }
      mEnterPath.last()->deleteObject( mUndo );
      mEnterPath.removeLast();
      if( mEnterPath.count() ) {
        //Get out properties of segment or via
        SdPtr<SdGraphTracedRoad> road( mEnterPath.last() );
        if( road.isValid() ) {
          mStack = road->stratum();
          mProp.mStratum = mStack;
          }
        else {
          SdPtr<SdGraphTracedVia> via( mEnterPath.last() );
          if( via.isValid() ) {
            mStack = via->stratum().getValue() & plate()->stratumMask();
            }
          }
        propSetToBar();
        }
      setDirty();
      setDirtyCashe();
      plate()->setDirtyRatNet();
      rebuildBarriers();
      movePoint( mPrevMove );
      }
    }
  else if( key == Qt::Key_F6 ) {
    //Change layer. If nessesery - add via
    if( getStep() == sNextPoint ) {
      changeTraceLayer();
      setDirtyCashe();
      update();
      }
    }
  SdModeCommon::keyDown( key, ch );
  }



void SdModeCRoadEnter::getNetOnPoint(SdPoint p, SdStratum s, QString *netName, int *destStratum)
  {
  //Clear net name and stratum
  netName->clear();
  *destStratum = 0;
  plate()->forEach( dctTraced, [p,s,netName,destStratum] (SdObject *obj) -> bool {
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
    if( traced != nullptr ) {
      traced->isPointOnNet( p, s, netName, destStratum );
      }
    return true;
    });
  }





//If point is on middle of same road segment we split it on point
void SdModeCRoadEnter::splitRoadSegment(SdPoint p, SdStratum s, QString *netName, int *destStratum)
  {
  SdUndo *undo = mUndo;
  plate()->forEach( dctTraceRoad, [p,s,netName,destStratum,undo] (SdObject *obj) -> bool {
    SdPtr<SdGraphTracedRoad> road(obj);
    if( road.isValid() && road->isMatchNetAndStratum( *netName, s ) && road->isPointOnNet( p, s, netName, destStratum ) ) {
      //Segment with point found
      road->splitRoad( p, undo );
      //qDebug() << "Road splitted at" << p;
      return false;
      }
    return true;
    });
  }






void SdModeCRoadEnter::calcFirstSmartPoint()
  {
  //At first we find on nearest end of nets or pins
  SdSnapInfo info;
  info.mSour = mPrevMove;
  info.mSnapMask = snapNearestNetEnd | snapNearestPin;
  info.mStratum = stmThrough;
  info.scan( plate(), dctTraced );
  double stp = mEditor->gridGet().x();
  if( !info.isFound() || info.mSqDistance > stp * stp  ) {
    //If not found we try find at any point of nearest net
    info.mSqDistance = stp * stp;
    info.mSnapMask   = snapNearestNet;
    info.scan( plate(), dctTraceRoad );
    }
  if( info.isFound() ) {
    mFirst = info.mDest;
    QString netName;
    int destStratum;
    getNetOnPoint( mFirst, mProp.mStratum, &netName, &destStratum );
    if( mProp.mNetName.str() != netName ) {
      mProp.mNetName = netName;
      SdPulsar::sdPulsar->emitSetStatusMessage( getStepHelp() );
      }
    }
  else {
    mFirst = mPrevMove;
    mProp.mNetName.clear();
    }
  }




void SdModeCRoadEnter::calcNextSmartPoint( SdPoint fromPoint )
  {
  //Find nearest dest point
  SdSnapInfo info;
  info.mSour = fromPoint;
  info.mExclude = mSource;
  //info.mSnapMask = snapNearestNetNet | snapNearestNetPin | snapExcludeSour;
  info.mSnapMask = snapNearestNetPin | snapExcludeSour | snapExcludeExcl;
  info.mStratum = mProp.mStratum;
  info.mNetName = mProp.mNetName.str();
  info.scan( plate(), dctTraced );
  double stp = mEditor->gridGet().x();
  if( !info.isFound() || info.mSqDistance > stp * stp  ) {
    //If not found we try find at any point of nearest net
    info.mSqDistance = stp * stp;
    info.mSnapMask   = snapNearestNetNet;
    info.scan( plate(), dctTraceRoad );
    }
  if( info.isFound() ) {
    //Destignation pin
    mTargetPoint = info.mDest;
    //qDebug() << mSmartPoint;
    //Middle point to smartPoint
//    SdPoint delta = mSmartPoint - mMiddle;
//    if( abs(delta.x()) < 1000 || abs(delta.y()) < 1000 ) {
//      //Orthogonal line
//      mSmartMiddle =
//      }
    }
  else
    //Set illegal point as smart
    mTargetPoint = SdPoint::far();

  }




//For all barriers: we check intersection point of line(p1,p2) and
SdPoint SdModeCRoadEnter::checkRoad(SdPoint p1, SdPoint p2) const
  {
  return sdCheckRoadOnBarrierList( mRoads, p1, p2, mProp.mNetName.str() );
  }




//Check if point p is inside any barrier and return true or false if not
bool SdModeCRoadEnter::isBarriersContains(const SdBarrierList &bar, SdPoint p) const
  {
  return sdIsBarrierListContains( bar, mProp.mNetName.str(), p );
  }





//Check if mSmartPath available
bool SdModeCRoadEnter::checkSmartPath() const
  {
  //Check every point pair. If any fail then all test is fail
  for( int i = 1; i < mSmartPath.count(); i++ )
    if( mSmartPath.at(i-1) != mSmartPath.at(i) && checkRoad( mSmartPath.at(i-1), mSmartPath.at(i) ) != mSmartPath.at(i) )
      return false;
  //All path corrected
  return true;
  }





//Check if four vertex path available
bool SdModeCRoadEnter::checkSmartPath4(SdPoint p1, SdPoint p2, SdPoint p3, SdPoint p4)
  {
  mSmartPath.append( p1 );
  if( p1 != p2 )
    mSmartPath.append( p2 );
  if( p2 != p3 )
    mSmartPath.append( p3 );
  if( p3 != p4 )
    mSmartPath.append( p4 );
  return checkSmartPath();
  }





//Try build smart path
void SdModeCRoadEnter::buildSmartPath(SdPoint p1, SdPoint p2)
  {
  mSmartPath.clear();
  if( p1 == p2 )
    return;
  //Offset point p2 referenced to p1
  int dx = p2.x() - p1.x();
  int dy = p2.y() - p1.y();
  //Orthogonal distance between two points p1 and p2
  int adx = abs(dx);
  int ady = abs(dy);
  //First - with one vertex
  SdPoint mp = calcMiddlePoint( p1, p2, sdGlobalProp->mWireEnterType );
  if( checkSmartPath4( p1, mp, p2, p2 ) )
    return;

  //Fail
  mSmartPath.clear();

  if( adx == 0 || ady == 0 )
    return;

  //Try with two vertex
  SdPoint v1,v2;
  for( int i = 0; i < 2; i++ ) {
    int sp = qMin(adx,ady);
    int diag = sp / (3 + i);
    int dsx = dx < 0 ? dx + diag : dx - diag;
    int dsy = dy < 0 ? dy + diag : dy - diag;
    v1 = p1;
    v2 = p2;
    //By X path first
    v1.rx() += dsx;
    v2.ry() -= dsy;
    if( checkSmartPath4( p1, v1, v2, p2 ) )
      return;

    //Fail
    mSmartPath.clear();

    v1 = p1;
    v2 = p2;
    //By Y path first
    v1.ry() += dsy;
    v2.rx() -= dsx;
    if( checkSmartPath4( p1, v1, v2, p2 ) )
      return;

    //Fail
    mSmartPath.clear();

    //By diagonal on middle
    dsx = dx < 0 ? -sp : sp;
    dsy = dy < 0 ? -sp : sp;
    int ortho = (qMax(adx,ady) - sp) / (2 + i);
    v1 = p1;
    if( adx > ady )
      v1.rx() += dx < 0 ? -ortho : ortho;
    else
      v1.ry() += dy < 0 ? -ortho : ortho;
    v2 = v1;
    v2.rx() += dsx;
    v2.ry() += dsy;
    if( checkSmartPath4( p1, v1, v2, p2 ) )
      return;

    //Fail
    mSmartPath.clear();
    }

  }





void SdModeCRoadEnter::addTrace(SdObject *obj , const QString msg)
  {
  addPic( obj, msg );
  SdPtr<SdGraphTracedRoad> road(obj);
  if( road.isValid() )
    road->utilize( mUndo );
  mEnterPath.append( obj );
  }




//Change trace layer
void SdModeCRoadEnter::changeTraceLayer()
  {
  //Circle stratum change
  SdStratum st( plate()->stratumMask() );
  changeToTraceLayer( st.stratumNext( mProp.mStratum ) );
  }




//Change trace layer to desired stratum
void SdModeCRoadEnter::changeToTraceLayer(SdStratum dest)
  {
  //Check if current stack available changing
  if( mStack.match(dest) ) {
    //Start point is on through pin. Therefore we simple change stratum
    qDebug() << "Stratum change without via";
    mProp.mStratum = dest;
    plate()->ruleBlockForNet( mProp.mNetName.str(), mRule );
    mProp.mWidth   = mRule.mRules[ruleRoadWidth];
    }
  else {
    //No stratum stack at this point
    //Try insert Via
    addVia( dest );
    }
  //Update rules
  propSetToBar();
  rebuildBarriers();
  calcNextSmartPoint( mFirst );
  }




//Add via to change trace layer
void SdModeCRoadEnter::addVia(SdStratum newStratum)
  {
  //If not same stratum and via available in this place
  if( mProp.mStratum != newStratum && !isBarriersContains( mPads, mFirst) ) {
    //Via is available at this point - insert
    mViaProp.mNetName = mProp.mNetName;
    qDebug() << "Via inserted" << mViaProp.mPadType.str() << mViaProp.mNetName.str();
    addTrace( new SdGraphTracedVia( mFirst, mViaProp ), QObject::tr("Insert trace via") );
    mMiddle = mFirst;
    mBarMiddle = mFirst;
    mLast = mFirst;
    mBarLast = mFirst;
    //qDebug() << plate()->stratumMask();
    mProp.mStratum = newStratum;
    //plate()->ruleBlockForNet( mProp.mStratum.getValue(), mProp.mNetName.str(), mRule );
    //mProp.mWidth   = mRule.mRules[ruleRoadWidth];
    }
  }






//First point enter
void SdModeCRoadEnter::firstPointEnter(bool enter)
  {
  //Find whose net it contained
  //At first find on current stratum
  QString netName;
  int destStratum;
  getNetOnPoint( mFirst, mProp.mStratum, &netName, &destStratum );

  if( netName.isEmpty() )
    //No net on this point at current stratum
    //Try on all stratums
    getNetOnPoint( mFirst, stmThrough, &netName, &destStratum );

  if( !netName.isEmpty() ) {
    mSource = mFirst;
    mMiddle = mFirst;
    mBarMiddle = mFirst;
    mLast = mFirst;
    mBarLast = mFirst;
    mStack = destStratum & plate()->stratumMask();
    mProp.mNetName = netName;
    mProp.mStratum = mStack.stratumFirst(mProp.mStratum);
    plate()->ruleBlockForNet( mProp.mNetName.str(), mRule );
    mProp.mWidth   = mRule.mRules[ruleRoadWidth];
    propSetToBar();
    if( enter ) {
      //If mFirst is on road segment we split road
      splitRoadSegment( mFirst, mProp.mStratum, &netName, &destStratum );
      setStep(sNextPoint);
      mLoopPath.clear();
      }
    rebuildBarriers();
    //Find destignate point
    calcNextSmartPoint( mFirst );
    if( !mTargetPoint.isFar() )
      //Try build smart path
      buildSmartPath( mFirst, mTargetPoint );
    }
  }





//Find loop. If found it will be placed to mLoopPath
void SdModeCRoadEnter::findLoop(SdPoint src, SdPoint dst, SdStratum st)
  {

  }





void SdModeCRoadEnter::rebuildBarriers()
  {
  //Accum barriers for current segment
  mRoads.clear();
  //Update width in rule block to current from prop
  mRule.mRules[ruleRoadWidth] = mProp.mWidth.getValue();
  plate()->accumBarriers( dctTraced, mRoads, mProp.mStratum, ruleRoadRoad, mRule );

  //Accum barriers for via
  mPads.clear();
  //Update width in rule block to current from prop
  int r = plate()->getPadOverRadius( mViaProp.mPadType.str() );
  if( r > 0 )
    mRule.mRules[ruleRoadWidth] = r;
  plate()->accumBarriers( dctTraced, mPads, mViaProp.mStratum, rulePadPad, mRule );
  }



//====================================================================================================================
//   Loop detector
struct SdLoopEdge {
    QList<SdGraphTracedPtr> mPath;
  };

typedef SdLoopEdge *SdLoopEdgePtr;

struct SdLoopNode {

  };
