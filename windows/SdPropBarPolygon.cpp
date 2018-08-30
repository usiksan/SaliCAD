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
#include "objects/SdUtil.h"
#include <QLineEdit>

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
  mGap->lineEdit()->setValidator( new QRegExpValidator( QRegExp("[0-9]{1,3}((\\.|\\,)[0-9]{0,3})?")) );
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


  mWireName = new QLineEdit();
  mWireName->setReadOnly(true);
  mWireName->setMinimumWidth(80);
  mWireName->setMaximumWidth(120);

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





void SdPropBarPolygon::setPropPolygon(SdPropPolygon *propPolygon, double ppm, int enterType)
  {
  if( propPolygon ) {
    //Set current stratum
    setSelectedStratum( propPolygon->mStratum );

    //Set current width
    mPPM = ppm;
    if( propPolygon->mGap.isValid() ) {
      mGap->setCurrentText( propPolygon->mGap.log2Phis(mPPM)  );
      prevGap.reorderComboBoxDoubleString( mGap );
      }
    else
      mGap->setCurrentText( QString()  );

    //line enter type
    setVertexType( enterType );

    //Current road name name
    mWireName->setText( propPolygon->mNetName.str() );
    }
  }




void SdPropBarPolygon::getPropPolygon(SdPropPolygon *propPolygon, int *enterType)
  {
  if( propPolygon ) {
    int stratum = getSelectedStratum();
    if( stratum )
      propPolygon->mStratum = stratum;

    if( !mGap->currentText().isEmpty() )
      propPolygon->mGap.setFromPhis( mGap->currentText(), mPPM );

    //Net name not used
    }
  if( enterType ) {
    if( mEnterOrtho->isChecked() ) *enterType = dleOrtho;
    else if( mEnter45degree->isChecked() ) *enterType = dle45degree;
    else *enterType = dleAnyDegree;
    }
  }

