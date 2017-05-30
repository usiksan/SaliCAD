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
#include "windows/SdPropBarWire.h"
#include "windows/SdWCommand.h"
#include <QMessageBox>

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
    ctx->setOverColor( sdEnvir->getSysColor(scEnter) );
    if( mFirst != mMiddle ) ctx->line( mFirst, mMiddle, sdGlobalProp->mWireProp );
    if( mMiddle != mPrevMove ) ctx->line( mMiddle, mPrevMove, sdGlobalProp->mWireProp );
    ctx->resetOverColor();
    }
  if( sdEnvir->mIsWireSmart ) {
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
              //Question: rename net in other sheets
              if( QMessageBox::question( this, QObject::tr("Query"), QObject::tr("Rename nets in all other sheets?")) == QMessageBox::Yes ) {
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
            }
          mName = wireName;
          }
        else {

          }
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
  }

void SdModeCWire::enterPoint(SdPoint)
  {
  }

void SdModeCWire::cancelPoint(SdPoint)
  {
  }

void SdModeCWire::movePoint(SdPoint)
  {
  }

void SdModeCWire::enterPrev()
  {
  }

bool SdModeCWire::getInfo(SdPoint p, QString &info)
  {
  }




QString SdModeCWire::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of wire polyline") : QObject::tr("Enter first wire point");
  }




QString SdModeCWire::getModeHelp() const
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




bool SdModeCWire::okUnion(const QString newName)
  {
  return QMessageBox::question( mEditor, QObject::tr("Warning!"), QObject::tr("Union net '%1' and net '%2' to net '%2'?").arg(mName).arg(newName) ) == QMessageBox::Yes;
  }




SdPItemSheet *SdModeCWire::getSheet()
  {
  return dynamic_cast<SdPItemSheet*>(mObject);
  }
