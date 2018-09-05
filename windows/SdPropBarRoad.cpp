/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for tracing road
*/
#include "SdPropBarRoad.h"
#include "SdStringHistory.h"
#include "SdDPads.h"
#include "objects/SdUtil.h"
#include "objects/SdEnvir.h"

#include <QLineEdit>
#include <QToolButton>

static SdStringHistory prevWidth;

static SdStringHistory padTypeHistory;


SdPropBarRoad::SdPropBarRoad(const QString title) :
  SdPropBarStratum( title )
  {
  mWidth = new QComboBox();
  mWidth->setEditable(true);
  //Fill width list with previous values
  if( prevWidth.count() == 0 )
    prevWidth.addDouble( 0.0 );
  for( const QString &str : prevWidth )
    mWidth->addItem( str );
  //Select first item
  mWidth->setCurrentIndex(0);
  mWidth->lineEdit()->setValidator( new QRegExpValidator( QRegExp("[0-9]{1,3}((\\.|\\,)[0-9]{0,3})?")) );
  mWidth->setMinimumWidth(80);

  //on complete editing
  connect( mWidth->lineEdit(), &QLineEdit::editingFinished, [=]() {
    prevWidth.reorderComboBoxDoubleString( mWidth );
    emit propChanged();
    });
  //on select other width
  connect( mWidth, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int){
    prevWidth.reorderComboBoxDoubleString( mWidth );
    emit propChanged();
    });

  addWidget( mWidth );

  addSeparator();


  mWireName = new QLineEdit();
  mWireName->setReadOnly(true);
  mWireName->setMinimumWidth(80);
  mWireName->setMaximumWidth(120);

  addWidget( mWireName );

  addSeparator();

  //Cursor align to greed or not
  mAlignToGrid = addAction( QIcon(QString(":/pic/alignGrid.png")), tr("Align cursor to grid") );
  mAlignToGrid->setCheckable(true);
  connect( mAlignToGrid, &QAction::triggered, [=](bool checked){
    sdEnvir->mCursorAlignGrid = checked;
    });


  //Vertex type of two lines
  mEnterOrtho = addAction( QIcon(QString(":/pic/dleOrto.png")), tr("lines connects orthogonal") );
  mEnterOrtho->setCheckable(true);
  connect( mEnterOrtho, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setVertexType( dleOrtho );
    emit propChanged();
    });

  mEnter45degree = addAction( QIcon(QString(":/pic/dle45.png")), tr("lines connects at 45 degree") );
  mEnter45degree->setCheckable(true);
  connect( mEnter45degree, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setVertexType( dle45degree );
    emit propChanged();
    });

  mEnterAnyDegree = addAction( QIcon(QString(":/pic/dleAngle.png")), tr("lines connects at any degree") );
  mEnterAnyDegree->setCheckable(true);
  connect( mEnterAnyDegree, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setVertexType( dleAnyDegree );
    emit propChanged();
    });


  addSeparator();

  //Vias
  //Via through or blind
  mViaThrough = addAction( QIcon(QString(":/pic/dleOrto.png")), tr("Via through or blind") );
  mViaThrough->setCheckable(true);
  connect( mViaThrough, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    emit propChanged();
    });

  addSeparator();

  //Via pad type
  mViaPadType = new QComboBox();
  mViaPadType->setEditable(true);
  for( const QString &str : padTypeHistory )
    mViaPadType->addItem( str );
  mViaPadType->setMinimumWidth(180);


  //on select other pin type
  connect( mViaPadType, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int){
    padTypeHistory.reorderComboBoxString( mViaPadType );
    emit propChanged();
    });
  connect( mViaPadType->lineEdit(), &QLineEdit::editingFinished, [=](){
    padTypeHistory.reorderComboBoxString( mViaPadType );
    emit propChanged();
    });
  addWidget( mViaPadType );

  //Button to select pin type from default pad association table
  QToolButton *but = new QToolButton();
  but->setText( QStringLiteral("...") );
  connect( but, &QToolButton::clicked, this, [this] () {
    QString str = SdDPads::selectPlatePinType( mPlate, this );
    if( !str.isEmpty() ) {
      mViaPadType->setCurrentText( str );
      padTypeHistory.reorderComboBoxString( mViaPadType );
      emit propChanged();
      }
    } );
  addWidget( but );

  }




void SdPropBarRoad::setPropRoad(SdPropRoad *propRoad, SdPropVia *propVia, double ppm, int enterType)
  {
  if( propRoad && propVia ) {
    //Set current stratum
    setSelectedStratum( propRoad->mStratum );

    //Set current width
    mPPM = ppm;
    if( propRoad->mWidth.isValid() ) {
      mWidth->setCurrentText( propRoad->mWidth.log2Phis(mPPM)  );
      prevWidth.reorderComboBoxDoubleString( mWidth );
      }
    else
      mWidth->setCurrentText( QString()  );

    mAlignToGrid->setChecked( sdEnvir->mCursorAlignGrid );

    //line enter type
    setVertexType( enterType );

    //Current road name name
    mWireName->setText( propRoad->mNetName.str() );

    //Current via type
    mViaPadType->setCurrentText( propVia->mPadType.str() );
    padTypeHistory.reorderComboBoxString( mViaPadType );


    }
  }





void SdPropBarRoad::getPropRoad(SdPropRoad *propRoad, SdPropVia *propVia, int *enterType)
  {
  if( propRoad && propVia ) {
    int stratum = getSelectedStratum();
    if( stratum )
      propRoad->mStratum = stratum;

    if( !mWidth->currentText().isEmpty() )
      propRoad->mWidth.setFromPhis( mWidth->currentText(), mPPM );

    if( !mViaPadType->currentText().isEmpty() )
      propVia->mPadType = mViaPadType->currentText();

    //TODO D056 Append via through-blink support
    propVia->mStratum = stmThrough;

    //Net name not used
    }
  if( enterType ) {
    if( mEnterOrtho->isChecked() ) *enterType = dleOrtho;
    else if( mEnter45degree->isChecked() ) *enterType = dle45degree;
    else *enterType = dleAnyDegree;
    }
  }







void SdPropBarRoad::setVertexType(int type)
  {
  mEnterOrtho->setChecked( type == dleOrtho );
  mEnter45degree->setChecked( type == dle45degree );
  mEnterAnyDegree->setChecked( type == dleAnyDegree );
  }





