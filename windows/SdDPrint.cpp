/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Print dialog
*/
#include "SdDPrint.h"
#include "ui_SdDPrint.h"

#include <QtPrintSupport/QPageSetupDialog>



SdDPrint::SdDPrint(SdRect over, SdRect sel, SdRect wnd, double ppm, QPrinter *printer,  QWidget *parent) :
  QDialog(parent),
  mPrinter( printer ),
  ui(new Ui::SdDPrint)
  {
  ui->setupUi(this);

  //Printer info setup
  ui->mPrinter->setText( mPrinter->printerName() );

  connect( ui->mPrinterOptions, &QPushButton::clicked, this, [this] () {
    QPageSetupDialog setupDlg( mPrinter, this );
    setupDlg.exec();
    //Update page orientation
    updateOrientation( mPrinter->pageLayout().orientation() == QPageLayout::Landscape );
    });

  //Orientation setup and init
  connect( ui->mOrientationHorizontal, &QRadioButton::toggled, this, [this] ( bool checked) {
    updateOrientation(checked);
    });
  connect( ui->mOrientationVertical, &QRadioButton::toggled, this, [this] ( bool checked) {
    updateOrientation(!checked);
    });

  updateOrientation( true );
  }



SdDPrint::~SdDPrint()
  {
  delete ui;
  }



//Setup page orientation
void SdDPrint::updateOrientation(bool horizontal)
  {
  ui->mOrientationHorizontal->setChecked( horizontal );
  ui->mOrientationVertical->setChecked( !horizontal );
  if( horizontal )
    ui->mOrientationIcon->setPixmap( QPixmap(QStringLiteral(":/pic/printHorz.png")) );
  else
    ui->mOrientationIcon->setPixmap( QPixmap(QStringLiteral(":/pic/printVert.png")) );
  }




void SdDPrint::updatePrintArea(int area, SdRect ovr)
  {
  //Check area
  ui->mAreaFull->setChecked( area == SDPA_FULL_OBJECT );
  ui->mAreaSelection->setChecked( area == SDPA_SELECTION );
  ui->mAreaWindow->setChecked( area == SDPA_WINDOW );
  //Setup window coords
  ui->mAreaWindowBottom->setText( log2physStr(ovr.getBottom()) );
  ui->mAreaW
  }
