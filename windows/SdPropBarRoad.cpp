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
#include "objects/SdUtil.h"
#include <QLineEdit>

static QList<double> prevWidth;


SdPropBarRoad::SdPropBarRoad(const QString title) :
  SdPropBarStratum( title )
  {
  mWidth = new QComboBox();
  mWidth->setEditable(true);
  //Fill width list with previous values
  if( prevWidth.count() == 0 )
    prevWidth.append( 0.0 );
  for( double v : prevWidth )
    mWidth->addItem( QString::number( v, 'f', 3) );
  //Select first item
  mWidth->setCurrentIndex(0);
  mWidth->lineEdit()->setValidator( new QRegExpValidator( QRegExp("[0-9]{1,3}((\\.|\\,)[0-9]{0,3})?")) );
  mWidth->setMinimumWidth(80);

  //on complete editing
  connect( mWidth->lineEdit(), &QLineEdit::editingFinished, [=](){
    setWidth( SdUtil::str2phys( mWidth->currentText() ) );
    emit propChanged();
    });
  //on select other width
  connect( mWidth, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index){
    Q_UNUSED(index)
    setWidth( SdUtil::str2phys( mWidth->currentText() ) );
    emit propChanged();
    });

  addWidget( mWidth );

  addSeparator();


  mWireName = new QLineEdit();
  mWireName->setReadOnly(true);
  mWireName->setMinimumWidth(80);

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




void SdPropBarRoad::setPropRoad(SdPropRoad *propRoad, double ppm, int enterType)
  {
  if( propRoad ) {
    //Set current stratum
    setSelectedStratum( propRoad->mStratum );

    //Set current width
    mPPM = ppm;
    if( propRoad->mWidth.isValid() ) {
      mWidth->setCurrentText( propRoad->mWidth.log2Phis(mPPM)  );
      setWidth( propRoad->mWidth.getDouble() * mPPM );
      }
    else
      mWidth->setCurrentText( QString()  );

    //line enter type
    setVertexType( enterType );

    //Current road name name
    mWireName->setText( propRoad->mNetName.str() );
    }
  }





void SdPropBarRoad::getPropRoad(SdPropRoad *propRoad, int *enterType)
  {
  if( propRoad ) {
    int stratum = getSelectedStratum();
    if( stratum )
      propRoad->mStratum = stratum;

    if( !mWidth->currentText().isEmpty() )
      propRoad->mWidth.setFromPhis( mWidth->currentText(), mPPM );

    //Net name not used
    }
  if( enterType ) {
    if( mEnterOrtho->isChecked() ) *enterType = dleOrtho;
    else if( mEnter45degree->isChecked() ) *enterType = dle45degree;
    else *enterType = dleAnyDegree;
    }
  }




void SdPropBarRoad::setWidth(double width)
  {
  int index = prevWidth.indexOf( width );
  if( index < 0 ) {
    //new value, insert
    if( prevWidth.count() >= ROAD_WIDTH_PREV_COUNT )
      //List of previous width is full. Remove last value.
      prevWidth.removeLast();
    prevWidth.insert( 0, width );
    }
  else {
    //Move used value to top
    prevWidth.removeAt( index );
    prevWidth.insert( 0, width );
    }
  }




void SdPropBarRoad::setVertexType(int type)
  {
  mEnterOrtho->setChecked( type == dleOrtho );
  mEnter45degree->setChecked( type == dle45degree );
  mEnterAnyDegree->setChecked( type == dleAnyDegree );
  }





