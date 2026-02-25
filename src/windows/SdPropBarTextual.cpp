/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for text and other text based objects
*/
#include "SdPropBarTextual.h"
#include "SdStringHistory.h"
#include "objects/SdEnvir.h"
#include <QList>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QIcon>
#include <QDebug>

static SdStringHistory prevSize;

SdPropBarTextual::SdPropBarTextual(const QString title) :
  SdPropBarLay( title )
  {

  mFont = new QComboBox();
  mFont->setEditable(false);
  //Fill font list with available fonts
  for( int i = 0; i < FONT_COUNT; i++ )
    mFont->addItem( SdEnvir::instance()->getSysFont(i) );
  //on select other font
  connect( mFont, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [this](int index){
    Q_UNUSED(index)
    emit propChanged();
    });
  addWidget( mFont );


  mSize = new QComboBox();
  mSize->setEditable(true);
  //Fill font sizes list with previous values
  if( prevSize.count() == 0 )
    prevSize.addDouble( 3.5 );
  for( const QString &v : std::as_const(prevSize) )
    mSize->addItem( v );
  //Select first item
  mSize->setCurrentIndex(0);
  mSize->lineEdit()->setValidator( new QRegularExpressionValidator( QRegularExpression("[0-9]{0,3}((\\.|\\,)[0-9]{0,3})?")) );
  mSize->setMinimumWidth(80);

  //on complete editing
  connect( mSize->lineEdit(), &QLineEdit::editingFinished, [this]() {
    prevSize.reorderComboBoxDoubleString( mSize );
    emit propChanged();
    });
  //on select other size
  connect( mSize, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [this](int) {
    prevSize.reorderComboBoxDoubleString( mSize );
    emit propChanged();
    });

  addWidget( mSize );


  addSeparator();


  //Vertical alignment
  mVertTop = addAction( QIcon(QString(":/pic/tjTop.png")), tr("Text aligned to top") );
  mVertTop->setCheckable(true);
  connect( mVertTop, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setVerticalAlignment( dvjTop );
    emit propChanged();
    });

  mVertMiddle = addAction( QIcon(QString(":/pic/tjMiddle.png")), tr("Text aligned to vertical middle") );
  mVertMiddle->setCheckable(true);
  connect( mVertMiddle, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setVerticalAlignment( dvjMiddle );
    emit propChanged();
    });

  mVertBottom = addAction( QIcon(QString(":/pic/tjBottom.png")), tr("Text aligned to bottom") );
  mVertBottom->setCheckable(true);
  connect( mVertBottom, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setVerticalAlignment( dvjBottom );
    emit propChanged();
    });

  addSeparator();


  //Horizontal alignment
  mHorzLeft = addAction( QIcon(QString(":/pic/tjLeft.png")), tr("Text aligned to left") );
  mHorzLeft->setCheckable(true);
  connect( mHorzLeft, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setHorizontalAlignment( dhjLeft );
    emit propChanged();
    });

  mHorzCenter = addAction( QIcon(QString(":/pic/tjCenter.png")), tr("Text aligned to horizontal center") );
  mHorzCenter->setCheckable(true);
  connect( mHorzCenter, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setHorizontalAlignment( dhjCenter );
    emit propChanged();
    });

  mHorzRight = addAction( QIcon(QString(":/pic/tjRight.png")), tr("Text aligned to right") );
  mHorzRight->setCheckable(true);
  connect( mHorzRight, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setHorizontalAlignment( dhjRight );
    emit propChanged();
    });

  addSeparator();

  //Text direction
  mDir0 = addAction( QIcon(QString(":/pic/textDirLR.png")), tr("Text direction left to right") );
  mDir0->setCheckable(true);
  connect( mDir0, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setDirection( da0 );
    emit propChanged();
    });

  mDir90 = addAction( QIcon(QString(":/pic/textDirBT.png")), tr("Text direction bottom to top") );
  mDir90->setCheckable(true);
  connect( mDir90, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setDirection( da90 );
    emit propChanged();
    });

  mDir180 = addAction( QIcon(QString(":/pic/textDirRL.png")), tr("Text direction right to left") );
  mDir180->setCheckable(true);
  connect( mDir180, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setDirection( da180 );
    emit propChanged();
    });

  mDir270 = addAction( QIcon(QString(":/pic/textDirTB.png")), tr("Text direction top to bottom") );
  mDir270->setCheckable(true);
  connect( mDir270, &QAction::triggered, [this](bool checked){
    Q_UNUSED(checked)
    setDirection( da270 );
    emit propChanged();
    });

  }





void SdPropBarTextual::setPropText(const SdPropText *propText, double ppm)
  {
  SdPropComposerText composerText;
  if( propText != nullptr )
    composerText.reset( *propText );
  setPropText( composerText, ppm );
  }




void SdPropBarTextual::getPropText(SdPropText *propText)
  {
  SdPropComposerText composerText;
  getPropText( composerText );
  if( propText != nullptr )
    composerText.store( *propText );
  }



void SdPropBarTextual::setPropText(const SdPropComposerText &propText, double ppm)
  {
  //Set current layer
  setSelectedLayer( propText.layer().asSingleLayerOrNull(false) );

  //Set current font
  auto &propFont = propText.get<&SdPropText::mFont>();
  setFont( propFont.isSingle() ? propFont.value().value() : -1 );

  //Set current width
  mPPM = ppm;
  auto &propSize = propText.get<&SdPropText::mSize>();
  if( propSize.isSingle() ) {
    mSize->setCurrentText( propSize.value().log2Phis(mPPM) );
    prevSize.reorderComboBoxDoubleString( mSize );
    }
  else
    mSize->setCurrentText( QString( ) );

  //Vertical alignment
  setVerticalAlignment( propText.vert().asSingleIntOrMinus() );

  //Horizontal alignment
  setHorizontalAlignment( propText.horz().asSingleIntOrMinus() );

  //Text direction
  auto &propDir = propText.get<&SdPropText::mDir>();
  setDirection( propDir.isSingle() ? propDir.value().as360() : -1 );
  }




void SdPropBarTextual::getPropText(SdPropComposerText &propText)
  {
  //Get current layer
  propText.layer().resetLayerNonNull( getSelectedLayer() );

  //Get current font
  propText.font().resetIntIfGeZero( mFont->currentIndex() );

  //Get text size
  if( !mSize->currentText().isEmpty() )
    propText.get<&SdPropText::mSize>().reset( SdPvInt( mSize->currentText(), mPPM ) );

  //Get text vertical alignment
  auto &propVert = propText.get<&SdPropText::mVert>();
  if( mVertTop->isChecked() ) propVert.reset( SdPvInt(dvjTop) );
  else if( mVertMiddle->isChecked() ) propVert.reset( SdPvInt(dvjMiddle) );
  else if( mVertBottom->isChecked() ) propVert.reset( SdPvInt(dvjBottom) );

  //Get horizontal alignment
  auto &propHorz = propText.get<&SdPropText::mHorz>();
  if( mHorzLeft->isChecked() ) propHorz.reset( SdPvInt(dhjLeft) );
  else if( mHorzCenter->isChecked() ) propHorz.reset( SdPvInt(dhjCenter) );
  else if( mHorzRight->isChecked() ) propHorz.reset( SdPvInt(dhjRight) );

  //Get direction
  auto &propDir = propText.get<&SdPropText::mDir>();
  if( mDir0->isChecked() ) propDir.reset( SdPvAngle(da0) );
  else if( mDir90->isChecked() ) propDir.reset( SdPvAngle(da90) );
  else if( mDir180->isChecked() ) propDir.reset( SdPvAngle(da180) );
  else if( mDir270->isChecked() ) propDir.reset( SdPvAngle(da270) );
  }




void SdPropBarTextual::setFont(int index)
  {
  if( index >= 0 && index < FONT_COUNT )
    mFont->setCurrentIndex( index );
  else
    mFont->setCurrentIndex( -1 );
  }








void SdPropBarTextual::setVerticalAlignment(int vert)
  {
  mVertTop->setChecked( vert == dvjTop );
  mVertMiddle->setChecked( vert == dvjMiddle );
  mVertBottom->setChecked( vert == dvjBottom );
  }







void SdPropBarTextual::setHorizontalAlignment(int horz)
  {
  mHorzLeft->setChecked( horz == dhjLeft );
  mHorzCenter->setChecked( horz == dhjCenter );
  mHorzRight->setChecked( horz == dhjRight );
  }







void SdPropBarTextual::setDirection(int dir)
  {
  mDir0->setChecked( dir == da0 );
  mDir90->setChecked( dir == da90 );
  mDir180->setChecked( dir == da180 );
  mDir270->setChecked( dir == da270 );
  }
