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
#include <algorithm>

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
    if( mVia == nullptr ) {
      //Move segments only
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
    else {
      //Move via
      mFragment.draw( ctx );
      if( mSource1 != mMove1 ) {
        ctx->setPen( mProp.mWidth.getValue(), sdEnvir->mCacheForRoad.getVisibleLayer( mViaProp.mStratum ), dltSolid );
        ctx->line( mSource1, mMove1 );
        }
//      if( mMove1 != mMove2 ) {
//        ctx->setPen( mProp.mWidth.getValue(), sdEnvir->mCacheForRoad.getVisibleLayer( mViaProp.mStratum ), dltSolid );
//        ctx->line( mMove1, mMove2 );
//        }
      }
    }
  }





void SdModeCRoadMove::enterPoint(SdPoint)
  {
  //Preliminary disable move sources
  mMoveSource1 = mMoveSource2 = false;
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
        mMove1 = mSourcePoint;
        mMove2 = mMove1;
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
          mMoveSource2 = true;
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
      SdPtr<SdGraphTracedVia> via(mSourceObject);
      if( via.isValid() ) {
        mVia = via.ptr();
        mViaProp = mVia->propVia();
        mMove1 = mVia->position();
        mSource1 = mMove1;
        mMove2 = mMove1;
        mSource2 = mMove2;
        //Get rules for net
        plate()->ruleBlockForNet( mViaProp.mNetName.str(), mRule );

        //Prepare prop for building roads
        mProp.mWidth   = mRule.mRules[ruleRoadWidth];

        //Barriers for via
        SdPad viaPad = plate()->getPad( mViaProp.mPadType.str() );
        mPads.clear();
        //We need via pad diameter
        mRule.mRules[ruleRoadWidth] = viaPad.overCircleRadius() * 2;
        plate()->accumBarriers( dctTraced, mPads, mViaProp.mStratum, rulePadPad, mRule );

        //Accumulate roads connected to via
        mVia->accumLinkedTrace( mVia, mMove1, mViaProp.mNetName.str(), &mFragment );
        int stratum = 0;
        mFragment.forEach( dctTraceRoad, [&stratum] (SdObject *obj) ->bool {
          SdPtr<SdGraphTracedRoad> road(obj);
          if( road.isValid() )
            stratum |= road->stratum().getValue();
          return true;
          });

        //Prepare prop for building roads
        mProp.mNetName = mViaProp.mNetName;
        mProp.mStratum = stratum;

        mProp1 = mProp;
        mProp2 = mProp;
        }
      else {
        mVia = nullptr;
        mFragment.removeAll();
        mSourceType = 0;
        }
      }

    //Build barriers
    //Get rules for road
    plate()->ruleBlockForNet( mProp.mNetName.str(), mRule );

    //Barriers for central segment
    mRoads.clear();
    mRule.mRules[ruleRoadWidth] = mProp.mWidth.getValue();
    plate()->accumBarriers( dctTraced, mRoads, mProp.mStratum, ruleRoadRoad, mRule );

    //Rebuild barriers for segment 1
    mRoads1.clear();
    mRule.mRules[ruleRoadWidth] = mProp1.mWidth.getValue();
    plate()->accumBarriers( dctTraced, mRoads1, mProp1.mStratum, ruleRoadRoad, mRule );

    //Rebuild barriers for segment 2
    mRoads2.clear();
    mRule.mRules[ruleRoadWidth] = mProp2.mWidth.getValue();
    plate()->accumBarriers( dctTraced, mRoads2, mProp2.mStratum, ruleRoadRoad, mRule );


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
      if( mSegment ) mSegment->utilize( mUndo );
      if( mSegment1 ) mSegment1->utilize( mUndo );
      if( mSegment2 ) mSegment2->utilize( mUndo );
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
    if( mVia == nullptr ) {
      //Move segments
      //Mutual orientation of segments
      SdOrientation orient1 = SdSegment( mSource1, mMove1 ).orientation();
      SdOrientation orient2 = SdSegment( mSource2, mMove2 ).orientation();
      SdOrientation orient  = SdSegment( mMove1, mMove2 ).orientation();
      if( (orient2 == sorVertical ) ||
          (orient1 == sorAny) ||
          (orient1 == sorNull) ||
          (orient1 == sorHorizontal && orient2 == sorVertical) ||
          (orient1 == sorSlashForward && orient2 == sorVertical) ||
          (orient1 == sorSlashForward && orient2 == sorHorizontal) ||
          (orient1 == sorSlashBackward) ) {
        //Change segment1 and segment2
        std::swap( mSegment1, mSegment2 );
        std::swap( mSource1, mSource2 );
        std::swap( mMove1, mMove2 );
        std::swap( orient1, orient2 );
        }
      switch( orient1 ) {
        case sorNull :
          //For p1 == p2
        case sorAny :
          //Second direction also is null or any
          //Make orthogonal segments
          switch( orient ) {
            case sorNull :
              //For p1 == p2
            case sorAny :
              //Make orthogonal segments
              //This point move it horizontal
            case sorVertical :
              mDirX1 = mDirX2 = 1;
              mDirY1 = mDirY2 = 0;
              break;
            case sorHorizontal :
              //Make vertical segments
              mDirX1 = mDirX2 = 0;
              mDirY1 = mDirY2 = 1;
              break;
            case sorSlashForward :
              //Make backward segments
              mDirX1 = mDirX2 = 1;
              mDirY1 = mDirY2 = -1;
              break;
            case sorSlashBackward :
              //Make forward segments
              mDirX1 = mDirX2 = 1;
              mDirY1 = mDirY2 = 1;
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
              if( mMove1.y() > mMove2.y() || (mMove1.y() == mMove2.y() && mSource1.y() > mMove1.y()) ) {
                //up
                if( mMove2.x() < mMove1.x() || (mMove2.x() == mMove1.x() && mSource2.x() < mMove2.x() ) ) {
                  //left
                  mDirX2 = -1;
                  mDirY2 = 0;
                  qDebug() << "left up";
                  }
                else {
                  //right
                  mDirX2 = 1;
                  mDirY2 = 0;
                  qDebug() << "right up";
                  }
                }
              else {
                //down
                if( mMove2.x() < mMove1.x() || (mMove2.x() == mMove1.x() && mSource2.x() < mMove2.x() ) ) {
                  //left
                  mDirX2 = 1;
                  mDirY2 = 0;
                  qDebug() << "left down";
                  }
                else {
                  //right
                  mDirX2 = -1;
                  mDirY2 = 0;
                  qDebug() << "right down";
                  }
                }
              qDebug() << "*vertical horizontal" << mMove1 << mMove2;
              break;


            case sorSlashForward :
              //For dx == dy
              if( mMove1.y() == mMove2.y() ) {
                //In this configuration we move only horizontal segments
                mDirX2 = 1;
                mDirY2 = 1;
                if( mMove1 == mMove2 ) {
                  mFragment.remove( mSegment1 );
                  mSegment1 = nullptr;
                  mSource1 = mMove1;
                  mDirX1 = 1;
                  mDirY1 = 0;
                  }
                }
              else {
                //Moving disable
                mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
                }
              qDebug() << "vertical forward";
              break;


            case sorSlashBackward :
              //For dx == -dy
              if( mMove1.y() == mMove2.y() ) {
                mDirX2 = -1;
                mDirY2 = 1;
                if( mMove1 == mMove2 ) {
                  mFragment.remove( mSegment1 );
                  mSegment1 = nullptr;
                  mSource1 = mMove1;
                  mDirX1 = -1;
                  mDirY1 = 0;
                  }
                }
              else {
                //Moving disable
                mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
                }
              qDebug() << "vertical backward";
              break;
            }
          break;






        case sorHorizontal :
          //For p1.y == p2.y
          mDirX1 = 1;
          mDirY1 = 0;
          switch( orient2 ) {
            case sorVertical :
              mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
              qDebug() << "horizontal vertical not available";
              break;
            case sorNull :
              //For p1 == p2
            case sorAny :
            case sorHorizontal :
              //For p1.y == p2.y
              mDirX2 = 1;
              mDirY2 = 0;
              qDebug() << "*horizontal horizontal";
              break;


            case sorSlashForward :
              //For dx == dy
              if( mMove1.x() == mMove2.x() ) {
                //In this configuration we move only vertical segments
                mDirX2 = 1;
                mDirY2 = 1;
                if( mMove1 == mMove2 ) {
                  mFragment.remove( mSegment1 );
                  mSegment1 = nullptr;
                  mSource1 = mMove1;
                  mDirX1 = 0;
                  mDirY1 = 1;
                  }
                }
              else {
                //Moving disable
                mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
                }
              qDebug() << "horizontal forward";
              break;


            case sorSlashBackward :
              //For dx == -dy
              if( mMove1.x() == mMove2.x() ) {
                //In this configuration we move only vertical segments
                mDirX2 = 1;
                mDirY2 = -1;
                if( mMove1 == mMove2 ) {
                  mFragment.remove( mSegment1 );
                  mSegment1 = nullptr;
                  mSource1 = mMove1;
                  mDirX1 = 0;
                  mDirY1 = -1;
                  }
                }
              else {
                //Moving disable
                mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
                }
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
              //Make orthogonal segments
              switch( orient ) {
                case sorNull :
                  //For p1 == p2
                case sorAny :
                  //Make orthogonal segments
                  //This point move it horizontal
                  mDirX2 = 1;
                  mDirY2 = 1;
                  break;
                case sorVertical :
                  //Make horizontal segments
                  mDirX2 = 1;
                  mDirY2 = 0;
                  break;
                case sorHorizontal :
                  //Make vertical segments
                  mDirX2 = 0;
                  mDirY2 = 1;
                  break;
                case sorSlashForward :
                  //Make backward segments
                  mDirX2 = 1;
                  mDirY2 = -1;
                  break;
                case sorSlashBackward :
                  //Make forward segments
                  mDirX2 = 1;
                  mDirY2 = -1;
                  break;
                }
              break;
              qDebug() << "*forward any";
              break;


            case sorVertical :
              //For p1.x == p2.x
              mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
              qDebug() << "forward vertical not available";
              break;


            case sorHorizontal :
              //For p1.y == p2.y
              mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
              qDebug() << "forward horizontal not available";
              break;


            case sorSlashForward :
              //For dx == dy
              mDirX2 = 1;
              mDirY2 = 1;
              qDebug() << "forward forward";
              break;


            case sorSlashBackward : {
              //For dx == -dy
              //y = (x2 +y2 + y1 - x1) / 2
              //x = y - y1 + x1
              int y = (mSource2.x() + mSource2.y() + mSource1.y() - mSource1.x()) / 2;
              //int x = y - mSource1.y() + mSource1.x();
              // \1/
              // 4+2
              // /3\ .
              if( mMove1.y() > y || (mMove1.y() == y && mSource1.y() > y) ) {
                //1 or 2
                if( mMove2.y() > y || (mMove2.y() == y && mSource2.y() > y) ) {
                  //1
                  mDirX2 = -1;
                  mDirY2 = 1;
                  }
                else {
                  //2
                  mDirX2 = 1;
                  mDirY2 = -1;
                  }
                }
              else {
                //3 or 4
                if( mMove2.y() > y || (mMove2.y() == y && mSource2.y() > y) ) {
                  //4
                  mDirX2 = 1;
                  mDirY2 = -1;
                  }
                else {
                  //2
                  mDirX2 = -1;
                  mDirY2 = 1;
                  }
                }
              qDebug() << "forward backward";
              }
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
              mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
              qDebug() << "backward vertical not available";
              break;
            case sorHorizontal :
              //For p1.y == p2.y
              mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
              qDebug() << "backward horizontal not available";
              break;
            case sorSlashForward :
              //For dx == dy
              mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
              qDebug() << "backward forward not available";
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
      }
    else {
      //Move via
      //mSource1 = mVia->position();
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
  if( mVia ) {
    //Move via
    SdPoint offset = p.sub(mMove2);
    SdPoint dest   = p.sub(mSource1);
    int dx = qAbs(dest.x());
    int dy = qAbs(dest.y());
    if( dx > dy * 2 )
      //Horizontal
      dest.setY(0);

    else if( dy > dx * 2 )
      //Vertical
      dest.setX(0);

    else {
      //Diagonal
      if( dx > dy )
        dest.setY( dest.y() < 0 ? -dx : dx );
      else
        dest.setX( dest.x() < 0 ? -dy : dy );
      }

    dest.move( mSource1 );
    if( sdCheckRoadOnBarrierList( mRoads, mSource1, dest, mProp.mNetName.str() ) == dest &&
        !sdIsBarrierListContains( mPads, mViaProp.mNetName.str(), dest ) ) {
      //New point available
      offset = dest.sub( mVia->position() );
      mVia->move(offset);
      mMove1 = dest;
      mMove2 = dest;
      }
    }
  else {
    //Move segments only
    SdPoint offset = p.sub(mPrevMove);
    //Smart points
    SdPoint sm1 = mSource1.sub(mMove1);
    SdPoint sm2 = mSource2.sub(mMove2);
    bool link = false;
    //SdPoint sm3 =
    int sd = qMax(qAbs(sm1.x()),qAbs(sm1.y()));
    int d;
    if( qAbs(offset.x()) > qAbs(offset.y()) )
      d = offset.x();
    else
      d = offset.y();
    if( sd < 200 && qAbs(d) < 200 ) {
      //Link to sm1
      link = true;
      if( qAbs(sm1.x()) > qAbs(sm1.y()) )
        d = sm1.x() * mDirX1;
      else
        d = sm1.y() * mDirY1;
      }
    else {
      sd = qMax(qAbs(sm2.x()),qAbs(sm2.y()));
      if( !mMoveSource2 && sd < 200 && qAbs(d) < 200 ) {
        //Link to sm2
        link = true;
        if( qAbs(sm2.x()) > qAbs(sm2.y()) )
          d = sm2.x() * mDirX2;
        else
          d = sm2.y() * mDirY2;
        }
      }
    //Try moving
    SdPoint move1(mMove1);
    SdPoint move2(mMove2);
    move1.move( SdPoint(d*mDirX1,d*mDirY1) );
    move2.move( SdPoint(d*mDirX2,d*mDirY2) );
    SdPoint source2(mSource2);
    if( mMoveSource2 )
      source2.move( SdPoint(d*mDirX2,d*mDirY2) );


    //Check if new position available
    if( sdCheckRoadOnBarrierList( mRoads1, mSource1, move1, mProp.mNetName.str() ) == move1 &&
        sdCheckRoadOnBarrierList( mRoads, move1, move2, mProp.mNetName.str() ) == move2 &&
        sdCheckRoadOnBarrierList( mRoads2, source2, move2, mProp.mNetName.str() ) == move2 ) {
      //Fact moving
      mMove1 = move1;
      mMove2 = move2;
      mSource2 = source2;
      if( !link )
        mPrevMove.move( offset );
      }
    }

  update();
  }




void SdModeCRoadMove::stopDrag(SdPoint p)
  {
  dragPoint(p);
  mFragment.removeAll();
  if( mVia ) {
    //Move via
    //Append segments to new via position for each stratum in mProp
    mProp1.mStratum = mProp.mStratum.stratumFirst( mProp1.mStratum );
    mProp2.mStratum = mProp1.mStratum;
    do {
      qDebug() << "appended segment" << mSource1 << mMove1 << "and" << mMove2;
      qDebug() << "at stratum" << mProp1.mStratum.getValue();
      SdGraphTracedRoad *road1 = nullptr;
      updateSegment( mProp1, road1, mSource1, mMove1 );
      SdGraphTracedRoad *road2 = nullptr;
      updateSegment( mProp1, road2, mMove1, mMove2 );
      if( road1 ) road1->utilize( mUndo );
      if( road2 ) road2->utilize( mUndo );
      mProp1.mStratum = mProp.mStratum.stratumNext(mProp1.mStratum);
      }
    while( mProp2.mStratum != mProp1.mStratum );
    }
  else {
    //Move segments only
    updateSegment( mProp1, mSegment1, mSource1, mMove1 );
    updateSegment( mProp2, mSegment2, mSource2, mMove2 );
    updateSegment( mProp,  mSegment,  mMove1, mMove2 );
    if( mSegment1 ) mSegment1->utilize( mUndo );
    if( mSegment2 ) mSegment2->utilize( mUndo );
    if( mSegment ) mSegment->utilize( mUndo );
    }
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



//Update segment. We create or delete segment if nessesery and change its position
void SdModeCRoadMove::updateSegment(SdPropRoad &prop, SdGraphTracedRoadPtr &segment, SdPoint a, SdPoint b)
  {
  if( segment ) {
    if( a == b ) {
      //Segments must be removed
      segment->deleteObject( mUndo );
      }
    else {
      //Change position of segment
      segment->setSegment( a, b, mUndo );
      }
    }
  else if( a != b ) {
    //Segment must be inserted
    segment = new SdGraphTracedRoad( prop, a, b );
    plate()->insertChild( segment, mUndo );
    }
  }




