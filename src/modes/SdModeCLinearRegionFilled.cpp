/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCLinearRegionFilled.h"
#include "objects/SdGraphLinearRegionFilled.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include <QObject>

SdModeCLinearRegionFilled::SdModeCLinearRegionFilled(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinearRegion( editor, obj )
  {

  }


QString SdModeCLinearRegionFilled::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of filled region") : QObject::tr("Enter first point of filled region");
  }




QString SdModeCLinearRegionFilled::getModeThema() const
  {
  return QString( MODE_HELP "ModeCLinearRegionFilled.htm" );
  }




QString SdModeCLinearRegionFilled::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCLinearRegionFilled.htm#nextPoint" ) : QString( MODE_HELP "ModeCLinearRegionFilled.htm#firstPoint" );
  }




int SdModeCLinearRegionFilled::getIndex() const
  {
  return MD_FILL_REGION;
  }




void SdModeCLinearRegionFilled::addRegion()
  {
  addPic( new SdGraphLinearRegionFilled( mList, *sdGlobalProp->propLine( mObject->getClass() ) ), QObject::tr("Insert filled region") );
  }

