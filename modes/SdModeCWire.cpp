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
#include "SdModeCWire.h"
#include "objects/SdGraph.h"
#include "objects/SdContainerSheetNet.h"
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

SdModeCWire::SdModeCWire(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mNet(nullptr),       //Enter net
  mShow(nullptr)       //Net for show
  {

  }




void SdModeCWire::drawStatic(SdContext *ctx)
  {
  //All objects draw normally except mShow net. It show selected
  mObject->forEach( dctAll, [this, ctx] (SdObject *obj) -> bool {
    if( obj->getParent() != mShow ) {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph )
        graph->draw( ctx );
      }
    return true;
    });

  //Draw mShow net if present
  if( mShow ) {
    ctx->setOverColor( sdEnvir->getSysColor(scEnter) );
    mShow->forEach( dctAll, [ctx] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph )
        graph->draw( ctx );
      return true;
      });
    ctx->resetOverColor();
    }
  }




void SdModeCWire::drawDynamic(SdContext *ctx)
  {
  if( getStep() ) {
    //Draw entering segment with possible vertex
    ctx->setOverColor( sdEnvir->getSysColor(scEnter) );
    if( mFirst != mMiddle ) ctx->line( mFirst, mMiddle, sdGlobalProp->mWireProp );
    if( mMiddle != mPrevMove ) ctx->line( mMiddle, mPrevMove, sdGlobalProp->mWireProp );
    ctx->resetOverColor();
    }
  if( sdEnvir->mIsWireSmart ) {
    //Draw smart variant
    ctx->setOverColor( sdEnvir->getSysColor(scSmart) );
    if( mSmA != mMiddle ) ctx->line( mFirst, mSmA, sdGlobalProp->mWireProp );
    ctx->line( mSmA, mStrEnd, sdGlobalProp->mWireProp );
    ctx->line( mStrEnd, mSmEnd, sdGlobalProp->mWireProp );
    ctx->smartPoint( mSmEnd, snapNetPoint );
    ctx->resetOverColor();
    }
  }




int SdModeCWire::getPropBarId() const
  {
  return PB_WIRE;
  }



void SdModeCWire::propGetFromBar()
  {
  SdPropBarWire *bar = dynamic_cast<SdPropBarWire*>( SdWCommand::mbarTable[PB_WIRE] );
  if( bar ) {
    QString wireName;
    bar->getPropWire( &(sdGlobalProp->mWireProp), &(sdGlobalProp->mWireEnterType), &wireName );
    if( getStep() ) {
      //If wire enter in process then perhaps net union. Detect it
      if( mName != wireName ) {
        //Entered new wire name
        if( getSheet()->netGet(wireName) ) {
          if( getSheet()->netGet(mName) ) {
            //Both nets present: with new name and with old name
            if( okUnion(wireName) ) {
              getSheet()->netRename( mName, wireName, mUndo );
              //Question: rename net in other sheets and rename if yes
              renameOtherSheets( wireName );
              }
            }
          }
        else {
          if( getSheet()->netGet(mName) ) {
            //New net is not defined yet. Rename old net
            getSheet()->netRename( mName, wireName, mUndo );
            //Question: rename net in other sheets and rename if yes
            renameOtherSheets( wireName );
            }
          }
        mName = wireName;
        }
      }
    else {
      mName = wireName;
      }
    mEditor->setFocus();
    update();
    }
  }





void SdModeCWire::propSetToBar()
  {
  SdPropBarWire *bar = dynamic_cast<SdPropBarWire*>( SdWCommand::mbarTable[PB_WIRE] );
  if( bar )
    bar->setPropWire( &(sdGlobalProp->mWireProp), mEditor->getPPM(), sdGlobalProp->mWireEnterType, mName );
  }




void SdModeCWire::enterPoint( SdPoint enter )
  {
  if( getStep() ) {
    mPrevMove = calcMiddlePoint( mFirst, enter, sdGlobalProp->mWireEnterType );
    if( mPrevMove == mFirst ) mPrevMove = enter;
    mShow = 0;
    //Append segment
    if( testNextPoint( mPrevMove ) && mFirst != mPrevMove ) {
      mUndo->begin( QObject::tr("Insert wire segment"), mObject );
      needCurNet()->insertChild( new SdGraphNetWire( mFirst, mPrevMove, sdGlobalProp->mWireProp ), mUndo );
      mFirst = mPrevMove;
      setDirty();
      }
    mPrevMove = enter;
    }
  else {
    mFirst = enter;
    mPrevMove = mFirst;
    if( testFirstPoint( mFirst ) ) setStep(sNextPoint);
    }
  update();
  }



void SdModeCWire::cancelPoint(SdPoint)
  {
  update();
  if( getStep() ) nextNet();
  else cancelMode();
  }



void SdModeCWire::movePoint( SdPoint p )
  {
  if( getStep() ) {
    mPrevMove = p;
    mMiddle = calcMiddlePoint( mFirst, mPrevMove, sdGlobalProp->mWireEnterType );
    calcSmartPoint();
    }
  else {
    mPrevMove = p;
    calcSecondSmart();
    }
  update();
  }




void SdModeCWire::enterPrev()
  {
  mUndo->begin( QObject::tr("Insert smart net"), mObject );
  if( getStep() ) {
    //Net end variant
    if( mFirst != mSmA ) needCurNet()->insertChild( new SdGraphNetWire( mFirst, mSmA, sdGlobalProp->mWireProp ), mUndo );
    if( mSmA != mStrEnd && mSmEnd != mSmA ) needCurNet()->insertChild( new SdGraphNetWire( mSmA, mStrEnd, sdGlobalProp->mWireProp ), mUndo );
    if( mStrEnd != mSmEnd && mSmEnd != mSmA  ) needCurNet()->insertChild( new SdGraphNetWire( mStrEnd, mSmEnd, sdGlobalProp->mWireProp ), mUndo );
    }
  else {
    //Full routing net variant
    if( testFirstPoint( mFirst ) ) {
      if( mFirst != mSmA ) needCurNet()->insertChild( new SdGraphNetWire( mFirst, mSmA, sdGlobalProp->mWireProp ), mUndo );
      if( mSmA != mStrEnd && mSmEnd != mSmA  ) needCurNet()->insertChild( new SdGraphNetWire( mSmA, mStrEnd, sdGlobalProp->mWireProp ), mUndo );
      if( mStrEnd != mSmEnd && mSmEnd != mSmA  ) needCurNet()->insertChild( new SdGraphNetWire( mStrEnd, mSmEnd, sdGlobalProp->mWireProp ), mUndo );
      }
    }
  nextNet();
  setDirty();
  update();
  calcSecondSmart();
  mFirst = mPrevMove;
  }






QString SdModeCWire::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of wire polyline") : QObject::tr("Enter first wire point");
  }




QString SdModeCWire::getModeThema() const
  {
  return QString( MODE_HELP "ModeCWire.htm" );
  }




QString SdModeCWire::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCWire.htm#nextPoint" ) : QString( MODE_HELP "ModeCWire.htm#firstPoint" );
  }




int SdModeCWire::getCursor() const
  {
  return CUR_WIRE;
  }




int SdModeCWire::getIndex() const
  {
  return MD_WIRE;
  }




SdModeCWire::RenumResult SdModeCWire::getUnionResult(const QString firstWireName, const QString secondWireName)
  {
  return static_cast<SdModeCWire::RenumResult>( SdDNetUnion( firstWireName, secondWireName, mEditor ).exec() );
  }




bool SdModeCWire::okUnion(const QString newName)
  {
  return QMessageBox::question( mEditor, QObject::tr("Warning!"), QObject::tr("Union net '%1' and net '%2'?").arg(mName).arg(newName) ) == QMessageBox::Yes;
  }



void SdModeCWire::renameOtherSheets(const QString wireName)
  {
  if( QMessageBox::question( mEditor, QObject::tr("Query"), QObject::tr("Rename nets in all other sheets?")) == QMessageBox::Yes ) {
    //User answer "Yes", so for each sheet in project execute renaming
    SdProject *prj = getSheet()->getProject();
    Q_ASSERT(prj != nullptr);
    prj->forEach( dctSheet, [this,wireName] (SdObject *obj) -> bool {
      SdPItemSheet *sheet = dynamic_cast<SdPItemSheet*>(obj);
      Q_ASSERT( sheet != nullptr );
      sheet->netRename( mName, wireName, mUndo );
      return true;
      });
    }
  }





bool SdModeCWire::testFirstPoint(SdPoint p)
  {
  QString tmpName;
  if( getSheet()->getNetFromPoint( p, tmpName ) ) {
    if( mName.startsWith( defNetNamePrefix ) )
      //Our net has default name, assign it found net name
      mName = tmpName;
    else {
      if( tmpName.startsWith( defNetNamePrefix ) )
        //Net has default name, override it with our existing net (rename)
        renameNet( tmpName, mName );
      else {
        //Nets have not default names. Query user to deсision what name assign
        switch( getUnionResult( tmpName, mName ) ) {
          case renFirst  : mName = tmpName; break;             //Lets first
          case renSecond : renameNet( tmpName, mName ); break; //Lets second
          default : return false;
          }
        }
      }
    }
  propSetToBar();
  mShow = mNet = getSheet()->netGet( mName );
  return true;
  }




bool SdModeCWire::testNextPoint(SdPoint p)
  {
  QString tmpName;
  if( getSheet()->getNetFromPoint( p, tmpName ) && tmpName != mName ) {
    if( okUnion(tmpName) ) {
      //Net union
      if( mName.startsWith( defNetNamePrefix ) ) renameNet( mName, tmpName );
      else if( tmpName.startsWith( defNetNamePrefix ) ) renameNet( tmpName, mName );
      else {
        //Nets have not default names. Query user to deсision what name assign
        switch( getUnionResult( mName, tmpName ) ) {
          case renFirst  : renameNet( tmpName, mName ); break; //Lets first
          case renSecond : renameNet( mName, tmpName ); break; //Lets second
          default : return false;
          }
        }
      }
    else return false;
    mShow = getSheet()->netGet( mName );
    }
  propSetToBar();
  mNet = getSheet()->netGet( mName );
  return true;
  }




void SdModeCWire::renameNet(const QString sour, const QString dest)
  {
  getSheet()->netRename( sour, dest, mUndo );
  mName = dest;
  }




void SdModeCWire::calcSecondSmart()
  {
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestNet | snapNearestPin;
  snap.mExclude  = mPrevMove;
  snap.mFlag     = dsifExSour;
  snap.mDest     = mPrevMove;
  snap.calculate( getSheet() );
  mFirst = snap.mDest;
  calcSmartPoint();
  }




static SdPoint pointToFar( SdPoint sour, SdPoint dest ) {
  if( sour.x() == dest.x() ) {
    if( sour.y() > dest.y() ) sour.ry()--;
    else sour.ry()++;
    }
  else if( sour.y() == dest.y() ) {
    if( sour.x() > dest.x() ) sour.rx()--;
    else sour.rx()++;
    }
  return sour;
  }


void SdModeCWire::calcSmartPoint()
  {
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestPin;
  snap.mExclude  = mFirst;
  snap.mFlag     = dsifExExcl;
  SdRect over;
  bool noResult = true;
  getSheet()->forEach( dctSymImp, [this, &snap, &over, &noResult] (SdObject *obj) -> bool {
    SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>( obj );
    Q_ASSERT( sym != nullptr );
    if( sym->snapPoint( &snap ) ) {
      noResult = false;
      over = sym->getOverRect();
      }
    return noResult;
    });

  qDebug() << "calcSmartPoint" << noResult << snap.mDest;
  if( noResult ) {
    mSmEnd = mPrevMove;
    mSmA   = mSmEnd;
    }
  else {
    mSmEnd = snap.mDest;
    //Calculate stringer
    mStrEnd = mSmEnd;
    int stringer = getGrid().x();
    if( snap.mDest.x() == over.getTopLeft().x() ) mSmA.rx() = mStrEnd.rx() -= stringer;
    else if( snap.mDest.x() == over.getTopRight().x() ) mSmA.rx() = mStrEnd.rx() += stringer;
    else if( snap.mDest.y() == over.getTopLeft().y() ) mSmA.ry() = mStrEnd.ry() -= stringer;
    else if( snap.mDest.y() == over.getBottomLeft().y() ) mSmA.ry() = mStrEnd.ry() += stringer;
    else mSmA = calcMiddlePoint( mFirst, mStrEnd, sdGlobalProp->mWireEnterType );
    //Calculate vertex point
    if( mSmA.x() == mStrEnd.x() ) mSmA.ry() = mFirst.y();
    else mSmA.rx() = mFirst.x();

    if( mSmA != mStrEnd && mSmA != mFirst ) {
      //Dubl vertex, test position vertex point
      int tmp = 0;
      getSheet()->behindPoint( dctAll, pointToFar( mFirst, mSmA ), &tmp );
      if( tmp ) {
        //Излом пошел на объект, сделать углом
        mSmA = mFirst;
        if( mSmEnd.x() == mStrEnd.x() ) mStrEnd.ry() = mFirst.y();
        else mStrEnd.rx() = mFirst.x();
        //Снова тест нового излома
        getSheet()->behindPoint( dctAll, pointToFar( mFirst, mStrEnd ), &tmp );
        if( tmp ) {
          //Снова над объектом, добавить стрингер
          if( mSmEnd.x() == mStrEnd.x() ) {
            //Стрингер по Y
            if( mSmEnd.y() > mStrEnd.y() ) {
              mStrEnd.ry() -= stringer;
              mSmA.ry() -= stringer;
              }
            else {
              mStrEnd.ry() += stringer;
              mSmA.ry() += stringer;
              }
            }
          else {
            //Стрингер по X
            if( mSmEnd.x() > mStrEnd.x() ) {
              mStrEnd.rx() -= stringer;
              mSmA.rx() -= stringer;
              }
            else {
              mStrEnd.rx() += stringer;
              mSmA.rx() += stringer;
              }
            }
          }
        }
      }
    }
  }




void SdModeCWire::nextNet()
  {
  setStep(sFirstPoint);
  mName = getSheet()->getProject()->getUnusedNetName();
  propSetToBar();
  mShow = mNet = 0;
  }




SdContainerSheetNet *SdModeCWire::needCurNet()
  {
  if( !mNet || mName != mNet->getNetName() ) {
    //Try to find net in schematic sheet, if not found then create it
    mNet = getSheet()->netCreate( mName, mUndo );
    }
  //At this point net must be defined
  return mNet;
  }







SdPItemSheet *SdModeCWire::getSheet()
  {
  return dynamic_cast<SdPItemSheet*>(mObject);
  }




void SdModeCWire::activate()
  {
  nextNet();
  }
