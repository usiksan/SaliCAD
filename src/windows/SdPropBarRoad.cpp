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
    SdEnvir::instance()->mCursorAlignGrid = checked;
    });

  //Enable-disable automatic road loop detection and removing
  mLoopDetection = addAction( QIcon(QString(":/pic/objRemoveLoop.png")), tr("On-off automatic road loop detection and removing") );
  mLoopDetection->setCheckable(true);
  connect( mLoopDetection, &QAction::triggered, this, [this](bool checked) {
    SdEnvir::instance()->mAutoRemoveRoadLoop = checked;
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




void SdPropBarRoad::setPropRoad(const SdPropRoad &propRoad, const SdPropVia &propVia, double ppm, int enterType)
  {
  SdPropComposerRoad propComposerRoad;
  propComposerRoad.reset( propRoad );
  SdPropComposerVia propComposerVia;
  propComposerVia.reset( propVia );
  setPropRoad( propComposerRoad, propComposerVia, ppm, enterType );
  }





void SdPropBarRoad::getPropRoad(SdPropRoad &propRoad, SdPropVia &propVia, int *enterType)
  {
  SdPropComposerRoad propComposerRoad;
  SdPropComposerVia propComposerVia;
  getPropRoad( propComposerRoad, propComposerVia, enterType );
  propComposerRoad.store( propRoad );
  propComposerVia.store( propVia );
  }



void SdPropBarRoad::setPropRoad(const SdPropComposerRoad &propRoad, const SdPropComposerVia &propVia, double ppm, int enterType)
  {
  //Set current stratum
  auto &propStratum = propRoad.get<&SdPropRoad::mStratum>();
  setSelectedStratum( propStratum.isSingle() ? propStratum.value() : SdPvStratum(0) );

  //Set current width
  mPPM = ppm;
  auto &propWidth = propRoad.get<&SdPropRoad::mWidth>();
  if( propWidth.isSingle() ) {
    mWidth->setCurrentText( propWidth.value().log2Phis(mPPM)  );
    prevWidth.reorderComboBoxDoubleString( mWidth );
    }
  else
    mWidth->setCurrentText( QString()  );

  mAlignToGrid->setChecked( SdEnvir::instance()->mCursorAlignGrid );
  mLoopDetection->setChecked( SdEnvir::instance()->mAutoRemoveRoadLoop );

  //line enter type
  setVertexType( enterType );

  //Current road name name
  auto &propNetName = propRoad.get<&SdPropRoad::mNetName>();
  mWireName->setText( propNetName.isSingle() ? propNetName.value.string() : QString{} );

  //Current via type
  auto &propPadType = propVia.get<&SdPropVia::mPadType>();
  mViaPadType->setCurrentText( propPadType.isSingle() ? propPadType.value().string() : QString{} );
  padTypeHistory.reorderComboBoxString( mViaPadType );
  }



void SdPropBarRoad::getPropRoad(SdPropComposerRoad &propRoad, SdPropComposerVia &propVia, int *enterType)
  {
  propRoad.clear();
  propVia.clear();
  int stratum = getSelectedStratum();
  if( stratum )
    propRoad.get<&SdPropRoad::mStratum>().reset(stratum);

  //Store width if setted
  if( !mWidth->currentText().isEmpty() )
    propRoad.get<&SdPropRoad::mWidth>().reset( SdPvInt( mWidth->currentText(), mPPM ) );

  if( !mViaPadType->currentText().isEmpty() )
    propVia.get<&SdPropVia::mPadType>().reset( mViaPadType->currentText() );

  //TODO D056 Append via through-blink support
  propVia.get<&SdPropVia::mStratum>().reset(stmThrough);

  //Net name not used
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

    mAlignToGrid->setChecked( SdEnvir::instance()->mCursorAlignGrid );

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





