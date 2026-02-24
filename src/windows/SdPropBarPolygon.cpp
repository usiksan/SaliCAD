/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for tracing polygon
*/
#include "SdPropBarPolygon.h"
#include "SdStringHistory.h"
#include "objects/SdProp.h"
#include <QLineEdit>
#include <QDebug>

static SdStringHistory prevGap;

SdPropBarPolygon::SdPropBarPolygon(const QString title) :
  SdPropBarStratum( title )
  {
  mGap = new QComboBox();
  mGap->setEditable(true);
  //Fill gap list with previous values
  if( prevGap.count() == 0 )
    prevGap.addDouble( 0.1 );
  for( const QString &str : prevGap )
    mGap->addItem( str );
  //Select first item
  mGap->setCurrentIndex(0);
  mGap->lineEdit()->setValidator( new QRegularExpressionValidator( QRegularExpression("[0-9]{1,3}((\\.|\\,)[0-9]{0,3})?")) );
  mGap->setMinimumWidth(80);

  //on complete editing
  connect( mGap->lineEdit(), &QLineEdit::editingFinished, [=]() {
    prevGap.reorderComboBoxDoubleString( mGap );
    emit propChanged();
    });
  //on select other gap
  connect( mGap, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int){
    prevGap.reorderComboBoxDoubleString( mGap );
    emit propChanged();
    });

  addWidget( mGap );

  addSeparator();


  mWireName = new QComboBox();
  mWireName->setEditable(true);
  mWireName->setMinimumWidth(120);
  mWireName->setMaximumWidth(160);

  //on select other gap
  connect( mWireName, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int){
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
  }





void SdPropBarPolygon::setPropPolygon(const SdPropPolygon &propPolygon, double ppm, int enterType, const QStringList list )
  {
  SdPropComposerPolygon propComposerPolygon;
  propComposerPolygon.reset( propPolygon );
  setPropPolygon( propComposerPolygon, ppm, enterType, list );
  }




void SdPropBarPolygon::getPropPolygon(SdPropPolygon &propPolygon, int *enterType)
  {
  SdPropComposerPolygon propComposerPolygon;
  getPropPolygon( propComposerPolygon, enterType );
  propComposerPolygon.store( propPolygon );
  }



void SdPropBarPolygon::setPropPolygon(const SdPropComposerPolygon &propPolygon, double ppm, int enterType, const QStringList list)
  {
  //Set current stratum
  auto &propStratum = propPolygon.get<&SdPropPolygon::mStratum>();
  setSelectedStratum( propStratum.isSingle() ? propStratum.value() : SdPvStratum(0) );

  //Set current gap
  mPPM = ppm;
  auto &propGap = propPolygon.get<&SdPropPolygon::mGap>();
  if( propGap.isSingle() ) {
    mGap->setCurrentText( propGap.value().log2Phis(mPPM)  );
    prevGap.reorderComboBoxDoubleString( mGap );
    }
  else
    mGap->setCurrentText( QString()  );

  //line enter type
  setVertexType( enterType );

  //Fill net list
  mWireName->addItems( list );
  //Current road name name
  auto &propNetName = propPolygon.get<&SdPropPolygon::mNetName>();
  mWireName->setCurrentText( propNetName.isSingle() ? propNetName.value().string() : QString{} );
  }




void SdPropBarPolygon::getPropPolygon(SdPropComposerPolygon &propPolygon, int *enterType)
  {
  propPolygon.clear();
  SdPvStratum stratum = getSelectedStratum();
  if( !stratum.isEmpty() )
    propPolygon.get<&SdPropPolygon::mStratum>().reset(stratum);

  //Store gap if setted
  if( !mGap->currentText().isEmpty() )
    propPolygon.get<&SdPropPolygon::mGap>().reset( SdPvInt( mGap->currentText(), mPPM ) );

  //Net name
  QString netName = mWireName->currentText();
  if( !netName.isEmpty() )
    propPolygon.get<&SdPropPolygon::mNetName>().reset( netName );

  if( enterType ) {
    if( mEnterOrtho->isChecked() ) *enterType = dleOrtho;
    else if( mEnter45degree->isChecked() ) *enterType = dle45degree;
    else *enterType = dleAnyDegree;
    }
  }




void SdPropBarPolygon::setVertexType(int type)
  {
  mEnterOrtho->setChecked( type == dleOrtho );
  mEnter45degree->setChecked( type == dle45degree );
  mEnterAnyDegree->setChecked( type == dleAnyDegree );
  }

