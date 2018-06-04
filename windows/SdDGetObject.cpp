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
#include "SdWEditorGraphView.h"
#include "SdWCategory.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdProjectItem.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemPart.h"
#include "objects/SdSection.h"
#include "objects/SdPartVariant.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>


SdDGetObject::SdDGetObject(quint64 sort, const QString title, QWidget *parent) :
  QDialog(parent),
  mComponent(nullptr),
  mSectionIndex(-1), //Section index
  mSort(sort),
  ui(new Ui::SdDGetObject)
  {
  ui->setupUi(this);
  setWindowTitle( title );

  //Enable edit mode
  ui->mFind->setEditable(true);

  //Create symbol and part view
  mSymbolView = new SdWEditorGraphView( ui->mSymbolBox );
  QHBoxLayout *lay = new QHBoxLayout();
  ui->mSymbolBox->setLayout( lay );
  lay->addWidget( mSymbolView );

  mPartView = new SdWEditorGraphView( ui->mPartBox );
  lay = new QHBoxLayout();
  ui->mPartBox->setLayout( lay );
  lay->addWidget( mPartView );

  SdWCategory *category = new SdWCategory( ui->mCategoryBox );
  lay = new QHBoxLayout();
  ui->mCategoryBox->setLayout( lay );
  lay->addWidget( category );
  connect( category, &SdWCategory::tagPathSelected, this, &SdDGetObject::onTagPath );


  connect( ui->mFindButton, &QPushButton::clicked, this, &SdDGetObject::find );
  //connect( ui->mTable, &QTableWidget::cellActivated, this, &SdDGetObject::onSelectItem );
  connect( ui->mTable, &QTableWidget::cellClicked, this, &SdDGetObject::onSelectItem );
  connect( ui->mSections, &QListWidget::currentRowChanged, this, &SdDGetObject::onCurrentSection );
  ui->mSections->setSortingEnabled(false);
  }



SdDGetObject::~SdDGetObject()
  {
  delete ui;
  }




void SdDGetObject::find()
  {
  QString name = ui->mFind->currentText();
  mHeaderList.clear();
  mHeaderMap.clear();
  SdObjectFactory::forEachHeader( [this,name] (SdLibraryHeader &hdr) -> bool {
    if( hdr.mClass & mSort ) {
      //Test if name match any part of object name
      if( hdr.mName.indexOf(name, 0, Qt::CaseInsensitive) >= 0 ) {
        //Name matched, insert header in list
        appendNewly( hdr );
        //Prevent too much headers in find result
        if( mHeaderList.count() > 300 )
          return true;
        }
      }
    //Continue execution
    return false;
    });
  //Fill visual table
  fillTable();
  }




void SdDGetObject::onSelectItem(int row, int column)
  {
  Q_UNUSED(column);
  clearComponent();
  SdLibraryHeader hdr = mHeaderList.at(row);
  if( hdr.mClass == dctSymbol || hdr.mClass == dctSheet ) {
    mSymbolView->setItemById( hdr.id() );
    mPartView->setItem( nullptr, true);
    }
  else if( hdr.mClass == dctPart ) {
    mSymbolView->setItem( nullptr, true );
    mPartView->setItemById( hdr.id() );
    }
  else if( hdr.mClass == dctComponent ) {
    SdObject *obj = SdObjectFactory::extractObject( hdr.id(), true, this );
    mComponent = dynamic_cast<SdPItemComponent*>( obj );
    if( mComponent == nullptr && obj != nullptr ) delete obj;
    }

  ui->mSections->clear();
  mSectionIndex = -1;

  if( mComponent ) {
    //Get section count
    int sectionCount = mComponent->getSectionCount();
    //If sections present, then fill visual list with sections
    if( sectionCount ) {
      for( int i = 0; i < sectionCount; i++ ) {
        ui->mSections->addItem( tr("Section %1: %2").arg(i+1).arg(mComponent->getSection(i)->getSymbolTitle()) );
        }
      ui->mSections->setCurrentRow(0);
      mSectionIndex = 0;
      mSymbolView->setItemById( mComponent->getSectionSymbolId(0) );
      }
    else {
      mSymbolView->setItem( mComponent, true );
      }

    //Setup part for view
    mPartView->setItemById( mComponent->getPartId() );
    }
  }




//On change component section selection
void SdDGetObject::onCurrentSection(int row)
  {
  if( mComponent != nullptr ) {
    mSymbolView->setItemById( mComponent->getSectionSymbolId(row) );
    mSectionIndex = row;
    }
  }




//Selected new category, apply filtr
void SdDGetObject::onTagPath(const QString path)
  {
  mHeaderList.clear();
  mHeaderMap.clear();
  SdObjectFactory::forEachHeader( [this,path] (SdLibraryHeader &hdr) -> bool {
    if( hdr.mClass & mSort ) {
      //Test if tag match any part of object tag
      if( hdr.mTag.indexOf(path, 0, Qt::CaseInsensitive) >= 0 || path.indexOf(hdr.mTag, 0, Qt::CaseInsensitive) >= 0 ) {
        //Name matched, insert header in list
        appendNewly( hdr );
        //Prevent too much headers in find result
        if( mHeaderList.count() > 300 )
          return true;
        }
      }
    //Continue execution
    return false;
    });

  //Fill visual table
  fillTable();
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

void SdDGetObject::clearComponent()
  {
  if( mComponent != nullptr )
    delete mComponent;
  mComponent = nullptr;
  }




//Fill visual table with mHeaderList contens
void SdDGetObject::fillTable()
  {
  ui->mTable->clear();
  ui->mTable->setColumnCount(3);
  ui->mTable->setRowCount( mHeaderList.count() );
  QStringList header;
  header << tr("Name") << tr("Author") << tr("Rank");
  ui->mTable->setHorizontalHeaderLabels( header );
  int row = 0;
  for( SdLibraryHeader &hdr : mHeaderList ) {
    ui->mTable->setItem( row, 0, new QTableWidgetItem(hdr.mName) );
    ui->mTable->setItem( row, 1, new QTableWidgetItem(hdr.mAuthor) );
    ui->mTable->setItem( row, 2, new QTableWidgetItem( QString::number(hdr.mTime) ) );
    row++;
    }

  clearComponent();
  mSymbolView->setItem(nullptr,true);
  mPartView->setItem(nullptr,true);
  ui->mSections->clear();
  mSectionIndex = -1;
  }




void SdDGetObject::appendNewly(SdLibraryHeader &hdr)
  {
  QString id = hdr.typeNameAndAuthor();
  if( mHeaderMap.contains(id) ) {
    int index = mHeaderMap.value(id);
    if( mHeaderList.at(index).mTime < hdr.mTime )
      mHeaderList[index] = hdr;
    }
  else {
    mHeaderMap.insert(id,mHeaderList.count());
    mHeaderList.append( hdr );
    }
  }




void SdDGetObject::accept()
  {
  //Get current row
  int row = ui->mTable->currentRow();
  if( row >= 0 && row < mHeaderList.count() ) {
    //Extract name, author and id
    SdLibraryHeader hdr = mHeaderList.at(row);
    mObjName   = hdr.mName;
    mObjAuthor = hdr.mAuthor;
    mObjId     = hdr.id();
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
  return dynamic_cast<SdProjectItem*>( SdObjectFactory::extractObject( getObjectId( sort, title, parent), false, parent ) );
  }




QString SdDGetObject::getObjectId(quint64 sort, const QString title, QWidget *parent)
  {
  SdDGetObject dget( sort, title, parent );
  if( dget.exec() )
    return dget.getObjId();
  return QString();
  }




SdProjectItem *SdDGetObject::getComponent(int *logSectionPtr, quint64 sort, const QString title, QWidget *parent)
  {
  SdDGetObject dget( sort, title, parent );
  if( dget.exec() ) {
    if( logSectionPtr )
      *logSectionPtr = dget.getSectionIndex();
    return dynamic_cast<SdProjectItem*>( SdObjectFactory::extractObject( dget.getObjId(), false, parent ) );
    }
  return nullptr;
  }
