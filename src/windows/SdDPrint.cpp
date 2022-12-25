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
#include "objects/SdUtil.h"
#include "SdDHelp.h"

#include <QtPrintSupport/QPageSetupDialog>
#include <QtPrintSupport/QPrintDialog>



SdDPrint::SdDPrint(SdRect over, SdRect sel, SdRect wnd, double ppm, QPrinter *printer,  QWidget *parent) :
  QDialog(parent),
  mOver(over),
  mSel(sel),
  mWin(wnd),
  mPPM(ppm),
  mPrinter( printer ),
  ui(new Ui::SdDPrint)
  {
  ui->setupUi(this);

  //Printer info setup
  ui->mPrinter->setText( mPrinter->printerName() );

  connect( ui->mPrinterOptions, &QPushButton::clicked, this, [this] () {
//    QPageSetupDialog setupDlg( mPrinter, this );
    QPrintDialog setupDlg( mPrinter, this );
    setupDlg.exec();
    //Update printer name
    ui->mPrinter->setText( mPrinter->printerName() );
    //Update copy count
    ui->mCopyCount->setValue( mPrinter->copyCount() );
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




  ui->mScaleFit->setChecked( true );
  ui->mScaleUser->setChecked( false );
  ui->mScaleValue->setEnabled( false );
  connect( ui->mScaleFit, &QRadioButton::toggled, this, [this] (bool checked) {
    ui->mScaleFit->setChecked( checked );
    ui->mScaleUser->setChecked( !checked );
    ui->mScaleValue->setEnabled( !checked );
    });
  connect( ui->mScaleUser, &QRadioButton::toggled, this, [this](bool checked) {
    ui->mScaleFit->setChecked( !checked );
    ui->mScaleUser->setChecked( checked );
    ui->mScaleValue->setEnabled( checked );
    });




  //Area setup and init
  if( sel.width() == 0 || sel.height() == 0 )
    //Disable selection
    ui->mAreaSelection->setEnabled(false);
  connect( ui->mAreaFull, &QRadioButton::toggled, this, [this] (bool checked) {
    if( checked )
      updatePrintArea( SDPA_FULL_OBJECT, mOver );
    });
  connect( ui->mAreaSelection, &QRadioButton::toggled, this, [this] (bool checked) {
    if( checked )
      updatePrintArea( SDPA_SELECTION, mSel );
    });
  connect( ui->mAreaWindow, &QRadioButton::toggled, this, [this] (bool checked) {
    if( checked )
      updatePrintArea( SDPA_WINDOW, mWin );
    });

  if( mWin.width() && mWin.height() )
    updatePrintArea( SDPA_WINDOW, mWin );
  else if( sel.width() && sel.height() )
    updatePrintArea( SDPA_SELECTION, mSel );
  else {
    mWin = mOver;
    updatePrintArea( SDPA_FULL_OBJECT, mOver );
    }

  connect( ui->mAreaWindowSelect, &QPushButton::clicked, this, [this] (bool) {
    done(2);
    });



  //Autoselect best orientation
  updateOrientation( false );
  double vertScale = getScaleFactor();
  updateOrientation( true );
  double horzScale = getScaleFactor();
  if( horzScale < vertScale )
    updateOrientation( false );




  //Copy count
  ui->mCopyCount->setValue(1);
  ui->mCopyCount->setRange(1,100);
  connect( ui->mCopyCount, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this] (int val) {
    mPrinter->setCopyCount(val);
    });


  //Zero width line
  ui->mZeroLineWidth->setValue(0);
  ui->mZeroLineWidth->setRange(0,100);

  connect( ui->buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDPrint.htm"), this );
    } );

  }



SdDPrint::~SdDPrint()
  {
  delete ui;
  }




SdRect SdDPrint::getWindow()
  {
  //+-10 is for exclude clipping at area edges
  int left = SdUtil::phys2log( ui->mAreaWindowLeft->text(), mPPM ) - 10;
  int bottom = SdUtil::phys2log( ui->mAreaWindowBottom->text(), mPPM ) - 10;
  int right = SdUtil::phys2log( ui->mAreaWindowRight->text(), mPPM ) + 10;
  int top = SdUtil::phys2log( ui->mAreaWindowTop->text(), mPPM ) + 10;
  return SdRect( SdPoint(left,bottom), SdPoint(right,top) );
  }




//Selected print area code
int SdDPrint::getPrintArea()
  {
  if( ui->mAreaFull->isChecked() )
    return SDPA_FULL_OBJECT;
  if( ui->mAreaSelection->isChecked() )
    return SDPA_SELECTION;
  return SDPA_WINDOW;
  }




//Return current scale factor
double SdDPrint::getScaleFactor()
  {
  return SdUtil::str2phys( ui->mScaleValue->text() );
  }




//Return color print flag
bool SdDPrint::isColor() const
  {
  return ui->mColorPrint->isChecked();
  }




//Return zero width line conversion width
int SdDPrint::getZeroWidth() const
  {
  return ui->mZeroLineWidth->value();
  }




//!
//! \brief isMirrorHorz Return current state of mirrored print
//! \return             When true must be mirrored on horizontal axiz
//!
bool SdDPrint::isMirrorHorz() const
  {
  return ui->mMirrorHorz->isChecked();
  }



//Setup page orientation
void SdDPrint::updateOrientation(bool horizontal)
  {
  ui->mOrientationHorizontal->setChecked( horizontal );
  ui->mOrientationVertical->setChecked( !horizontal );
  if( horizontal ) {
    ui->mOrientationIcon->setPixmap( QPixmap(QStringLiteral(":/pic/printHorz.png")) );
    mPrinter->setOrientation( QPrinter::Landscape );
    }
  else {
    ui->mOrientationIcon->setPixmap( QPixmap(QStringLiteral(":/pic/printVert.png")) );
    mPrinter->setOrientation( QPrinter::Portrait );
    }

  updateScaleFactor();
  }




void SdDPrint::updatePrintArea(int area, SdRect ovr)
  {
  //Check area
  ui->mAreaFull->setChecked( area == SDPA_FULL_OBJECT );
  ui->mAreaSelection->setChecked( area == SDPA_SELECTION );
  ui->mAreaWindow->setChecked( area == SDPA_WINDOW );
  //Setup window coords
  ui->mAreaWindowBottom->setText( SdUtil::log2physStr(ovr.getBottom(), mPPM)  );
  ui->mAreaWindowLeft->setText( SdUtil::log2physStr(ovr.getLeft(), mPPM) );
  ui->mAreaWindowRight->setText( SdUtil::log2physStr(ovr.getRight(), mPPM) );
  ui->mAreaWindowTop->setText( SdUtil::log2physStr(ovr.getTop(), mPPM) );
  ui->mAreaWindowSelect->setEnabled( area == SDPA_WINDOW );

  updateScaleFactor();
  }



void SdDPrint::updateScaleFactor()
  {
  if( ui->mScaleFit->isChecked() ) {
    QRectF r = mPrinter->pageRect( QPrinter::DevicePixel );
    SdRect wnd = getWindow();
    //Calculate scale factor log window to page
    double dw = r.width();
    double sw = wnd.width();
    if( sw == 0 ) sw = 1.0;
    double scalew = dw / sw;

    double dh = r.height();
    double sh = wnd.height();
    if( sh == 0 ) sh = 1.0;
    double scaleh = dh / sh;

    double scale = qMin( scalew, scaleh );
    ui->mScaleValue->setText( QString::number(scale, 'f', 3) );
    }
  }

