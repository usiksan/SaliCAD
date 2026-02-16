/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for wires
*/
#include "SdPropBarWire.h"
#include "SdStringHistory.h"
#include "objects/SdProp.h"

#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>

static SdStringHistory prevWidth;

static SdStringHistory prevWires;

SdPropBarWire::SdPropBarWire( const QString title ) :
  SdPropBar( title )
  {
  QToolButton *but;
  addWidget( but = new QToolButton() );
  but->setText( tr("Wire line width:") );
  but->setToolTip( tr("Activate width editor") );
  mWidth = new QComboBox();
  mWidth->setEditable(true);
  mWidth->setToolTip( tr("Wire line width editor") );
  //Fill width list with previous values
  if( prevWidth.count() == 0 )
    prevWidth.addDouble( 0.0 );
  for( const QString &v : prevWidth )
    mWidth->addItem( v );
  //Select first item
  mWidth->setCurrentIndex(0);
  mWidth->lineEdit()->setValidator( new QRegularExpressionValidator( QRegularExpression("[0-9]{1,3}((\\.|\\,)[0-9]{0,3})?")) );
  mWidth->setMinimumWidth(80);

  //on begin editing
  connect( but, &QToolButton::clicked, [=]() {
    mWidth->lineEdit()->setFocus();
    mWidth->lineEdit()->selectAll();
    });
  //on complete editing
  connect( mWidth->lineEdit(), &QLineEdit::editingFinished, [=]() {
    prevWidth.reorderComboBoxDoubleString( mWidth );
    emit propChanged();
    });
  //on select other width
  connect( mWidth, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int) {
    prevWidth.reorderComboBoxDoubleString( mWidth );
    emit propChanged();
    });

  addWidget( mWidth );

  addSeparator();

  addWidget( but = new QToolButton() );
  but->setText( tr("Wire name:") );
  but->setToolTip( tr("Activate wire name editor") );

  mWireName = new QComboBox();
  mWireName->setEditable(true);
  mWireName->setToolTip( tr("Wire name editor") );
  //Fill name list with previous wire names
  if( prevWires.count() == 0 )
    prevWires.addString( QStringLiteral("----") );
  for( const QString &v : prevWires )
    mWireName->addItem( v );
  //Select first item
  mWireName->setCurrentIndex(0);
  mWireName->setMinimumWidth(120);

  //on begin editing
  connect( but, &QToolButton::clicked, [=]() {
    mWireName->lineEdit()->setFocus();
    mWireName->lineEdit()->selectAll();
    });
  //on complete editing
  connect( mWireName->lineEdit(), &QLineEdit::editingFinished, [=]() {
    prevWires.reorderComboBoxString( mWireName );
    emit propChanged();
    });
  //on select other net name
  connect( mWireName, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int) {
    prevWires.reorderComboBoxString( mWireName );
    emit propChanged();
    });

  addWidget( mWireName );

  addSeparator();

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


  //Line type
  mLineSolid = addAction( QIcon(QString(":/pic/dltSolid.png")), tr("lines connects at any degree") );
  mLineSolid->setCheckable(true);
  connect( mLineSolid, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setLineType( dltSolid );
    emit propChanged();
    });

  mLineDotted = addAction( QIcon(QString(":/pic/dltDotted.png")), tr("lines connects at any degree") );
  mLineDotted->setCheckable(true);
  connect( mLineDotted, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setLineType( dltDotted );
    emit propChanged();
    });

  mLineDashed = addAction( QIcon(QString(":/pic/dltDashed.png")), tr("lines connects at any degree") );
  mLineDashed->setCheckable(true);
  connect( mLineDashed, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setLineType( dltDashed );
    emit propChanged();
    });

  }




void SdPropBarWire::setPropWire(SdPropLine *propLine, double ppm, int enterType, const QString wireName)
  {
  if( propLine ) {
    //Set current layer
    updateEditObjectProp( dctSheet, stmThrough, propLine->mLayer.layer(false) );

    //Set current width
    mPPM = ppm;
    if( propLine->mWidth.isValid() ) {
      mWidth->setCurrentText( propLine->mWidth.log2Phis(mPPM) );
      prevWidth.reorderComboBoxDoubleString( mWidth );
      }
    else
      mWidth->setCurrentText( QString()  );

    //line enter type
    setVertexType( enterType );

    //line type
    setLineType( propLine->mType.getValue() );

    //Current wire name
    mWireName->setCurrentText( wireName );
    mWireName->lineEdit()->selectAll();
    prevWires.reorderComboBoxString( mWireName );
    }
  }




void SdPropBarWire::getPropWire(SdPropLine *propLine, int *enterType, QString *wireName)
  {
  if( propLine ) {
    SdLayer *layer = getSelectedLayer();
    if( layer )
      propLine->mLayer = layer;

    if( !mWidth->currentText().isEmpty() )
      propLine->mWidth.setFromPhis( mWidth->currentText(), mPPM );

    if( !mWireName->currentText().isEmpty() )
      *wireName = mWireName->currentText();

    if( mLineSolid->isChecked() ) propLine->mType = dltSolid;
    else if( mLineDotted->isChecked() ) propLine->mType = dltDotted;
    else if( mLineDashed->isChecked() ) propLine->mType = dltDashed;
    }
  if( enterType ) {
    if( mEnterOrtho->isChecked() ) *enterType = dleOrtho;
    else if( mEnter45degree->isChecked() ) *enterType = dle45degree;
    else *enterType = dleAnyDegree;
    }
  }








//Set line vertex type when enter line - angle between two connected segments
void SdPropBarWire::setVertexType(int type)
  {
  mEnterOrtho->setChecked( type == dleOrtho );
  mEnter45degree->setChecked( type == dle45degree );
  mEnterAnyDegree->setChecked( type == dleAnyDegree );
  }




//Set line type: solid, dotted or dashed
void SdPropBarWire::setLineType(int type)
  {
  mLineSolid->setChecked( type == dltSolid );
  mLineDotted->setChecked( type == dltDotted );
  mLineDashed->setChecked( type == dltDashed );
  }
