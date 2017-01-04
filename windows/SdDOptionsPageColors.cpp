/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConfig.h"
#include "SdDOptionsPageColors.h"
#include <QTableWidgetItem>
#include <QColorDialog>
#include <QSettings>
#include <QVBoxLayout>

SdDOptionsPageColors::SdDOptionsPageColors(QWidget *parent) :
  QWidget(parent)
  {
  mTable = new QTableWidget( 2, 2, this );
  }

