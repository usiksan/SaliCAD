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
    SdOrientation orient1 = SdSegment( mSource1, mMove1 ).orientation();
    SdOrientation orient2 = SdSegment( mSource2, mMove2 ).orientation();
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
        mDirX1 = mDirX2 = mDirY1 = mDirY2 = 0;
        qDebug() << "not available";
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
            if( mSource1.y() < mMove.y() && mSource2.y() <= mMove.y() ) {
              mMinY = qMax( mSource1.y(), mSource2.y() );
              mMaxY = INT_MAX;
              }
            else if( mSource1.y() > mMove.y() && mSource2.y() >= mMove.y() ) {
              mMinY = INT_MIN;
              mMaxY = qMin( mSource1.y(), mSource2.y() );
              }
            else {
              mMinY = qMin( mSource1.y(), mSource2.y() );
              mMaxY = qMax( mSource1.y(), mSource2.y() );
              }
            qDebug() << "*vertical vertical";
            break;


          case sorHorizontal :
            //For p1.y == p2.y
            if( mMove1.y() > mMove2.y() || (mMove1.y() == mMove2.y() && mSource1.y() > mMove1.y()) ) {
              //up
              if( mSource1.y() > mMove1.y() ) {
                mMinY = mMove2.y();
                mMaxY = mSource1.y();
                }
              else {
                mMinY = qMax( mMove2.y(), mSource1.y() );
                mMaxY = INT_MAX;
                }
              if( mMove2.x() < mMove1.x() || (mMove2.x() == mMove1.x() && mSource2.x() < mMove2.x() ) ) {
                //left
                mDirX2 = -1;
                mDirY2 = 0;
                if( mSource2.x() < mMove2.x() ) {
                  mMaxX = mSource1.x();
                  mMinX = mSource2.x();
                  }
                else {
                  mMinX = INT_MIN;
                  mMaxX = qMin( mSource1.x(), mMove2.x() );
                  }
                }
              else {
                //right
                mDirX2 = 1;
                mDirY2 = 0;
                if( mSource2.x() > mMove2.x() ) {
                  mMaxX = mSource2.x();
                  mMinX = mSource1.x();
                  }
                else {
                  mMaxX = INT_MAX;
                  mMinX = qMax( mSource1.x(), mMove2.x() );
                  }
                }
              }
            else {
              //down
              if( mSource1.y() < mMove1.y() ) {
                mMaxY = mMove2.y();
                mMinY = mSource1.y();
                }
              else {
                mMaxY = qMin( mMove2.y(), mSource1.y() );
                mMinY = INT_MIN;
                }
              if( mMove2.x() < mMove1.x() || (mMove2.x() == mMove1.x() && mSource2.x() < mMove2.x() ) ) {
                //left
                mDirX2 = 1;
                mDirY2 = 0;
                if( mSource2.x() < mMove2.x() ) {
                  mMaxX = mSource1.x();
                  mMinX = mSource2.x();
                  }
                else {
                  mMinX = INT_MIN;
                  mMaxX = qMin( mSource1.x(), mMove2.x() );
                  }
                }
              else {
                //right
                mDirX2 = -1;
                mDirY2 = 0;
                if( mSource2.x() > mMove2.x() ) {
                  mMaxX = mSource2.x();
                  mMinX = mSource1.x();
                  }
                else {
                  mMaxX = INT_MAX;
                  mMinX = qMax( mSource1.x(), mMove2.x() );
                  }
                }
              }
            qDebug() << "*vertical horizontal";
            break;


          case sorSlashForward :
            //For dx == dy
            if( mMove1.y() == mMove2.y() ) {
              //In this configuration we move only horizontal segments
              mDirX2 = 1;
              mDirY2 = 1;
              //y - coord of intersection segment1 and segment2
              int y = mSource1.x() - mSource2.x() + mSource2.y();
              mMinX = INT_MIN;
              mMaxX = INT_MAX;
              if( mSource1.y() > mMove1.y() ) {
                if( mSource2.y() > mMove2.y() ) {
                  mMaxY = qMin( mSource1.y(), mSource2.y() );
                  mMinY = y > mMaxY ? INT_MIN : y;
                  }
                else {
                  mMaxY = mSource1.y();
                  mMinY = mSource2.y();
                  }
                }
              else {
                if( mSource2.y() < mMove2.y() ) {
                  mMinY = qMax( mSource1.y(), mSource2.y() );
                  mMaxY = y < mMinY ? INT_MAX : y;
                  }
                else {
                  mMinY = mSource1.y();
                  mMaxY = mSource2.y();
                  }
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
              //y - coord of intersection segment1 and segment2
              int y = mSource2.x() - mSource1.x() + mSource2.y();
              mMinX = INT_MIN;
              mMaxX = INT_MAX;
              if( mSource1.y() > mMove1.y() ) {
                if( mSource2.y() > mMove2.y() ) {
                  mMaxY = qMin( mSource1.y(), mSource2.y() );
                  mMinY = y > mMaxY ? INT_MIN : y;
                  }
                else {
                  mMaxY = mSource1.y();
                  mMinY = mSource2.y();
                  }
                }
              else {
                if( mSource2.y() < mMove2.y() ) {
                  mMinY = qMax( mSource1.y(), mSource2.y() );
                  mMaxY = y < mMinY ? INT_MAX : y;
                  }
                else {
                  mMinY = mSource1.y();
                  mMaxY = mSource2.y();
                  }
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
            if( mSource1.x() < mMove.x() && mSource2.x() < mMove.x() ) {
              mMinX = qMax( mSource1.x(), mSource2.x() );
              mMaxX = INT_MAX;
              }
            else if( mSource1.x() > mMove.x() && mSource2.x() > mMove.x() ) {
              mMinX = INT_MIN;
              mMaxX = qMin( mSource1.x(), mSource2.x() );
              }
            else {
              mMinX = qMin( mSource1.x(), mSource2.x() );
              mMaxX = qMax( mSource1.x(), mSource2.x() );
              }
            qDebug() << "*horizontal horizontal";
            break;


          case sorSlashForward :
            //For dx == dy
            if( mMove1.x() == mMove2.x() ) {
              //In this configuration we move only vertical segments
              mDirX2 = 1;
              mDirY2 = 1;
              //x - coord of intersection segment1 and segment2
              int x = mSource1.y() - mSource2.y() + mSource2.x();
              mMinY = INT_MIN;
              mMaxY = INT_MAX;
              if( mSource1.x() > mMove1.x() ) {
                if( mSource2.x() > mMove2.x() ) {
                  mMaxX = qMin( mSource1.x(), mSource2.x() );
                  mMinX = x > mMaxX ? INT_MIN : x;
                  }
                else {
                  mMaxX = mSource1.x();
                  mMinX = mSource2.x();
                  }
                }
              else {
                if( mSource2.x() < mMove2.x() ) {
                  mMinX = qMax( mSource1.x(), mSource2.x() );
                  mMaxX = x < mMinX ? INT_MAX : x;
                  }
                else {
                  mMinX = mSource1.x();
                  mMaxX = mSource2.x();
                  }
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
              //x - coord of intersection segment1 and segment2
              int x = mSource2.y() - mSource1.y() + mSource2.x();
              mMinY = INT_MIN;
              mMaxY = INT_MAX;
              if( mSource1.x() > mMove1.x() ) {
                if( mSource2.x() > mMove2.x() ) {
                  mMaxX = qMin( mSource1.x(), mSource2.x() );
                  mMinY = x > mMaxX ? INT_MIN : x;
                  }
                else {
                  mMaxX = mSource1.x();
                  mMinX = mSource2.x();
                  }
                }
              else {
                if( mSource2.x() < mMove2.x() ) {
                  mMinX = qMax( mSource1.x(), mSource2.x() );
                  mMaxX = x < mMinX ? INT_MAX : x;
                  }
                else {
                  mMinX = mSource1.x();
                  mMaxX = mSource2.x();
                  }
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
            mDirX2 = 1;
            mDirY2 = 1;
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
            if( mSource1.x() < mMove.x() && mSource2.x() < mMove.x() ) {
              mMinX = qMax( mSource1.x(), mSource2.x() );
              mMaxX = INT_MAX;
              mMinY = qMax( mSource1.y(), mSource2.y() );
              mMaxY = INT_MAX;
              }
            else if( mSource1.x() > mMove.x() && mSource2.x() > mMove.x() ) {
              mMinX = INT_MIN;
              mMaxX = qMin( mSource1.x(), mSource2.x() );
              mMinY = INT_MIN;
              mMaxY = qMin( mSource1.y(), mSource2.y() );
              }
            else {
              mMinX = qMin( mSource1.x(), mSource2.x() );
              mMaxX = qMax( mSource1.x(), mSource2.x() );
              mMinY = qMin( mSource1.y(), mSource2.y() );
              mMaxY = qMax( mSource1.y(), mSource2.y() );
              }
            qDebug() << "forward forward";
            break;


          case sorSlashBackward : {
            //For dx == -dy
            mDirX2 = 1;
            mDirY2 = -1;
            //y = (x2 +y2 + y1 - x1) / 2
            //x = y - y1 + x1
            int y = (mSource2.x() + mSource2.y() + mSource1.y() - mSource1.x()) / 2;
            int x = y - mSource1.y() + mSource1.x();
            // \1/
            // 4+2
            // /3\ .
            if( mMove1.y() > y || (mMove1.y() == y && mSource1.y() > y) ) {
              //1 or 2
              if( mMove2.y() > y || (mMove2.y() == y && mSource2.y() > y) ) {
                //1
                if( mSource1.y() < mMove1.y() && mSource2.y() < mMove2.y() ) {
                  mMaxY = INT_MAX;
                  mMinY = qMax( y, qMax(mSource1.y(),mSource2.y()) );
                  }
                else {
                  mMaxY = qMin( mSource1.y(), mSource2.y() );
                  mMinY = y;
                  }
                mMinX = INT_MIN;
                mMaxX = INT_MAX;
                }
              else {
                //2
                if( mSource1.x() < mMove1.x() && mSource2.x() < mMove2.x() ) {
                  mMaxX = INT_MAX;
                  mMinX = qMax( x, qMax(mSource1.x(),mSource2.x()) );
                  }
                else {
                  mMaxX = qMin( mSource1.x(), mSource2.x() );
                  mMinX = x;
                  }
                mMinY = INT_MIN;
                mMaxY = INT_MAX;
                }
              }
            else {
              //3 or 4
              if( mMove2.y() < y || (mMove2.y() == y && mSource2.y() < y) ) {
                //3
                if( mSource1.y() > mMove1.y() && mSource2.y() > mMove2.y() ) {
                  mMaxY = qMin( y, qMin(mSource1.y(),mSource2.y()) );;
                  mMinY = INT_MIN;
                  }
                else {
                  mMaxY = y;
                  mMinY = qMax( mSource1.y(), mSource2.y() );
                  }
                mMinX = INT_MIN;
                mMaxX = INT_MAX;
                }
              else {
                //4
                if( mSource1.x() > mMove1.x() && mSource2.x() > mMove2.x() ) {
                  mMaxX = qMin( x, qMin(mSource1.x(),mSource2.x()) );
                  mMinX = INT_MIN;
                  }
                else {
                  mMaxX = x;
                  mMinX = qMax( mSource1.x(), mSource2.x() );
                  }
                mMinY = INT_MIN;
                mMaxY = INT_MAX;
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
            if( mSource1.x() < mMove.x() && mSource2.x() < mMove.x() ) {
              mMinX = qMax( mSource1.x(), mSource2.x() );
              mMaxX = INT_MAX;
              mMinY = INT_MIN;
              mMaxY = qMin( mSource1.y(), mSource2.y() );
              }
            else if( mSource1.x() > mMove.x() && mSource2.x() > mMove.x() ) {
              mMinX = INT_MIN;
              mMaxX = qMin( mSource1.x(), mSource2.x() );
              mMinY = qMax( mSource1.y(), mSource2.y() );
              mMaxY = INT_MAX;
              }
            else {
              mMinX = qMin( mSource1.x(), mSource2.x() );
              mMaxX = qMax( mSource1.x(), mSource2.x() );
              mMinY = qMin( mSource1.y(), mSource2.y() );
              mMaxY = qMax( mSource1.y(), mSource2.y() );
              }
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
  //Limitation
  if( mDirX1 ) {
    int v = SdUtil::iLimit( mMove1.x() + d*mDirX1, mMinX, mMaxX );
    d = (v - mMove1.x()) / mDirX1;
    }
  if( mDirY1 ) {
    int v = SdUtil::iLimit( mMove1.y() + d*mDirY1, mMinY, mMaxY );
    d = (v - mMove1.y()) / mDirY1;
    }
  if( mDirX2 ) {
    int v = SdUtil::iLimit( mMove2.x() + d*mDirX2, mMinX, mMaxX );
    d = (v - mMove2.x()) / mDirX2;
    }
  if( mDirY2 ) {
    int v = SdUtil::iLimit( mMove2.y() + d*mDirY2, mMinY, mMaxY );
    d = (v - mMove2.y()) / mDirY2;
    }
  //Try moving
  SdPoint move1(mMove1);
  SdPoint move2(mMove2);
  move1.move( SdPoint(d*mDirX1,d*mDirY1) );
  move2.move( SdPoint(d*mDirX2,d*mDirY2) );

  //Check if new position available
  if( sdCheckRoadOnBarrierList( mRoads1, mSource1, mMove1, mProp.mNetName.str() ) == mMove1 &&
      sdCheckRoadOnBarrierList( mRoads, mMove1, mMove2, mProp.mNetName.str() ) == mMove2 &&
      sdCheckRoadOnBarrierList( mRoads2, mSource2, mMove2, mProp.mNetName.str() ) == mMove2 ) {
    //Fact moving
    mMove1.move( SdPoint(d*mDirX1,d*mDirY1) );
    mMove2.move( SdPoint(d*mDirX2,d*mDirY2) );
    }

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




