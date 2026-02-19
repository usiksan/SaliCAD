/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for lines and other linear objects
*/
#include "SdPropBarLinear.h"
#include "objects/SdProp.h"
#include "SdStringHistory.h"
#include "SdWCommand.h"
#include <QList>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QIcon>
#include <QDebug>


static SdStringHistory prevWidth;
//static QList<double> prevWidth;

SdPropBarLinear::SdPropBarLinear(const QString title) :
  SdPropBar( title )
  {
  mWidth = new QComboBox();
  mWidth->setEditable(true);
  //Fill width list with previous values
  if( prevWidth.count() == 0 )
    prevWidth.addDouble( 0.0 );
  for( const QString &v : std::as_const(prevWidth) )
    mWidth->addItem( v );
  //Select first item
  mWidth->setCurrentIndex(0);
  mWidth->lineEdit()->setValidator( new QRegularExpressionValidator( QRegularExpression("[0-9]{1,3}((\\.|\\,)[0-9]{0,3})?")) );
//  mWidth->lineEdit()->setValidator( new QDoubleValidator() );
  mWidth->setMinimumWidth(80);

  //on complete editing
  connect( mWidth->lineEdit(), &QLineEdit::editingFinished, [=](){
    prevWidth.reorderComboBoxDoubleString( mWidth );
    emit propChanged();
    });
  //on select other width
  connect( mWidth, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index){
    Q_UNUSED(index)
    prevWidth.reorderComboBoxDoubleString( mWidth );
    emit propChanged();
    });

  addWidget( mWidth );

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



//Set all visual line properties from SdPropLine structure
void SdPropBarLinear::setPropLine(SdProjectItem *pitem, SdPropLine *propLine, double ppm, int enterType)
  {
  SdPropComposerLine composerLine;
  if( propLine ) composerLine.reset( *propLine );
  setPropLine( pitem, composerLine, ppm, enterType );
  }



//Get all line properties from visual elements and fill SdPropLine
void SdPropBarLinear::getPropLine(SdPropLine *propLine, int *enterType )
  {
  SdPropComposerLine composerLine;
  getPropLine( composerLine, enterType );
  if( propLine != nullptr )
    composerLine.store( *propLine );
  }



void SdPropBarLinear::setPropLine(SdProjectItem *pitem, const SdPropComposerLine &propLine, double ppm, int enterType)
  {
  //Set current layer
  auto &propLayer = propLine.get<&SdPropLine::mLayer>();
  if( propLayer.isSingle() )
    updateEditObjectProp( pitem, propLayer.value().layer(false) );
  else
    updateEditObjectProp( pitem, nullptr );

  //Set current width
  mPPM = ppm;
  if( propLine.get<&SdPropLine::mWidth>().isSingle() ) {
    mWidth->setCurrentText( propLine.get<&SdPropLine::mWidth>().value().log2Phis(mPPM) );
    prevWidth.reorderComboBoxDoubleString( mWidth );
    }
  else
    mWidth->setCurrentText( QString()  );

  //line enter type
  setVertexType( enterType );

  //line type
  if( propLine.get<&SdPropLine::mType>().isSingle() )
    setLineType( propLine.get<&SdPropLine::mType>().value().value() );
  else
    setLineType( -1 );
  }




void SdPropBarLinear::getPropLine(SdPropComposerLine &propLine, int *enterType)
  {
  propLine.clear();
  //Store layer if setted
  SdLayer *layer = getSelectedLayer();
  if( layer )
    propLine.get<&SdPropLine::mLayer>().reset( SdPvLayer(layer) );

  //Store width if setted
  if( !mWidth->currentText().isEmpty() )
    propLine.get<&SdPropLine::mWidth>().reset( SdPvInt( mWidth->currentText(), mPPM ) );

  //Store type if setted
  auto &propType = propLine.get<&SdPropLine::mType>();
  if( mLineSolid->isChecked() ) propType.reset( SdPvInt(dltSolid) );
  else if( mLineDotted->isChecked() ) propType.reset( SdPvInt(dltDotted) );
  else if( mLineDashed->isChecked() ) propType.reset( SdPvInt(dltDashed) );

  if( enterType ) {
    if( mEnterOrtho->isChecked() ) *enterType = dleOrtho;
    else if( mEnter45degree->isChecked() ) *enterType = dle45degree;
    else *enterType = dleAnyDegree;
    }
  }



//Set line vertex type when enter line - angle between two connected segments
void SdPropBarLinear::setVertexType(int type)
  {
  mEnterOrtho->setChecked( type == dleOrtho );
  mEnter45degree->setChecked( type == dle45degree );
  mEnterAnyDegree->setChecked( type == dleAnyDegree );
  }



//Set line type: solid, dotted or dashed
void SdPropBarLinear::setLineType(int type)
  {
  mLineSolid->setChecked( type == dltSolid );
  mLineDotted->setChecked( type == dltDotted );
  mLineDashed->setChecked( type == dltDashed );
  }
