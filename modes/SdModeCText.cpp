/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for text edit
*/
#include "SdModeCText.h"

SdModeCText::SdModeCText(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCTextual( editor, obj )
  {

  }


void SdModeCText::drawDynamic(SdContext *ctx)
  {
  }

void SdModeCText::enterPoint(SdPoint)
  {
  }

void SdModeCText::cancelPoint(SdPoint)
  {
  }

QString SdModeCText::getStepHelp() const
  {
  }

QString SdModeCText::getModeHelp() const
  {
  }

QString SdModeCText::getStepThema() const
  {
  }

int SdModeCText::getIndex() const
  {
  }

void SdModeCText::cancelEdit()
  {
  }

void SdModeCText::applyEdit()
  {
  }
