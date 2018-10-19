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

SdModeCViaEnter::SdModeCViaEnter(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }





void SdModeCViaEnter::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mViaProp.mStratum, QString() );
  }




int SdModeCViaEnter::getPropBarId() const
  {
  return PB_ROAD;
  }




void SdModeCViaEnter::propGetFromBar()
  {
  }




void SdModeCViaEnter::propSetToBar()
  {
  }




void SdModeCViaEnter::enterPoint(SdPoint)
  {
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
