/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdDGetObject.h"
#include "ui_SdDGetObject.h"
#include "SdWEditorGraphSymbol.h"
#include "SdWEditorGraphPart.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdProjectItem.h"
#include "objects/SdPItemComponent.h"

#include <QSqlRecord>
#include <QSqlField>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

QSqlQueryModel *SdDGetObject::mModel;

SdDGetObject::SdDGetObject(quint64 sort, const QString title, QWidget *parent) :
  QDialog(parent),
  mSort(sort),
  ui(new Ui::SdDGetObject)
  {
  ui->setupUi(this);
  setWindowTitle( title );

  //Enable edit mode
  ui->mFind->setEditable(true);

  //Create symbol and part view
  mSymbolView = new SdWEditorGraphSymbol( nullptr, ui->mSymbolBox );
  QHBoxLayout *lay = new QHBoxLayout();
  ui->mSymbolBox->setLayout( lay );
  lay->addWidget( mSymbolView );

  mPartView = new SdWEditorGraphPart( nullptr, ui->mPartBox );
  lay = new QHBoxLayout();
  ui->mPartBox->setLayout( lay );
  lay->addWidget( mPartView );

  connect( ui->mFindButton, &QPushButton::clicked, this, &SdDGetObject::find );
  connect( ui->mFindTable, &QTableView::clicked, this, &SdDGetObject::onSelectItem );

  if( mModel == nullptr ) {
    mModel = new QSqlQueryModel();
    //mModel->setQuery( QString("SELECT name, author, rank FROM objects WHERE lower(name) LIKE '%%%1%%' AND author='%2'"));
    }
  ui->mFindTable->setModel( mModel );
  ui->mFindTable->hideColumn(3);
  }



SdDGetObject::~SdDGetObject()
  {
  delete ui;
  }




void SdDGetObject::find()
  {
  QString name = ui->mFind->currentText();
  if( name.isEmpty() )
    mModel->setQuery( QString("SELECT name, author, rank, hash FROM objects WHERE (class & %1)<>0").arg(mSort) );
  else
    mModel->setQuery( QString("SELECT name, author, rank, hash FROM objects WHERE lower(name) LIKE '%%%1%%'").arg(name.toLower()) );

  mModel->setHeaderData( 0, Qt::Horizontal, tr("Name") );
  mModel->setHeaderData( 1, Qt::Horizontal, tr("Author") );
  mModel->setHeaderData( 2, Qt::Horizontal, tr("Rank") );
  ui->mFindTable->hideColumn(3);
  }




void SdDGetObject::onSelectItem(QModelIndex index)
  {
  QSqlRecord rec = mModel->record( index.row() );
  qDebug() << "select" << rec.value( QStringLiteral("name") ).toString() << rec.value( QStringLiteral("hash") ).toString();
  SdObject *obj = SdObjectFactory::extractObject( rec.value( QStringLiteral("hash") ).toString(), true, this );

  SdPItemComponent *comp = dynamic_cast<SdPItemComponent*>( obj );
  if( comp ) {
    mSymbolView->setSymbol( dynamic_cast<SdPItemSymbol*>(comp->extractSymbolFromFactory(0,true,this)) );
    mPartView->setPart( dynamic_cast<SdPItemPart*>( comp->extractDefaultPartFromFacory(true, this)) );
    }
  else {
    SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( obj );
    mSymbolView->setSymbol( sym );

    SdPItemPart *prt = dynamic_cast<SdPItemPart*>( obj );
    mPartView->setPart( prt );
    }

  }




void SdDGetObject::changeEvent(QEvent *e)
  {
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
    }
  }


void SdDGetObject::accept()
  {
  //Get current row
  QSqlRecord rec = mModel->record( ui->mFindTable->currentIndex().row() );
  if( !rec.isEmpty() ) {
    //Extract name, author and id
    mObjName = rec.value( QStringLiteral("name") ).toString();
    mObjAuthor = rec.value( QStringLiteral("author") ).toString();
    mObjId     = rec.value( QStringLiteral("hash") ).toString();
    QDialog::accept();
    }
  else QMessageBox::warning( this, tr("Error"), tr("You must select element or press Cancel") );
  }




bool SdDGetObject::getObjectName(QString *name, QString *author, quint64 sort, const QString title, QWidget *parent)
  {
  SdDGetObject dget( sort, title, parent );
  if( dget.exec() ) {
    if( name ) *name = dget.getObjName();
    if( author ) *author = dget.getObjAuthor();
    return true;
    }
  return false;
  }




SdProjectItem *SdDGetObject::getObject(quint64 sort, const QString title, QWidget *parent)
  {
  SdDGetObject dget( sort, title, parent );
  if( dget.exec() ) {
    return dynamic_cast<SdProjectItem*>( SdObjectFactory::extractObject( dget.getObjId(), false, parent ) );
    }
  return nullptr;
  }
