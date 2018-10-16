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
      if( !mShowNet || net->getNetName() != mNetName )
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
    ctx->setOverColor( sdEnvir->getSysColor(scEnter) );
    mObject->forEach( dctNetWire | dctNetName, [this,ctx] (SdObject *obj) -> bool {
      SdGraphNet *net = dynamic_cast<SdGraphNet*>( obj );
      if( net != nullptr && net->getNetName() == mNetName )
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




int SdModeCNetWire::getPropBarId() const
  {
  return PB_WIRE;
  }



void SdModeCNetWire::propGetFromBar()
  {
  SdPropBarWire *bar = dynamic_cast<SdPropBarWire*>( SdWCommand::mbarTable[PB_WIRE] );
  if( bar ) {
    QString wireName;
    bar->getPropWire( &(sdGlobalProp->mWireProp), &(sdGlobalProp->mWireEnterType), &wireName );
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
  if( bar )
    bar->setPropWire( &(sdGlobalProp->mWireProp), mEditor->getPPM(), sdGlobalProp->mWireEnterType, mNetName );
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
      mUndo->begin( QObject::tr("Insert wire segment"), mObject );
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
    mSmA = mMiddle;
    mStrEnd = mSmA;
    mSmEnd = mSmA;
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
  mUndo->begin( QObject::tr("Insert smart net"), mObject );
  if( getStep() ) {
    //Net end variant
    if( mFirst != mSmA ) getSheet()->insertChild( new SdGraphNetWire( mFirst, mSmA, mNetName, sdGlobalProp->mWireProp ), mUndo );
    if( mSmA != mStrEnd && mSmEnd != mSmA ) getSheet()->insertChild( new SdGraphNetWire( mSmA, mStrEnd, mNetName, sdGlobalProp->mWireProp ), mUndo );
    if( mStrEnd != mSmEnd && mSmEnd != mSmA  ) getSheet()->insertChild( new SdGraphNetWire( mStrEnd, mSmEnd, mNetName, sdGlobalProp->mWireProp ), mUndo );
    }
  else {
    //Full routing net variant
    if( testFirstPoint( mFirst ) ) {
      if( mFirst != mSmA ) getSheet()->insertChild( new SdGraphNetWire( mFirst, mSmA, mNetName, sdGlobalProp->mWireProp ), mUndo );
      if( mSmA != mStrEnd && mSmEnd != mSmA  ) getSheet()->insertChild( new SdGraphNetWire( mSmA, mStrEnd, mNetName, sdGlobalProp->mWireProp ), mUndo );
      if( mStrEnd != mSmEnd && mSmEnd != mSmA  ) getSheet()->insertChild( new SdGraphNetWire( mStrEnd, mSmEnd, mNetName, sdGlobalProp->mWireProp ), mUndo );
      }
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
  return QString( MODE_HELP "ModeCWire.htm" );
  }




QString SdModeCNetWire::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCWire.htm#nextPoint" ) : QString( MODE_HELP "ModeCWire.htm#firstPoint" );
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
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestNet | snapNearestPin;
  snap.mExclude  = mPrevMove;
  snap.mDest     = mPrevMove;
  snap.calculate( getSheet() );
  mFirst = snap.mDest;
  calcSmartPoint();
  }





void SdModeCNetWire::calcSecondSmart()
  {
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestNet | snapNearestPin | snapExcludeSour;
  snap.mExclude  = mPrevMove;
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


void SdModeCNetWire::calcSmartPoint()
  {
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestPin | snapExcludeExcl;
  snap.mExclude  = mFirst;
  SdRect over;
  bool noResult = true;
  getSheet()->forEach( dctSymImp, [&snap, &over, &noResult] (SdObject *obj) -> bool {
    SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>( obj );
    Q_ASSERT( sym != nullptr );
    if( sym->snapPoint( &snap ) ) {
      noResult = false;
      over = sym->getOverRect();
      }
    return true;
    });

  //qDebug() << "calcSmartPoint" << noResult << snap.mDest;
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
    else if( snap.mDest.y() == over.getTopLeft().y() ) mSmA.ry() = mStrEnd.ry() += stringer;
    else if( snap.mDest.y() == over.getBottomLeft().y() ) mSmA.ry() = mStrEnd.ry() -= stringer;
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




void SdModeCNetWire::activate()
  {
  nextNet();
  }
