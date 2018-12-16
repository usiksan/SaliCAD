/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for move plate trace road
*/
#include "SdModeCRoadMove.h"
#include "objects/SdSnapInfo.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphTracedRoad.h"
#include "objects/SdGraphTracedVia.h"
#include "windows/SdWEditorGraph.h"

#include <QDebug>

SdModeCRoadMove::SdModeCRoadMove(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }



void SdModeCRoadMove::reset()
  {
  setDirtyCashe();
  setStep( sFindRoad );
  }




void SdModeCRoadMove::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mProp.mStratum, QString() );
  }




void SdModeCRoadMove::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sFindRoad ) {
    //Draw source point if present
    if( mSourceType )
      ctx->smartPoint( mSourcePoint, mSourceType );

    if( mFragment.count() ) {
      //Draw selected fragment
      ctx->setOverColor( sdEnvir->getSysColor(scSelected) );
      mFragment.draw( ctx );
      }
    }
  if( getStep() == sMoveRoad ) {
    //Draw segments
    ctx->setOverColor( sdEnvir->getSysColor(scSelected) );
    if( mSource1 != mMove1 ) {
      ctx->setPen( mProp1.mWidth.getValue(), sdEnvir->mCacheForRoad.getVisibleLayer( mProp1.mStratum ), dltSolid );
      ctx->line( mSource1, mMove1 );
      }
    if( mMove1 != mMove2 ) {
      ctx->setPen( mProp.mWidth.getValue(), sdEnvir->mCacheForRoad.getVisibleLayer( mProp.mStratum ), dltSolid );
      ctx->line( mMove1, mMove2 );
      }
    if( mMove2 != mSource2 ) {
      ctx->setPen( mProp2.mWidth.getValue(), sdEnvir->mCacheForRoad.getVisibleLayer( mProp2.mStratum ), dltSolid );
      ctx->line( mSource2, mMove2 );
      }
    }

  }





void SdModeCRoadMove::enterPoint(SdPoint)
  {
  if( mSourceType ) {
    //Select source object
    mSourceObject->selectByPoint( mSourcePoint, &mFragment );
    if( mSourceType == snapMidPoint ) {
      //Used 3 segments
      mSegment = mSegment1 = mSegment2 = nullptr;
      mVia = nullptr;
      SdPtr<SdGraphTracedRoad> road(mSourceObject);
      if( road.isValid() ) {
        mSegment = road.ptr();
        mMove = mSourcePoint;
        mMove1 = road->segment().getP1();
        mMove2 = road->segment().getP2();
        mProp  = road->propRoad();
        SdSelector sel;
        mSegment->accumLinkedTrace( mSegment, mMove1, mProp.mNetName.str(), &sel );
        if( sel.count() == 1 && sel.first()->getClass() == dctTraceRoad ) {
          //First segment found
          mSegment1 = dynamic_cast<SdGraphTracedRoad*>(sel.first());
          mProp1 = mSegment1->propRoad();
          if( mSegment1->segment().getP1() == mMove1 )
            mSource1 = mSegment1->segment().getP2();
          else
            mSource1 = mSegment1->segment().getP1();
          mFragment.insert( mSegment1 );
          }
        else {
          mSegment1 = nullptr;
          mSource1 = mMove1;
          mProp1 = mProp;
          }

        sel.removeAll();
        mSegment->accumLinkedTrace( mSegment, mMove2, mProp.mNetName.str(), &sel );
        if( sel.count() == 1 && sel.first()->getClass() == dctTraceRoad ) {
          //Second segment found
          mSegment2 = dynamic_cast<SdGraphTracedRoad*>(sel.first());
          mProp2 = mSegment2->propRoad();
          if( mSegment2->segment().getP1() == mMove2 )
            mSource2 = mSegment2->segment().getP2();
          else
            mSource2 = mSegment2->segment().getP1();
          mFragment.insert( mSegment2 );
          }
        else {
          mSegment2 = nullptr;
          mSource2 = mMove2;
          mProp2 = mProp;
          }
        }
      else {
        mFragment.removeAll();
        mSourceType = 0;
        }
      }
    else if( mSourceType == snapEndPoint ) {
      //Used 2 segments
      mSegment = mSegment1 = mSegment2 = nullptr;
      mVia = nullptr;
      SdPtr<SdGraphTracedRoad> road(mSourceObject);
      if( road.isValid() ) {
        mSegment = nullptr;
        mMove = mSourcePoint;
        mMove1 = mMove;
        mMove2 = mMove;
        mProp  = road->propRoad();

        //Selected segment is first
        mSegment1 = road.ptr();
        if( mSegment1->segment().getP1() == mSourcePoint )
          mSource1 = mSegment1->segment().getP2();
        else
          mSource1 = mSegment1->segment().getP1();
        mProp1 = mProp;

        SdSelector sel;
        mSegment1->accumLinkedTrace( mSegment1, mMove1, mProp.mNetName.str(), &sel );
        if( sel.count() == 1 && sel.first()->getClass() == dctTraceRoad ) {
          //Second segment found
          mSegment2 = dynamic_cast<SdGraphTracedRoad*>(sel.first());
          mProp2 = mSegment2->propRoad();
          if( mSegment2->segment().getP1() == mMove1 )
            mSource2 = mSegment2->segment().getP2();
          else
            mSource2 = mSegment2->segment().getP1();
          mFragment.insert( mSegment2 );
          }
        else {
          mSegment2 = nullptr;
          mSource2 = mMove2;
          mProp2 = mProp;
          }
        }
      else {
        mFragment.removeAll();
        mSourceType = 0;
        }
      }
    else if( mSourceType == snapViaPoint ) {
      //Used via and 2 or less segments
      mSegment = mSegment1 = mSegment2 = nullptr;
      mVia = nullptr;
      mFragment.removeAll();
      mSourceType = 0;
      }
    setDirtyCashe();
    update();
    }
  }





void SdModeCRoadMove::movePoint(SdPoint p)
  {
  if( getStep() == sFindRoad ) {
    if( mFragment.count() ) {
      mFragment.removeAll();
      setDirtyCashe();
      }
    //Find nearest smart point
    SdSnapInfo info;
    info.mSour = p;
    info.mSnapMask = snapEndPoint | snapMidPoint | snapViaPoint;
    info.mStratum = stmThrough;
    info.scan( plate(), dctTraceVia | dctTraceRoad );
    mSourceType = info.mDestMask;
    mSourcePoint = info.mDest;
    mSourceObject = info.mGraph;
    update();
    }
  }




void SdModeCRoadMove::cancelPoint(SdPoint)
  {
  cancelMode();
  }




void SdModeCRoadMove::beginDrag(SdPoint p)
  {
  if( mSourceType ) {
    mUndo->begin( QObject::tr("Move begin"), mObject );

    //Save state of all object before moving
    mFragment.forEach( dctAll, [this] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->saveState( mUndo );
      return true;
      });
    //Mutual orientation of segments
    SdOrientation orient2 = SdSegment( mSource2, mMove2 ).orientation();
    switch( SdSegment( mSource1, mMove1 ).orientation() ) {
      case sorNull :
        //For p1 == p2
      case sorAny :
        switch( orient2 ) {
          case sorNull :
            //For p1 == p2
          case sorAny :
            mDirX2 = mDirX1 = 1;
            mDirY2 = mDirY1 = 1;
            qDebug() << "any any";
            break;
          case sorVertical :
            //For p1.x == p2.x
            mDirX1 = mDirX2 = 0;
            mDirY1 = mDirY2 = 1;
            qDebug() << "any vertical";
            break;
          case sorHorizontal :
            //For p1.y == p2.y
            mDirX1 = mDirX2 = mSource1.x() < mSource2.x() ? -1 : 1;
            mDirY1 = mDirY2 = 0;
            qDebug() << "any horizontal";
            break;
          case sorSlashForward :
            //For dx == dy
            mDirX1 = mDirX2 = 1;
            mDirY1 = mDirY2 = 1;
            qDebug() << "*any forward";
            break;
          case sorSlashBackward :
            //For dx == -dy
            mDirX1 = mDirX2 = -1;
            mDirY1 = mDirY2 = 1;
            qDebug() << "any backward";
            break;
          }
        break;
      case sorVertical :
        //For p1.x == p2.x
        mDirX1 = 0;
        mDirY1 = 1;
        switch( orient2 ) {
          case sorNull :
            //For p1 == p2
          case sorAny :
          case sorVertical :
            //For p1.x == p2.x
            mDirX2 = 0;
            mDirY2 = 1;
            qDebug() << "*vertical vertical";
            break;
          case sorHorizontal :
            //For p1.y == p2.y
            mDirX2 = mSource1.x() < mSource2.x() ? -1 : 1;
            mDirY2 = 0;
            qDebug() << "*vertical horizontal";
            break;
          case sorSlashForward :
            //For dx == dy
            mDirX2 = 1;
            mDirY2 = 1;
            qDebug() << "vertical forward";
            break;
          case sorSlashBackward :
            //For dx == -dy
            mDirX2 = -1;
            mDirY2 = 1;
            qDebug() << "vertical backward";
            break;
          }
        break;
      case sorHorizontal :
        //For p1.y == p2.y
        mDirX1 = 1;
        mDirY1 = 0;
        switch( orient2 ) {
          case sorNull :
            //For p1 == p2
          case sorAny :
            mDirY2 = 1;
            mDirX2 = 0;
            qDebug() << "*horizontal any";
            break;
          case sorVertical :
            //For p1.x == p2.x
            mDirX2 = 0;
            mDirY2 = mSource1.y() < mSource2.y() ? 1 : -1;
            qDebug() << "*horizontal vertical";
            break;
          case sorHorizontal :
            //For p1.y == p2.y
            mDirX2 = 1;
            mDirY2 = 0;
            qDebug() << "*horizontal horizontal";
            break;
          case sorSlashForward :
            //For dx == dy
            mDirX2 = 1;
            mDirY2 = 1;
            qDebug() << "horizontal forward";
            break;
          case sorSlashBackward :
            //For dx == -dy
            mDirX2 = -1;
            mDirY2 = 1;
            qDebug() << "horizontal backward";
            break;
          }
        break;
      case sorSlashForward :
        //For dx == dy
        mDirX1 = 1;
        mDirY1 = 1;
        switch( orient2 ) {
          case sorNull :
            //For p1 == p2
          case sorAny :
            mDirX2 = 1;
            mDirY2 = 1;
            qDebug() << "*forward any";
            break;
          case sorVertical :
            //For p1.x == p2.x
            mDirX2 = 0;
            mDirY2 = mSource1.y() < mSource2.y() ? -1 : 1;
            qDebug() << "forward vertical";
            break;
          case sorHorizontal :
            //For p1.y == p2.y
            mDirX2 = mSource1.x() < mSource2.x() ? -1 : 1;
            mDirY2 = 0;
            qDebug() << "forward horizontal";
            break;
          case sorSlashForward :
            //For dx == dy
            mDirX2 = 1;
            mDirY2 = 1;
            qDebug() << "forward forward";
            break;
          case sorSlashBackward :
            //For dx == -dy
            mDirX2 = 1;
            mDirY2 = -1;
            qDebug() << "forward backward";
            break;
          }
        break;
      case sorSlashBackward :
        //For dx == -dy
        mDirX1 = -1;
        mDirY1 = 1;
        switch( orient2 ) {
          case sorNull :
            //For p1 == p2
          case sorAny :
            mDirX2 = -1;
            mDirY2 = 1;
            qDebug() << "backward any";
            break;
          case sorVertical :
            //For p1.x == p2.x
            mDirX2 = 0;
            mDirY2 = mSource1.y() < mSource2.y() ? -1 : 1;
            qDebug() << "backward vertical";
            break;
          case sorHorizontal :
            //For p1.y == p2.y
            mDirX2 = mSource1.x() < mSource2.x() ? -1 : 1;
            mDirY2 = 0;
            qDebug() << "backward horizontal";
            break;
          case sorSlashForward :
            //For dx == dy
            mDirX2 = 1;
            mDirY2 = 1;
            qDebug() << "*backward forward";
            break;
          case sorSlashBackward :
            //For dx == -dy
            mDirX2 = -1;
            mDirY2 = 1;
            qDebug() << "*backward backward";
            break;
          }
        break;
      }
    mPrevMove = p;
    setStep( sMoveRoad );
    setDirty();
    setDirtyCashe();
    update();
    }
  }




void SdModeCRoadMove::dragPoint(SdPoint p)
  {
  SdPoint offset = p.sub(mPrevMove);
  int d;
  if( qAbs(offset.x()) > qAbs(offset.y()) )
    d = offset.x();
  else
    d = offset.y();
  mMove1.move( SdPoint(d*mDirX1,d*mDirY1) );
  mMove2.move( SdPoint(d*mDirX2,d*mDirY2) );
//  mFragment.forEach( dctAll, [offset] (SdObject *obj) ->bool {
//    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
//    if( graph != nullptr )
//      graph->move( offset );
//    return true;
//    });
  mPrevMove.move( offset );
  update();
  }




void SdModeCRoadMove::stopDrag(SdPoint p)
  {
  dragPoint(p);
  mFragment.removeAll();
  setStep(sFindRoad);
  setDirty();
  setDirtyCashe();
  update();
  }




QString SdModeCRoadMove::getStepHelp() const
  {
  return getStep() == sMoveRoad ? QObject::tr("Move road to new position") : QObject::tr("Select road segment to move");
  }




QString SdModeCRoadMove::getModeThema() const
  {
  return  QString( MODE_HELP "ModeCRoadMove.htm" );
  }




QString SdModeCRoadMove::getStepThema() const
  {
  return getStep() == sMoveRoad ? QString( MODE_HELP "ModeCRoadMove.htm#moveRoad" ) : QString( MODE_HELP "ModeCRoadMove.htm#selectRoad" );
  }







int SdModeCRoadMove::getCursor() const
  {
  return CUR_WMOVE;
  }




int SdModeCRoadMove::getIndex() const
  {
  return MD_ROAD_MOVE;
  }
