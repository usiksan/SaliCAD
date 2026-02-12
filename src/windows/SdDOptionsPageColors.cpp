/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Display and selecting system colors
*/
#include "SdConfig.h"
#include "SdDOptionsPageColors.h"
#include "objects/SdEnvir.h"
#include <QTableWidgetItem>
#include <QColorDialog>
#include <QSettings>
#include <QVBoxLayout>

SdDOptionsPageColors::SdDOptionsPageColors(QWidget *parent) :
  QWidget(parent)
  {
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mTable = new QTableWidget( scLast, 2, this );
  //Table title
  mTable->setHorizontalHeaderLabels( { tr("Description"), tr("Color") } );
  mTable->setColumnWidth( 0, 300 );
  //Fill color table
  addColor( scLocked,     tr("Locked objects") );
  addColor( scSelected,   tr("Selected objects") );
  addColor( scEnter,      tr("Object color when entered") );
  addColor( scSmart,      tr("Smart mode remarks when entering") );
  addColor( scGraphBack,  tr("Graph editor background color") );
  addColor( scRatNet,     tr("Rat net color in pcb editor") );
  addColor( scCursor,     tr("Cursor color when cross-hair mode") );
  addColor( scTraseNet,   tr("Tracing net color") );
  addColor( scGrid,       tr("Grid color in graph editors") );
  addColor( scRuleErrors, tr("Color for rule errors") );
  addColor( scCatchPoint, tr("Catch point color in road enter mode") );
  addColor( sc3dPadTop,   tr("Color of top pad for 3d view") );
  addColor( sc3dPadBot,   tr("Color of bottom pad for 3d view") );
  addColor( sc3dPadHole,  tr("Color of pad holes for 3d view") );
  addColor( sc3dPcb,      tr("Color of pcb for 3d view") );

  mainLayout->addWidget( mTable );
  setLayout( mainLayout );

  connect( mTable, &QTableWidget::cellClicked, this, &SdDOptionsPageColors::onColorSelect );
  }




//Activate on select cell in color table
//We shows color dialog
void SdDOptionsPageColors::onColorSelect(int row, int column)
  {
  if( column ) {
    QColor color = QColorDialog::getColor( SdEnvir::instance()->getSysColor(row), this, mTable->item(row,0)->text() );
    if( color.isValid() )
      mTable->item(row,1)->setBackground( color );
    }
  }




void SdDOptionsPageColors::accept()
  {
  for( int i = 0; i < mTable->rowCount(); i++ )
    SdEnvir::instance()->setSysColor( i, mTable->item(i,1)->background().color() );
  }



void SdDOptionsPageColors::addColor(int colorIndex, const QString descr)
  {
  mTable->setRowHeight( colorIndex, 20 );
  QTableWidgetItem *item;
  mTable->setItem( colorIndex, 0, item = new QTableWidgetItem(descr) );
  item->setFlags( Qt::ItemIsEnabled );
  mTable->setItem( colorIndex, 1, item = new QTableWidgetItem() );
  item->setBackground( SdEnvir::instance()->getSysColor(colorIndex) );
  item->setFlags( Qt::ItemIsEnabled );
  }

