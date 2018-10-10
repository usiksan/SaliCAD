/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol creation master for integrated circuits
*/
#include "SdDMasterSymbolIc.h"
#include "ui_SdDMasterSymbolIc.h"
#include "objects/SdUtil.h"
#include "objects/SdPropText.h"


static QString strDelim("---");
static QString strGap;

SdDMasterSymbolIc::SdDMasterSymbolIc( SdProjectItem *item, QWidget *parent ) :
  SdDMasterSymbol( item, parent ),
  ui(new Ui::SdDMasterSymbolIc)
  {
  ui->setupUi(this);

  //Setup pin tables
  ui->mLeftTable->setColumnCount(2);
  ui->mLeftTable->setHorizontalHeaderLabels( {tr("Pin name"), tr("Pin description")} );
  ui->mLeftTable->setColumnWidth( 0, 100 );
  ui->mLeftTable->setColumnWidth( 1, 150 );

  ui->mRightTable->setColumnCount(2);
  ui->mRightTable->setHorizontalHeaderLabels( {tr("Pin name"), tr("Pin description")} );
  ui->mRightTable->setColumnWidth( 0, 100 );
  ui->mRightTable->setColumnWidth( 1, 150 );

  //Connect button signals
  connect( ui->mLeftInsPin,   &QPushButton::clicked, this, &SdDMasterSymbolIc::leftInsertPin );
  connect( ui->mLeftInsGap,   &QPushButton::clicked, this, &SdDMasterSymbolIc::leftInsertGap );
  connect( ui->mLeftInsDelim, &QPushButton::clicked, this, &SdDMasterSymbolIc::leftInsertDelimiter );
  connect( ui->mLeftDelete,   &QPushButton::clicked, this, &SdDMasterSymbolIc::leftDelete );

  connect( ui->mRightInsPin,   &QPushButton::clicked, this, &SdDMasterSymbolIc::rightInsertPin );
  connect( ui->mRightInsGap,   &QPushButton::clicked, this, &SdDMasterSymbolIc::rightInsertGap );
  connect( ui->mRightInsDelim, &QPushButton::clicked, this, &SdDMasterSymbolIc::rightInsertDelimiter );
  connect( ui->mRightDelete,   &QPushButton::clicked, this, &SdDMasterSymbolIc::rightDelete );

  connect( ui->mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  connect( ui->mRightTable, &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );

  onEditChanged(QString());
  }

SdDMasterSymbolIc::~SdDMasterSymbolIc()
  {
  delete ui;
  }




//Update preview on any params changed
void SdDMasterSymbolIc::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawSymbol( il );
  il.setPixmap( 400, 600, Qt::white );
  drawSymbol( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }




//Update preview on any pin changes
void SdDMasterSymbolIc::onPinChanged(int row, int column)
  {
  Q_UNUSED(row)
  Q_UNUSED(column)
  onEditChanged( QString() );
  }




//Insert new pin to left side
void SdDMasterSymbolIc::leftInsertPin()
  {
  int cr = ui->mLeftTable->currentRow();
  QString pinName;
  if( cr >= 0 ) {
    pinName = nextText( ui->mLeftTable->item(cr,0)->text() );
    cr++;
    }
  else
    cr = ui->mLeftTable->rowCount();
  disconnect( ui->mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  ui->mLeftTable->insertRow( cr );
  ui->mLeftTable->setRowHeight( cr, 20 );
  ui->mLeftTable->setItem( cr, 0, new QTableWidgetItem(pinName) );
  ui->mLeftTable->setItem( cr, 1, new QTableWidgetItem() );
  ui->mLeftTable->setCurrentCell( cr, 0 );
  connect( ui->mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  onEditChanged( QString() );
  }




//Insert new gap to left side
void SdDMasterSymbolIc::leftInsertGap()
  {
  int cr = ui->mLeftTable->currentRow();
  if( cr >= 0 ) cr++;
  else          cr = ui->mLeftTable->rowCount();
  disconnect( ui->mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  ui->mLeftTable->insertRow( cr );
  ui->mLeftTable->setRowHeight( cr, 20 );
  ui->mLeftTable->setItem( cr, 0, new QTableWidgetItem(strGap) );
  ui->mLeftTable->setItem( cr, 1, new QTableWidgetItem() );
  ui->mLeftTable->setCurrentCell( cr, 0 );
  connect( ui->mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  onEditChanged( QString() );
  }




//Insert new delimiter to left side
void SdDMasterSymbolIc::leftInsertDelimiter()
  {
  int cr = ui->mLeftTable->currentRow();
  if( cr >= 0 ) cr++;
  else          cr = ui->mLeftTable->rowCount();
  disconnect( ui->mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  ui->mLeftTable->insertRow( cr );
  ui->mLeftTable->setRowHeight( cr, 20 );
  ui->mLeftTable->setItem( cr, 0, new QTableWidgetItem(strDelim) );
  ui->mLeftTable->setItem( cr, 1, new QTableWidgetItem() );
  ui->mLeftTable->setCurrentCell( cr, 0 );
  connect( ui->mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  onEditChanged( QString() );
  }




//Delete pin, gap or delimiter from left side
void SdDMasterSymbolIc::leftDelete()
  {
  int cr = ui->mLeftTable->currentRow();
  if( cr >= 0 ) ui->mLeftTable->removeRow(cr);
  onEditChanged( QString() );
  }




//Insert new pin to right side
void SdDMasterSymbolIc::rightInsertPin()
  {
  int cr = ui->mRightTable->currentRow();
  QString pinName;
  if( cr >= 0 ) {
    pinName = nextText( ui->mRightTable->item(cr,0)->text() );
    cr++;
    }
  else
    cr = ui->mRightTable->rowCount();
  disconnect( ui->mRightTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  ui->mRightTable->insertRow( cr );
  ui->mRightTable->setRowHeight( cr, 20 );
  ui->mRightTable->setItem( cr, 0, new QTableWidgetItem(pinName) );
  ui->mRightTable->setItem( cr, 1, new QTableWidgetItem() );
  ui->mRightTable->setCurrentCell( cr, 0 );
  connect( ui->mRightTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  onEditChanged( QString() );
  }




//Insert new gap to right side
void SdDMasterSymbolIc::rightInsertGap()
  {
  int cr = ui->mRightTable->currentRow();
  if( cr >= 0 ) cr++;
  else          cr = ui->mRightTable->rowCount();
  disconnect( ui->mRightTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  ui->mRightTable->insertRow( cr );
  ui->mRightTable->setRowHeight( cr, 20 );
  ui->mRightTable->setItem( cr, 0, new QTableWidgetItem(strGap) );
  ui->mRightTable->setItem( cr, 1, new QTableWidgetItem() );
  ui->mRightTable->setCurrentCell( cr, 0 );
  connect( ui->mRightTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  onEditChanged( QString() );
  }




//Insert new delimiter to right side
void SdDMasterSymbolIc::rightInsertDelimiter()
  {
  int cr = ui->mRightTable->currentRow();
  if( cr >= 0 ) cr++;
  else          cr = ui->mRightTable->rowCount();
  disconnect( ui->mRightTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  ui->mRightTable->insertRow( cr );
  ui->mRightTable->setRowHeight( cr, 20 );
  ui->mRightTable->setItem( cr, 0, new QTableWidgetItem(strDelim) );
  ui->mRightTable->setItem( cr, 1, new QTableWidgetItem() );
  ui->mRightTable->setCurrentCell( cr, 0 );
  connect( ui->mRightTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIc::onPinChanged );
  onEditChanged( QString() );
  }




//Delete pin, gap or delimiter from right side
void SdDMasterSymbolIc::rightDelete()
  {
  int cr = ui->mRightTable->currentRow();
  if( cr >= 0 ) ui->mRightTable->removeRow(cr);
  onEditChanged( QString() );
  }





//Draw symbol preview
void SdDMasterSymbolIc::drawSymbol(SdIllustrator &il)
  {
  //Cell count on left and right sides
  int leftCellCount = ui->mLeftTable->rowCount();
  int rightCellCount = ui->mRightTable->rowCount();
  //Calc extending pin name presentation
  bool extended = false;
  for( int i = 0; i < leftCellCount && !extended; i++ )
    extended = !ui->mLeftTable->item( i, 1 )->text().isEmpty();
  for( int i = 0; i < rightCellCount && !extended; i++ )
    extended = !ui->mRightTable->item( i, 1 )->text().isEmpty();

  int maxPins     = qMax(leftCellCount,rightCellCount);
  if( maxPins < 1 ) maxPins = 1;
  int cellHeight  = 500; //sdEnvir->fromPhisSchematic( ui->mCellHeight->text() );
  int pinSizeX    = 750; //sdEnvir->fromPhisSchematic( ui->mPinSizeX->text() );
  int cellSizeX   = extended ? 5000 : 1250; //sdEnvir->fromPhisSchematic( ui->mCellSizeX->text() );
  int leftPin     = 0;
  int leftBody    = pinSizeX;
  int rightBody   = leftBody + 2 * cellSizeX + 1500;
  int rightPin    = rightBody + pinSizeX;
  int topBody     = 250;
  int bottomBody  = topBody - maxPins * cellHeight;


  QColor red("red");
  QColor green("green");

  //Body
  il.drawRect( leftBody, topBody,  rightBody, bottomBody, red );
  //Vertical delimiter
  int leftDelimiter = leftBody + cellSizeX;
  il.drawLine( leftDelimiter, topBody,  leftDelimiter, bottomBody, red );
  int rightDelimiter = leftDelimiter + 1500;
  il.drawLine( rightDelimiter, topBody,  rightDelimiter, bottomBody, red );

  //Pins
  int nameSize = 300;
  cellHeight = -cellHeight;
  for( int i = 0; i < leftCellCount; i++ ) {
    QString pinName = ui->mLeftTable->item( i, 0 )->text();
    if( pinName.isEmpty() ) {
      //This is gap. Nothing done
      }
    else if( pinName == strDelim ) {
      //Delimiter
      il.drawLine( pinSizeX, i * cellHeight, leftDelimiter, i * cellHeight, red );
      }
    else {
      //Pin line
      il.drawLine( leftPin, i * cellHeight, pinSizeX, i * cellHeight, red );
      //Pin crosshair
      il.drawCross( leftPin, i * cellHeight, 50, green );
      //Pin name
      il.drawTextMapped( pinSizeX + 10, i * cellHeight - 150, pinName, red, nameSize );
      }
    }

  for( int i = 0; i < rightCellCount; i++ ) {
    QString pinName = ui->mRightTable->item( i, 0 )->text();
    if( pinName.isEmpty() ) {
      //This is gap. Nothing done
      }
    else if( pinName == strDelim ) {
      //Delimiter
      il.drawLine( rightBody, i * cellHeight, rightDelimiter, i * cellHeight, red );
      }
    else {
      //Pin line
      il.drawLine( rightBody, i * cellHeight, rightPin, i * cellHeight, red );
      //Pin crosshair
      il.drawCross( rightPin, i * cellHeight, 50, green );
      //Pin name
      il.drawTextMapped( rightBody - 1240, i * cellHeight - 150, pinName, red, nameSize );
      }
    }
  }




//Build symbol
void SdDMasterSymbolIc::accept()
  {
  //Cell count on left and right sides
  int leftCellCount = ui->mLeftTable->rowCount();
  int rightCellCount = ui->mRightTable->rowCount();
  //Calc extending pin name presentation
  bool extended = false;
  for( int i = 0; i < leftCellCount && !extended; i++ )
    extended = !ui->mLeftTable->item( i, 1 )->text().isEmpty();
  for( int i = 0; i < rightCellCount && !extended; i++ )
    extended = !ui->mRightTable->item( i, 1 )->text().isEmpty();

  int maxPins     = qMax(leftCellCount,rightCellCount);
  if( maxPins < 1 ) maxPins = 1;
  int cellHeight  = 500; //sdEnvir->fromPhisSchematic( ui->mCellHeight->text() );
  int pinSizeX    = 750; //sdEnvir->fromPhisSchematic( ui->mPinSizeX->text() );
  int cellSizeX   = extended ? 5000 : 1250; //sdEnvir->fromPhisSchematic( ui->mCellSizeX->text() );
  int leftPin     = 0;
  int leftBody    = pinSizeX;
  int rightBody   = leftBody + 2 * cellSizeX + 1500;
  int rightPin    = rightBody + pinSizeX;
  int topBody     = 250;
  int bottomBody  = topBody - maxPins * cellHeight;
  int leftPinName = leftBody + 625;
  int rightPinName = rightBody - 625;


  //Body
  addRect( leftBody, topBody,  rightBody, bottomBody );
  //Vertical delimiter
  int leftDelimiter = leftBody + cellSizeX;
  addLine( leftDelimiter, topBody,  leftDelimiter, bottomBody );
  int rightDelimiter = leftDelimiter + 1500;
  addLine( rightDelimiter, topBody,  rightDelimiter, bottomBody );

  //Pins
  cellHeight = -cellHeight;

  mPinNumberProp.mHorz = dhjLeft;
  mPinNameProp.mHorz = dhjCenter;
  for( int i = 0; i < leftCellCount; i++ ) {
    QString pinName = ui->mLeftTable->item( i, 0 )->text();
    if( pinName.isEmpty() ) {
      //This is gap. Nothing done
      }
    else if( pinName == strDelim ) {
      //Delimiter
      addLine( pinSizeX, i * cellHeight, leftDelimiter, i * cellHeight );
      }
    else {
      //Pin line
      addLine( leftPin, i * cellHeight, pinSizeX, i * cellHeight );
      //Pin
      SdPoint pinOrg(leftPin, i * cellHeight);
      addPin( pinOrg, 0, SdPoint(leftPinName,pinOrg.y()), pinName, SdPoint(leftPin,pinOrg.y()+250) );
      //Pin description
      }
    }

  mPinNumberProp.mHorz = dhjRight;
  for( int i = 0; i < rightCellCount; i++ ) {
    QString pinName = ui->mRightTable->item( i, 0 )->text();
    if( pinName.isEmpty() ) {
      //This is gap. Nothing done
      }
    else if( pinName == strDelim ) {
      //Delimiter
      addLine( rightBody, i * cellHeight, rightDelimiter, i * cellHeight );
      }
    else {
      //Pin line
      addLine( rightBody, i * cellHeight, rightPin, i * cellHeight );
      //Pin
      SdPoint pinOrg(rightPin, i * cellHeight);
      addPin( pinOrg, 0, SdPoint(rightPinName,pinOrg.y()), pinName, SdPoint(rightPin,pinOrg.y()+250) );
      }
    }

  //id
  int hcenter = (leftDelimiter + rightDelimiter) / 2;
  setId( SdPoint( hcenter, topBody + 250 ) );

  //value
  setValue( SdPoint( hcenter, bottomBody - 250 ) );


  SdDMasterSymbol::accept();
  }





