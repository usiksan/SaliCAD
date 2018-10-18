#include "SdModeCViaEnter.h"

SdModeCViaEnter::SdModeCViaEnter(SdWEditorGraph *editor, SdProjectItem *obj)
  {

  }


void SdModeCViaEnter::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mProp.mStratum, getStep() ? mProp.mNetName.str() : QString() );
  }

int SdModeCViaEnter::getPropBarId() const
  {
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
  }

QString SdModeCViaEnter::getStepHelp() const
  {
  }

QString SdModeCViaEnter::getModeThema() const
  {
  }

QString SdModeCViaEnter::getStepThema() const
  {
  }

int SdModeCViaEnter::getCursor() const
  {
  }

int SdModeCViaEnter::getIndex() const
  {
  }
