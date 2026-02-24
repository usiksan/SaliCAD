/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter via
*/
#include "SdModeCViaEnter.h"
#include "objects/SdProp.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphTracedRoad.h"
#include "objects/SdGraphTracedVia.h"
#include "windows/SdPropBarRoad.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

#include <QDebug>

SdModeCViaEnter::SdModeCViaEnter(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {
  mViaProp.mPadType = SdEnvir::instance()->mViaType;
  mViaProp.mStratum = stmThrough;
  }





void SdModeCViaEnter::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mViaProp.mStratum, QString() );
  }




int SdModeCViaEnter::getPropBarId() const
  {
  return PB_VIA;
  }




void SdModeCViaEnter::propGetFromBar()
  {
  SdPropBarRoad *bar = dynamic_cast<SdPropBarRoad*>( SdWCommand::mBarTable[PB_VIA] );
  if( bar ) {
    bar->getPropVia( &mViaProp );
    sdGlobalProp->mViaProp = mViaProp;
    mEditor->setFocus();
    setDirtyCashe();
    update();
    }
  }




void SdModeCViaEnter::propSetToBar()
  {
  SdPropBarRoad *bar = dynamic_cast<SdPropBarRoad*>( SdWCommand::mBarTable[PB_VIA] );
  if( bar ) {
    //Setup tracing layer count and trace type
    bar->setPlateAndTrace( plate(), layerTraceRoad );
    bar->setPropVia( &mViaProp );
    }
  }




void SdModeCViaEnter::enterPoint(SdPoint p)
  {
  //Find net behind cursor
  SdPvStratum stratum = mViaProp.mStratum;
  QString netName;
  plate()->forEach( dctTraced, [p,&stratum,&netName] (SdObject *obj) -> bool {
    SdPtr<SdGraphTraced> traced(obj);
    if( traced.isValid() )
      return !traced->isPointOnNet( p, stratum, netName, stratum );
    return true;
    });

  if( !netName.isEmpty() ) {
    SdRuleBlock rule;
    plate()->ruleBlockForNet( netName, rule );
    //Accum barriers for via
    mPads.clear();
    //Update width in rule block to current from prop
    int r = plate()->getPadOverRadius( mViaProp.mPadType.string() );
    if( r > 0 )
      rule.mRules[ruleRoadWidth] = r;
    plate()->accumBarriers( dctTraced, mPads, mViaProp.mStratum, rulePadPad, rule );
    //Check if current point possible
    if( !sdIsBarrierListContains( mPads, netName, p ) ) {
      //Via is available
      //Via is available at this point - insert
      mViaProp.mNetName = netName;
      //qDebug() << "Via inserted" << mViaProp.mPadType.str() << mViaProp.mNetName.str();
      addPic( new SdGraphTracedVia( p, mViaProp ), QObject::tr("Insert trace via") );
      setDirtyCashe();
      update();
      }
    }

  }




void SdModeCViaEnter::cancelPoint(SdPoint)
  {
  cancelMode();
  }




QString SdModeCViaEnter::getStepHelp() const
  {
  return QObject::tr("Enter position to add via to");
  }





QString SdModeCViaEnter::getModeThema() const
  {
  return QString( MODE_HELP "ModeCViaEnter.htm" );
  }





QString SdModeCViaEnter::getStepThema() const
  {
  return getModeThema();
  }





int SdModeCViaEnter::getCursor() const
  {
  return CUR_VIA_ADD;
  }




int SdModeCViaEnter::getIndex() const
  {
  return MD_VIA_ENTER;
  }
