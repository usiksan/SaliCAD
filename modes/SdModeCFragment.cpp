/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCFragment.h"

SdModeCFragment::SdModeCFragment(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }


void SdModeCFragment::activate()
  {
  }

void SdModeCFragment::reset()
  {
  }

void SdModeCFragment::drawStatic(SdContext *ctx)
  {
  }

void SdModeCFragment::drawDynamic(SdContext *ctx)
  {
  }

int SdModeCFragment::getPropBarId() const
  {
  }

void SdModeCFragment::enterPoint(SdPoint)
  {
  }

void SdModeCFragment::cancelPoint(SdPoint)
  {
  }

void SdModeCFragment::movePoint(SdPoint)
  {
  }

QString SdModeCFragment::getStepHelp() const
  {
  }

QString SdModeCFragment::getModeThema() const
  {
  }

QString SdModeCFragment::getStepThema() const
  {
  }

int SdModeCFragment::getCursor() const
  {
  }

int SdModeCFragment::getIndex() const
  {
  }
