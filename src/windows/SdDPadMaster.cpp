/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "objects/SdEnvir.h"

#include "SdDPadMaster.h"
#include "ui_SdDPadMaster.h"
#include "SdIllustrator.h"
#include "SdDHelp.h"

#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>

SdDPadMaster::SdDPadMaster(SdPad pad, QWidget *parent) :
  QDialog(parent),
  mPad(pad),
  ui(new Ui::SdDPadMaster)
  {
  //Build ui
  ui->setupUi(this);

  //Fill fields with values
  ui->mCenterX->setText( sdEnvir->toPhisPcb(mPad.mCenterX) );
  ui->mCenterY->setText( sdEnvir->toPhisPcb(mPad.mCenterY) );
  ui->mPadDiametr->setText( sdEnvir->toPhisPcb(mPad.mDiametrWidth) );
  ui->mCirclePad->setChecked(mPad.mIsCircle);
  onPadCircleToggle(mPad.mIsCircle);
  ui->mMaskThreshold->setText( sdEnvir->toPhisPcb(mPad.mMaskThreshold) );
  onThroughPin( mPad.mHoleDiametr > 0 );
  ui->mThroughPin->setChecked( mPad.isThrough() );

  //Connect signals
  connect( ui->mCirclePad, &QCheckBox::toggled, this, &SdDPadMaster::onPadCircleToggle );
  connect( ui->mThroughPin, &QCheckBox::toggled, this, &SdDPadMaster::onThroughPin );
  connect( ui->mStensilColumns, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mStencilCols = txt.toInt();
    onArrayEnable(true);
    updatePadSchematic();
    });
  connect( ui->mStensilRows, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mStencilRows = txt.toInt();
    onArrayEnable(true);
    updatePadSchematic();
    });
  //On other editors update value from editor and update pad schematic
  connect( ui->mCenterX, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mCenterX = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });
  connect( ui->mCenterY, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mCenterY = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });
  connect( ui->mPadDiametr, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mDiametrWidth = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });
  connect( ui->mPadHeight, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mHeight = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });
  connect( ui->mMaskThreshold, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mMaskThreshold = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });
  connect( ui->mHoleDiametr, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mHoleDiametr = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });
  connect( ui->mStensilThreshold, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mStencilThreshold = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });
  connect( ui->mStensilWidth, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mStencilWidth = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });
  connect( ui->mStensilHeight, &QLineEdit::textEdited, this, [this] (QString txt) {
    mPad.mStencilHeight = sdEnvir->fromPhisPcb( txt );
    updatePadSchematic();
    });

  connect( ui->buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDPadMaster.htm"), this );
    } );
  }



SdDPadMaster::~SdDPadMaster()
  {
  delete ui;
  }




//Build pad from source name, edit it and return new name
QString SdDPadMaster::build(const QString name, QWidget *parent)
  {
  //Build source pad
  SdPad pad;
  pad.parse( name );
  //Create master dialog
  SdDPadMaster master( pad, parent );
  //Edit pad
  if( master.exec() )
    //return new name for pad
    return master.pad().name();
  //Return old name
  return name;
  }




static void drawPadProfile( SdIllustrator &ill, bool hole ) {
  //pcb
  ill.drawFillRectWH( 0, 0, 350, 60, QColor(0x007F0E) );
  ill.drawText( 3, 3, QObject::tr("PCB"), Qt::black );
  //solder mask
  ill.drawFillRectWH( 0, 60, 45, 20, QColor(0xA5FF7F) );
  ill.drawFillRect( 250-45, 60, 350, 60+20, QColor(0xA5FF7F) );
  ill.drawText( 250-45+3, 61, QObject::tr("Solder mask"), Qt::black );
  ill.drawLineDoubleArrow( 45, 105, 125-50, 105, Qt::black, 6 );
  ill.drawText( 48, 112, QString("M"), Qt::black );

  //Pad
  ill.drawFillRectWH( 125 - 50, 60, 100, 25, QColor(0xFFE97F) );
  ill.drawLineDoubleArrow( 125-50, 95, 125+50, 95, Qt::black, 6 );
  ill.drawText( 110, 97, QObject::tr("Pad"), Qt::black );

  //Hole
  if( hole ) {
    ill.drawFillRect( 125-20,0, 125+20,85, QColor(0x7F6A00) );
    ill.drawLineDoubleArrow( 125-20, -10, 125+20, -10, Qt::black, 6 );
    ill.drawText( 105, -33, QObject::tr("Hole"), Qt::black );
    }
  else {
    //Stensil
    ill.drawFillRect( 0, 140, 125-30, 160, QColor(0x808080) );
    ill.drawFillRect( 125+30, 140, 350, 160, QColor(0x808080) );
    ill.drawText( 125+40+3, 161, QObject::tr("Stensil"), Qt::black );
    ill.drawLineDoubleArrow( 125-30, 135, 125-50, 135, Qt::black, 6 );
    ill.drawText( 125-25, 122, QString("S"), Qt::black );
    }
  }


void SdDPadMaster::updatePadProfile()
  {
  SdIllustrator ill;
  drawPadProfile( ill, mPad.mHoleDiametr != 0 );
  ill.setPixmap( 250, 250, Qt::white );
  drawPadProfile( ill, mPad.mHoleDiametr != 0 );
  ui->mPadProfile->setPixmap( ill.pixmap() );
  }




void SdDPadMaster::updatePadSchematic()
  {
  SdIllustrator ill;
  drawPadSchematic( ill );
  int mx = qMax( abs(ill.getMinX()), ill.getMaxX() );
  int my = qMax( abs(ill.getMinY()), ill.getMayY() );
  mx = qMax( mx, my );
  mx = mx * 110 / 100;
  //pcb
  ill.drawFillRect( -mx, -mx, mx, mx, QColor(0x007F0E) );
  ill.setPixmap( 250, 250, QColor(0x007F0E) );
  drawPadSchematic( ill );
  ui->mPadSchematic->setPixmap( ill.pixmap() );
  }




void SdDPadMaster::drawPadSchematic(SdIllustrator &ill)
  {

  if( mPad.mIsCircle ) {
    //pad
    ill.drawFillCircle( mPad.mCenterX, mPad.mCenterY, mPad.mDiametrWidth/2, QColor(0xFFE97F) );

    //Hole if present
    if( mPad.mHoleDiametr > 0 )
      ill.drawFillCircle( mPad.mCenterX, mPad.mCenterY, mPad.mHoleDiametr/2, QColor(0x7F6A00) );

    //mask
    if( mPad.mMaskThreshold > 0 )
      ill.drawFillCircle( mPad.mCenterX, mPad.mCenterY, (mPad.mDiametrWidth + mPad.mMaskThreshold)/2, QColor(0xA5FF7F), 0.5 );

    //Stensil if present
    if( mPad.mHoleDiametr <= 0 )
      ill.drawFillCircle( mPad.mCenterX, mPad.mCenterY, (mPad.mDiametrWidth - mPad.mStencilThreshold)/2, QColor(0x808080) );
    }
  else {
    //pad
    ill.drawCenterFillRectWH( mPad.mCenterX, mPad.mCenterY,
                        mPad.mDiametrWidth, mPad.mHeight, QColor(0xFFE97F) );

    //Hole if present
    if( mPad.mHoleDiametr > 0 )
      ill.drawFillCircle( mPad.mCenterX, mPad.mCenterY, mPad.mHoleDiametr/2, QColor(0x7F6A00) );

    //mask
    if( mPad.mMaskThreshold > 0 )
      ill.drawCenterFillRectWH( mPad.mCenterX,  mPad.mCenterY,
                        mPad.mDiametrWidth+mPad.mMaskThreshold, mPad.mHeight+mPad.mMaskThreshold, QColor(0xA5FF7F), 0.5 );

    //Stensil if present
    if( mPad.mHoleDiametr <= 0 ) {
      if( mPad.mStencilCols > 1 || mPad.mStencilRows > 1 ) {
        //Draw array of apertures
        int rows = mPad.mStencilRows > 1 ? mPad.mStencilRows : 1;
        int cols = mPad.mStencilCols > 1 ? mPad.mStencilCols : 1;
        int cellx = mPad.mDiametrWidth / cols;
        int celly = mPad.mHeight / rows;
        int w = mPad.mDiametrWidth - mPad.mStencilThreshold * 2;
        int h = mPad.mHeight - mPad.mStencilThreshold * 2;
        for( int y = 0; y < rows; y++ )
          for( int x = 0; x < cols; x++ )
            ill.drawFillRectWH( mPad.mCenterX - (w >> 1) + x * cellx,
                                mPad.mCenterY - (h >> 1) + y * celly,
                                mPad.mStencilWidth,
                                mPad.mStencilHeight, QColor(0x808080) );
        }
      else
        ill.drawCenterFillRectWH( mPad.mCenterX,  mPad.mCenterY,
                            mPad.mDiametrWidth-mPad.mStencilThreshold, mPad.mHeight-mPad.mStencilThreshold, QColor(0x808080) );
      }
    }
  }




//Toggle between circle pad and rectangle pad
void SdDPadMaster::onPadCircleToggle(bool isCircle)
  {
  mPad.mIsCircle = isCircle;
  if( mPad.mIsCircle ) {
    //When pad is circle then disable pad height and replace title
    ui->mPadDiametrTitle->setText( tr("Pad diametr") );
    ui->mPadHeight->clear();
    ui->mPadHeight->setEnabled(false);
    }
  else {
    ui->mPadDiametrTitle->setText( tr("Pad witdh") );
    ui->mPadHeight->setText( sdEnvir->toPhisPcb(mPad.mHeight) );
    ui->mPadHeight->setEnabled(true);
    }
  updatePadSchematic();
  }





//Enable-disable through pin (else planar)
void SdDPadMaster::onThroughPin(bool isThrough)
  {
  ui->mHoleDiametr->setEnabled(isThrough);
  ui->mStensilThreshold->setEnabled(!isThrough);
  ui->mStensilRows->setEnabled(!isThrough);
  ui->mStensilColumns->setEnabled(!isThrough);
  onArrayEnable( !isThrough );
  if( isThrough ) {
    //Disable stensil hole
    ui->mStensilThreshold->clear();
    mPad.mStencilThreshold = 0;
    ui->mStensilRows->clear();
    ui->mStensilColumns->clear();
    //Enable pin hole
    if( mPad.mHoleDiametr <= 0 )
      mPad.mHoleDiametr = 500;
    ui->mHoleDiametr->setText( sdEnvir->toPhisPcb(mPad.mHoleDiametr) );
    }
  else {
    //Enable stensil hole
    if( mPad.mStencilThreshold <= 0 )
      mPad.mStencilThreshold = 50;
    ui->mStensilThreshold->setText( sdEnvir->toPhisPcb(mPad.mStencilThreshold) );
    ui->mStensilRows->setText( QString::number(mPad.mStencilRows) );
    ui->mStensilColumns->setText( QString::number(mPad.mStencilCols) );
    //Disable pin hole
    ui->mHoleDiametr->clear();
    mPad.mHoleDiametr = 0;
    }
  updatePadProfile();
  updatePadSchematic();
  }




//Enable-disable stensil array on isEna and current settings of stensil cols and rows
void SdDPadMaster::onArrayEnable(bool isEna)
  {
  isEna = isEna && (mPad.mStencilCols > 1 || mPad.mStencilRows > 1);
  ui->mStensilWidth->setEnabled(isEna);
  ui->mStensilHeight->setEnabled(isEna);
  if( isEna ) {
    ui->mStensilWidth->setText( sdEnvir->toPhisPcb(mPad.mStencilWidth) );
    ui->mStensilHeight->setText( sdEnvir->toPhisPcb(mPad.mStencilHeight) );
    }
  else {
    ui->mStensilWidth->clear();
    ui->mStensilHeight->clear();
    }
  }

