/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter wire in sheet
*/
#include "SdModeCNetWire.h"
#include "objects/SdGraph.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdEnvir.h"
#include "objects/SdProject.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdGraphNetWire.h"
#include "windows/SdPropBarWire.h"
#include "windows/SdWCommand.h"
#include "windows/SdDNetUnion.h"
#include "windows/SdWEditorGraph.h"
#include <QMessageBox>
#include <QDebug>

SdModeCNetWire::SdModeCNetWire(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }




void SdModeCNetWire::drawStatic(SdContext *ctx)
  {
  //All objects draw normally except net with netName.
  mObject->forEach( dctAll, [this,ctx] (SdObject *obj) -> bool {
    SdGraphNet *net = dynamic_cast<SdGraphNet*>( obj );
    if( net != nullptr ) {
      if( !mShowNet || mNetName != net->getNetName() )
        net->draw( ctx );
      }
    else {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph != nullptr )
        graph->draw( ctx );
      }
    return true;
    });

  //Draw if net present
  if( !mNetName.isEmpty() && mShowNet ) {
    ctx->setOverColor( SdEnvir::instance()->getSysColor(scEnter) );
    mObject->forEach( dctNetWire | dctNetName | dctNetParam, [this,ctx] (SdObject *obj) -> bool {
      SdGraphNet *net = dynamic_cast<SdGraphNet*>( obj );
      if( net != nullptr && mNetName == net->getNetName() )
        net->draw( ctx );
      return true;
      });
    ctx->resetOverColor();
    }
  }




void SdModeCNetWire::drawDynamic(SdContext *ctx)
  {
  if( getStep() ) {
    //Draw entering segment with possible vertex
    ctx->setOverColor( SdEnvir::instance()->getSysColor(scEnter) );
    if( mFirst != mMiddle ) ctx->line( mFirst, mMiddle, sdGlobalProp->mWireProp );
    if( mMiddle != mPrevMove ) ctx->line( mMiddle, mPrevMove, sdGlobalProp->mWireProp );
    ctx->resetOverColor();
    }
  if( SdEnvir::instance()->mIsWireSmart && mSmartSour != mSmartDest ) {
    //Draw smart variant
    ctx->setOverColor( SdEnvir::instance()->getSysColor(scSmart) );
    //Draw source stringer if present
    if( mSmartSour != mSmartSourStr )   ctx->line( mSmartSour, mSmartSourStr, sdGlobalProp->mWireProp );
    //Draw first intermediate vertex
    if( mSmartSourStr != mSmartInter0 ) ctx->line( mSmartSourStr, mSmartInter0, sdGlobalProp->mWireProp );
    //Draw second intermediate vertex
    if( mSmartInter0 != mSmartInter1 )  ctx->line( mSmartInter0, mSmartInter1, sdGlobalProp->mWireProp );
    //Draw destignation vertex
    if( mSmartInter1 != mSmartDestStr ) ctx->line( mSmartInter1, mSmartDestStr, sdGlobalProp->mWireProp );
    //Draw destignation stringer if present
    if( mSmartDestStr != mSmartDest )   ctx->line( mSmartDestStr, mSmartDest, sdGlobalProp->mWireProp );
    ctx->smartPoint( mSmartDest, snapNetPoint );
    ctx->resetOverColor();
    }
  }




int SdModeCNetWire::getPropBarId() const
  {
  return PB_WIRE;
  }



void SdModeCNetWire::propGetFromBar()
  {
  SdPropBarWire *bar = dynamic_cast<SdPropBarWire*>( SdWCommand::mbarTable[PB_WIRE] );
  if( bar ) {
    SdPvMulty<SdPvString> pvWireName;
    QString wireName;
    bar->getPropWire( sdGlobalProp->mWireProp, &(sdGlobalProp->mWireEnterType), pvWireName );
    Q_ASSERT( pvWireName.isSingle() );
    wireName = pvWireName.value().string();
    if( getStep() ) {
      //If wire enter in process then perhaps net union. Detect it
      if( mNetName != wireName ) {
        //Entered new wire name
        if( getSheet()->isNetPresent(wireName) ) {
          if( getSheet()->isNetPresent(mNetName) ) {
            //Both nets present: with new name and with old name
            if( okUnion(wireName) ) {
              getSheet()->netRename( mNetName, wireName, mUndo );
              //Question: rename net in other sheets and rename if yes
              renameOtherSheets( wireName );
              }
            }
          }
        else {
          if( getSheet()->isNetPresent(mNetName) ) {
            //New net is not defined yet. Rename old net
            getSheet()->netRename( mNetName, wireName, mUndo );
            //Question: rename net in other sheets and rename if yes
            renameOtherSheets( wireName );
            }
          }
        mNetName = wireName;
        }
      }
    else {
      mNetName = wireName;
      }
    mEditor->setFocus();
    update();
    }
  }





void SdModeCNetWire::propSetToBar()
  {
  SdPropBarWire *bar = dynamic_cast<SdPropBarWire*>( SdWCommand::mbarTable[PB_WIRE] );
  if( bar ) {
    SdPvMulty<SdPvString> netName;
    netName.reset( SdPvString(mNetName) );
    bar->setPropWire( sdGlobalProp->mWireProp, mEditor->getPPM(), sdGlobalProp->mWireEnterType, netName );
    }
  }




void SdModeCNetWire::enterPoint( SdPoint enter )
  {
  if( getStep() ) {
    mPrevMove = calcMiddlePoint( mFirst, enter, sdGlobalProp->mWireEnterType );
    if( mPrevMove == mFirst ) mPrevMove = enter;
    mShowNet = false;
    setDirtyCashe();
    //Append segment
    if( testNextPoint( mPrevMove ) && mFirst != mPrevMove ) {
      mUndo->begin( QObject::tr("Insert wire segment"), mObject, false );
      mObject->insertChild( new SdGraphNetWire( mFirst, mPrevMove, mNetName, sdGlobalProp->mWireProp ), mUndo );
      mFirst = mPrevMove;
      setDirty();
      }
    mPrevMove = enter;
    mMiddle = calcMiddlePoint( mFirst, mPrevMove, sdGlobalProp->mWireEnterType );
    calcSmartPoint();
    }
  else {
    mFirst = enter;
    mPrevMove = mFirst;
    if( testFirstPoint( mFirst ) ) setStep(sNextPoint);
    //Reset smart
    mMiddle = enter;
    mSmartSour = enter;
    mSmartDest = enter;
    }
  update();
  }



void SdModeCNetWire::cancelPoint(SdPoint)
  {
  update();
  if( getStep() ) nextNet();
  else cancelMode();
  }



void SdModeCNetWire::movePoint( SdPoint p )
  {
  if( getStep() ) {
    mPrevMove = p;
    mMiddle = calcMiddlePoint( mFirst, mPrevMove, sdGlobalProp->mWireEnterType );
    //For smart point we request is it sym imp
    mSmartSour = mFirst;
    int state = 0;
    SdPtr<SdGraph> graph( getSheet()->behindPoint( dctSymImp, mFirst, &state ) );
    mSmartSourStr = getStringer( mSmartSour.x(), mSmartSour.y(), graph.ptr() );
    calcSmartPoint();
    }
  else {
    mPrevMove = p;
    calcFirstSmart();
    }
  update();
  }




SdPoint SdModeCNetWire::enterPrev()
  {
  mUndo->begin( QObject::tr("Insert smart net"), mObject, false );
  if( getStep() )
    //Net end variant
    insertSmartNet();
  else {
    //Full routing net variant
    if( testFirstPoint( mSmartSour ) )
      insertSmartNet();
    }
  nextNet();
  setDirty();
  calcFirstSmart();
  mFirst = mPrevMove;
  mMiddle = mFirst;
  update();
  return mFirst;
  }






QString SdModeCNetWire::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of wire polyline") : QObject::tr("Enter first wire point");
  }




QString SdModeCNetWire::getModeThema() const
  {
  return QString( MODE_HELP "ModeCNetWire.htm" );
  }




QString SdModeCNetWire::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCNetWire.htm#nextPoint" ) : QString( MODE_HELP "ModeCNetWire.htm#firstPoint" );
  }




int SdModeCNetWire::getCursor() const
  {
  return CUR_WIRE;
  }




int SdModeCNetWire::getIndex() const
  {
  return MD_WIRE;
  }




SdModeCNetWire::RenumResult SdModeCNetWire::getUnionResult(const QString firstWireName, const QString secondWireName)
  {
  return static_cast<SdModeCNetWire::RenumResult>( SdDNetUnion( firstWireName, secondWireName, mEditor ).exec() );
  }




bool SdModeCNetWire::okUnion(const QString newName)
  {
  return QMessageBox::question( mEditor, QObject::tr("Warning!"), QObject::tr("Union net '%1' and net '%2'?").arg(mNetName).arg(newName) ) == QMessageBox::Yes;
  }



void SdModeCNetWire::renameOtherSheets(const QString wireName)
  {
  if( QMessageBox::question( mEditor, QObject::tr("Query"), QObject::tr("Rename nets in all other sheets?")) == QMessageBox::Yes ) {
    //User answer "Yes", so for each sheet in project execute renaming
    SdProject *prj = getSheet()->getProject();
    Q_ASSERT(prj != nullptr);
    prj->forEach( dctSheet, [this,wireName] (SdObject *obj) -> bool {
      SdPItemSheet *sheet = dynamic_cast<SdPItemSheet*>(obj);
      Q_ASSERT( sheet != nullptr );
      sheet->netRename( mNetName, wireName, mUndo );
      return true;
      });
    }
  }





bool SdModeCNetWire::testFirstPoint(SdPoint p)
  {
  QString tmpName;
  if( getSheet()->getNetFromPoint( p, tmpName ) ) {
    if( mNetName.startsWith( defNetNamePrefix ) )
      //Our net has default name, assign it found net name
      mNetName = tmpName;
    else {
      if( tmpName.startsWith( defNetNamePrefix ) )
        //Net has default name, override it with our existing net (rename)
        renameNet( tmpName, mNetName );
      else {
        //Nets have not default names. Query user to deсision what name assign
        switch( getUnionResult( tmpName, mNetName ) ) {
          case renFirst  : mNetName = tmpName; break;             //Lets first
          case renSecond : renameNet( tmpName, mNetName ); break; //Lets second
          default : return false;
          }
        }
      }
    }
  propSetToBar();
  mShowNet = getSheet()->isNetPresent( mNetName );
  setDirtyCashe();
  return true;
  }




bool SdModeCNetWire::testNextPoint(SdPoint p)
  {
  QString tmpName;
  if( getSheet()->getNetFromPoint( p, tmpName ) && tmpName != mNetName ) {
    if( okUnion(tmpName) ) {
      //Net union
      if( mNetName.startsWith( defNetNamePrefix ) ) renameNet( mNetName, tmpName );
      else if( tmpName.startsWith( defNetNamePrefix ) ) renameNet( tmpName, mNetName );
      else {
        //Nets have not default names. Query user to deсision what name assign
        switch( getUnionResult( mNetName, tmpName ) ) {
          case renFirst  : renameNet( tmpName, mNetName ); break; //Lets first
          case renSecond : renameNet( mNetName, tmpName ); break; //Lets second
          default : return false;
          }
        }
      }
    else return false;
    mShowNet = getSheet()->isNetPresent( mNetName );
    setDirtyCashe();
    }
  propSetToBar();
  return true;
  }




void SdModeCNetWire::renameNet(const QString sour, const QString dest)
  {
  getSheet()->netRename( sour, dest, mUndo );
  mNetName = dest;
  }






void SdModeCNetWire::calcFirstSmart()
  {
  //Prepare snap query
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestNet | snapNearestPin;
  snap.mExclude  = mPrevMove;
  snap.mDest     = mPrevMove;
  //Scan snap point
  snap.scan( getSheet() );
  //Assign founded snap point
  mSmartSour = snap.mDest;
  //Calculate stringer for source point if need
  mSmartSourStr = getStringer( mSmartSour.x(), mSmartSour.y(), snap.mGraph );
  //Calculate destignation snap
  calcSmartPoint();
  }









//static SdPoint pointToFar( SdPoint sour, SdPoint dest ) {
//  if( sour.x() == dest.x() ) {
//    if( sour.y() > dest.y() ) sour.ry()--;
//    else sour.ry()++;
//    }
//  else if( sour.y() == dest.y() ) {
//    if( sour.x() > dest.x() ) sour.rx()--;
//    else sour.rx()++;
//    }
//  return sour;
//  }





void SdModeCNetWire::calcSmartPoint()
  {
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestPin | snapExcludeExcl;
  snap.mExclude  = mSmartSour;
  snap.scan( getSheet(), dctSymImp );
  SdRect over;
  bool noResult = snap.mGraph == nullptr;
  if( !noResult )
    over = snap.mGraph->getOverRect();

  //qDebug() << "calcSmartPoint" << noResult << snap.mDest;
  if( noResult ) {
    mSmartDest = mSmartSour;
    }
  else {
    mSmartDest = snap.mDest;
    //Calculate stringer
    //Stringer must have opposite direction with component body

    mSmartDestStr = getStringer( mSmartDest.x(), mSmartDest.y(), snap.mGraph );

    //Simple vertex
    mSmartInter0 = get90( mSmartSourStr, mSmartDestStr );
    mSmartInter1 = mSmartInter0;

    //Remove fail stringer
    if( mSmartInter0 == mSmartDest )
      mSmartDestStr = mSmartDest;

    }
  }




void SdModeCNetWire::nextNet()
  {
  setStep(sFirstPoint);
  mNetName = getSheet()->getProject()->getUnusedNetName();
  propSetToBar();
  mShowNet = false;
  setDirtyCashe();
  }








SdPItemSheet *SdModeCNetWire::getSheet()
  {
  return dynamic_cast<SdPItemSheet*>(mObject);
  }




//Insert prepared smart path net if present
void SdModeCNetWire::insertSmartNet()
  {
  //Perform insertion only if source and destignation points are not equals
  if( mSmartSour != mSmartDest ) {
    //Insert source stringer if present
    if( mSmartSour != mSmartSourStr )
      getSheet()->insertChild( new SdGraphNetWire( mSmartSour, mSmartSourStr, mNetName, sdGlobalProp->mWireProp ), mUndo );

    //Insert first intermediate vertex
    if( mSmartSourStr != mSmartInter0 )
      getSheet()->insertChild( new SdGraphNetWire( mSmartSourStr, mSmartInter0, mNetName, sdGlobalProp->mWireProp ), mUndo );

    //Insert second intermediate vertex
    if( mSmartInter0 != mSmartInter1 )
      getSheet()->insertChild( new SdGraphNetWire( mSmartInter0, mSmartInter1, mNetName, sdGlobalProp->mWireProp ), mUndo );

    //Insert destignation vertex
    if( mSmartInter1 != mSmartDestStr )
      getSheet()->insertChild( new SdGraphNetWire( mSmartInter1, mSmartDestStr, mNetName, sdGlobalProp->mWireProp ), mUndo );

    //Insert destignation stringer if present
    if( mSmartDestStr != mSmartDest )
      getSheet()->insertChild( new SdGraphNetWire( mSmartDestStr, mSmartDest, mNetName, sdGlobalProp->mWireProp ), mUndo );
    }
  }




//Calculate stringer only for sym imp
SdPoint SdModeCNetWire::getStringer(int x, int y, SdGraph *graph)
  {
  if( graph != nullptr && graph->getClass() == dctSymImp ) {
    int stringer = getGrid().x();


    SdRect over = graph->getOverRect();

    //Distance to left side of body
    int left = abs( over.getLeft() - x );
    //Distance to right side of body
    int right = abs( over.getRight() - x );
    //Minimum distance with horizontal direction
    int horz = qMin(left,right);

    //Distance to top side of bidy
    int top = abs( over.getTop() - y );
    //Distance to bottom side of body
    int bot = abs( over.getBottom() - y );
    //Minimum distance with vertical direction
    int vert = qMin( top, bot );

    //qDebug() << "stringer" << over.getTopLeft() << over.getBottomRight() << x << y;
    if( horz < vert ) {
      //Stringer is horizontal
      if( left < right )
        //Left direction
        x -= stringer;
      else
        //Right direction
        x += stringer;
      }
    else {
      //Stringer is vertical
      if( top < bot )
        //Top direction
        y += stringer;
      else
        //Bottom direction
        y -= stringer;
      }
    }
  return SdPoint(x,y);
  }




void SdModeCNetWire::activate()
  {
  nextNet();
  }
