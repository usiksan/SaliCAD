/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModePrintWindow.h"

SdModePrintWindow::SdModePrintWindow(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeTemp( editor, obj )
  {

  }


void SdModePrintWindow::drawDynamic(SdContext *ctx)
  {
  }

void SdModePrintWindow::enterPoint(SdPoint)
  {
  }

void SdModePrintWindow::cancelPoint(SdPoint)
  {
  }

void SdModePrintWindow::movePoint(SdPoint)
  {
  }

QString SdModePrintWindow::getStepHelp() const
  {
  }

QString SdModePrintWindow::getModeThema() const
  {
  }

QString SdModePrintWindow::getStepThema() const
  {
  }

int SdModePrintWindow::getCursor() const
  {
  }

int SdModePrintWindow::getIndex() const
  {
  }
