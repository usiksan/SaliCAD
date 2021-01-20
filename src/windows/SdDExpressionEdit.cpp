/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for edit and test sheet expressions
*/
#include "SdDExpressionEdit.h"
#include "ui_SdDExpressionEdit.h"
#include "SdExpression.h"

#include <QSplitter>

SdDExpressionEdit::SdDExpressionEdit(QStringList list, QWidget *parent) :
  QDialog(parent),
  mExpression(),
  mParseActive(false),
  ui(new Ui::SdDExpressionEdit)
  {
  ui->setupUi(this);
  QSplitter *splitter = new QSplitter( Qt::Horizontal, this );
  mText = new QTextEdit(this);
  mTable = new QTableWidget(this);
  splitter->addWidget( mText );
  splitter->addWidget( mTable );
  ui->verticalLayout->insertWidget( 0, splitter );

  mText->setPlainText( list.join(QChar('\n')) );

  connect( mText, &QTextEdit::textChanged, this, &SdDExpressionEdit::parse );
  connect( mText, &QTextEdit::cursorPositionChanged, this, &SdDExpressionEdit::parse );
  connect( mTable, &QTableWidget::cellChanged, this, [this] ( int row, int column ) {
    Q_UNUSED(column)
    if( !mParseActive ) {
      QString name = mTable->item( row, 0 )->text();
      QString value = mTable->item( row, 1 )->text();
      mExpression.valueSet( name, value.toDouble() );
      mExpression.calculate();
      updateResult();
      }
    });
  parse();
  }



SdDExpressionEdit::~SdDExpressionEdit()
  {
  delete ui;
  }




void SdDExpressionEdit::parse()
  {
  mParseActive = true;
  QTextCursor cursor = mText->textCursor();
  cursor.select( QTextCursor::BlockUnderCursor );

  QString err = mExpression.parse( cursor.selectedText() );
  ui->mError->setText( err );

  QStringList params = mExpression.paramList();
  mTable->clear();
  mTable->setColumnCount(2);
  mTable->setRowCount( params.count() + 1 );
  //Fill table
  int row = 0;
  QTableWidgetItem *item;
  for( auto const &str : params ) {
    mTable->setItem( row, 0, item = new QTableWidgetItem(str) );
    item->setFlags( Qt::NoItemFlags );
    mTable->setItem( row, 1, item = new QTableWidgetItem( QString::number(mExpression.value(str)) )  );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEditable );
    row++;
    }
  //Result
  mTable->setItem( row, 0, item = new QTableWidgetItem(mExpression.result()) );
  item->setFlags( Qt::NoItemFlags );
  mTable->setItem( row, 1, item = new QTableWidgetItem( QString::number(mExpression.value(mExpression.result())) )  );
  item->setFlags( Qt::NoItemFlags );

  mExpression.calculate();
  updateResult();
  mParseActive = false;
  }




void SdDExpressionEdit::updateResult()
  {
  mParseActive = true;
  int result = mTable->rowCount() - 1;
  if( result >= 0 )
    mTable->item( result, 1 )->setText( QString::number(mExpression.value(mExpression.result())) );
  mParseActive = false;
  }
