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
#include "SdDPadMaster.h"
#include "objects/SdUtil.h"
#include "objects/SdEnvir.h"

#include <QLineEdit>
#include <QToolButton>

static SdStringHistory prevWidth;

static SdStringHistory padTypeHistory;


SdPropBarRoad::SdPropBarRoad(const QString title, bool asRoad) :
  SdPropBarStratum( title )
  {
  if( asRoad ) {
    QToolButton *but;
    addWidget( but = new QToolButton() );
    but->setText( tr("Road width:") );
    but->setToolTip( tr("Activate road width editor") );

    mWidth = new QComboBox();
    mWidth->setEditable(true);
    mWidth->setToolTip( tr("Current road width editor") );
    //Fill width list with previous values
    if( prevWidth.count() == 0 )
      prevWidth.addDouble( 0.0 );
    for( const QString &str : std::as_const( prevWidth ) )
      mWidth->addItem( str );
    //Select first item
    mWidth->setCurrentIndex(0);
    mWidth->lineEdit()->setValidator( new QRegularExpressionValidator( QRegularExpression("[0-9]{1,3}((\\.|\\,)[0-9]{0,3})?")) );
    mWidth->setMinimumWidth(80);

    //Activate road width editor
    connect( but, &QToolButton::clicked, this, [this] () {
      mWidth->lineEdit()->setFocus();
      mWidth->lineEdit()->selectAll();
      });
    //on complete editing
    connect( mWidth->lineEdit(), &QLineEdit::editingFinished, this, [this]() {
      prevWidth.reorderComboBoxDoubleString( mWidth );
      emit propChanged();
      });
    //on select other width
    connect( mWidth, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, [this](int){
      prevWidth.reorderComboBoxDoubleString( mWidth );
      emit propChanged();
      });

    addWidget( mWidth );

    addSeparator();
    }


  mWireName = new QLineEdit();
  mWireName->setReadOnly(true);
  mWireName->setToolTip( tr("Current road net name") );
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

  //Enable-disable automatic road loop detection and removing
  mLoopDetection = addAction( QIcon(QString(":/pic/objRemoveLoop.png")), tr("On-off automatic road loop detection and removing") );
  mLoopDetection->setCheckable(true);
  connect( mLoopDetection, &QAction::triggered, this, [this](bool checked) {
    sdEnvir->mAutoRemoveRoadLoop = checked;
    emit propChanged();
    });

  if( asRoad ) {
    //Vertex type of two lines
    mEnterOrtho = addAction( QIcon(QString(":/pic/dleOrto.png")), tr("lines connects orthogonal") );
    mEnterOrtho->setCheckable(true);
    connect( mEnterOrtho, &QAction::triggered, this, [this](bool checked){
      Q_UNUSED(checked)
      setVertexType( dleOrtho );
      emit propChanged();
      });

    mEnter45degree = addAction( QIcon(QString(":/pic/dle45.png")), tr("lines connects at 45 degree") );
    mEnter45degree->setCheckable(true);
    connect( mEnter45degree, &QAction::triggered, this, [this](bool checked){
      Q_UNUSED(checked)
      setVertexType( dle45degree );
      emit propChanged();
      });

    mEnterAnyDegree = addAction( QIcon(QString(":/pic/dleAngle.png")), tr("lines connects at any degree") );
    mEnterAnyDegree->setCheckable(true);
    connect( mEnterAnyDegree, &QAction::triggered, this, [this](bool checked){
      Q_UNUSED(checked)
      setVertexType( dleAnyDegree );
      emit propChanged();
      });

    }

  addSeparator();

  //Vias
  //Via through or blind
  mViaThrough = addAction( QIcon(QString(":/pic/iconViaThrow.png")), tr("Via through or blind") );
  mViaThrough->setCheckable(true);
  connect( mViaThrough, &QAction::triggered, this, [this](bool checked){
    Q_UNUSED(checked)
    emit propChanged();
    });

  addSeparator();

  //Via pad type
  mViaPadType = new QComboBox();
  mViaPadType->setEditable(true);
  mViaPadType->setToolTip( tr("Via pad type") );
  for( const QString &str : std::as_const( padTypeHistory ) )
    mViaPadType->addItem( str );
  mViaPadType->setMinimumWidth(180);


  //on select other pin type
  connect( mViaPadType, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, [this](int){
    padTypeHistory.reorderComboBoxString( mViaPadType );
    emit propChanged();
    });
  connect( mViaPadType->lineEdit(), &QLineEdit::editingFinished, this, [this](){
    padTypeHistory.reorderComboBoxString( mViaPadType );
    emit propChanged();
    });
  addWidget( mViaPadType );

  //Button to select pin type from default pad association table
  QToolButton *but = new QToolButton();
  but->setText( QStringLiteral("...") );
  but->setToolTip( tr("Edit via pad type") );
  connect( but, &QToolButton::clicked, this, [this] () {
    QString str = SdDPadMaster::build( mViaPadType->currentText(), this );
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
    mLoopDetection->setChecked( sdEnvir->mAutoRemoveRoadLoop );

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





void SdPropBarRoad::setPropVia(SdPropVia *propVia)
  {
  if( propVia ) {
    //Set current stratum
    setSelectedStratum( propVia->mStratum );

    mAlignToGrid->setChecked( sdEnvir->mCursorAlignGrid );

    //Current road name name
    mWireName->setText( propVia->mNetName.str() );

    //Current via type
    mViaPadType->setCurrentText( propVia->mPadType.str() );
    padTypeHistory.reorderComboBoxString( mViaPadType );
    }
  }





void SdPropBarRoad::getPropVia(SdPropVia *propVia)
  {
  if( propVia ) {
//    int stratum = getSelectedStratum();
//    if( stratum )
//      propVia->mStratum = stratum;

    if( !mViaPadType->currentText().isEmpty() )
      propVia->mPadType = mViaPadType->currentText();

    //TODO D056 Append via through-blink support
    propVia->mStratum = stmThrough;

    //Net name not used
    }
  }




void SdPropBarRoad::setViaRule(int viaRule)
  {
  mViaThrough->setChecked( viaRule & stmRuleHided );
  }




int SdPropBarRoad::getViaRule()
  {
  return mViaThrough->isChecked() ? stmRuleHided : 0;
  }







void SdPropBarRoad::setVertexType(int type)
  {
  mEnterOrtho->setChecked( type == dleOrtho );
  mEnter45degree->setChecked( type == dle45degree );
  mEnterAnyDegree->setChecked( type == dleAnyDegree );
  }





