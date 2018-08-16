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
#include "SdDNetClient.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdProjectItem.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemPart.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdSection.h"
#include "objects/SdPartVariant.h"
#include "objects/SdProject.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>


SdDGetObject::SdDGetObject(quint64 sort, const QString title, QWidget *parent) :
  QDialog(parent),
  mComponent(nullptr),
  mProject(nullptr),
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

  connect( ui->mAccept, &QPushButton::clicked, this, &SdDGetObject::accept );
  ui->mAccept->setDisabled(true);
  connect( ui->mReject, &QPushButton::clicked, this, &SdDGetObject::reject );
  connect( ui->mLoadFromCentral, &QPushButton::clicked, this, &SdDGetObject::onLoadFromCentral );
  ui->mLoadFromCentral->setDisabled(true);
  }



SdDGetObject::~SdDGetObject()
  {
  delete ui;
  }




void SdDGetObject::find()
  {
  QString name = ui->mFind->currentText();
  mHeaderList.clear();
  SdObjectFactory::forEachHeader( [this,name] (SdLibraryHeader &hdr) -> bool {
    if( hdr.mClass & mSort ) {
      //Test if name match any part of object name
      if( hdr.mName.indexOf(name, 0, Qt::CaseInsensitive) >= 0 ) {
        //Name matched, insert header in list
        mHeaderList.append( hdr );
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
    mSymbolView->setItemById( hdr.uid() );
    mPartView->setItem( nullptr, true);
    }
  else if( hdr.mClass == dctPart ) {
    mSymbolView->setItem( nullptr, true );
    mPartView->setItemById( hdr.uid() );
    }
  else if( hdr.mClass == dctComponent ) {
    mComponent = sdObjectOnly<SdPItemComponent>( SdObjectFactory::extractObject( hdr.uid(), true, this ) );
    }
  else if( hdr.mClass == dctProject ) {
    mProject = sdObjectOnly<SdProject>( SdObjectFactory::extractObject( hdr.uid(), true, this ) );
    }

  mParam = hdr.mParamTable;

  ui->mSections->clear();
  mSectionIndex = -1;

  bool present = SdObjectFactory::isObjectPresent(hdr.uid());

  if( mComponent ) {
    //Get section count
    int sectionCount = mComponent->getSectionCount();
    //If sections present, then fill visual list with sections
    if( sectionCount ) {
      for( int i = 0; i < sectionCount; i++ ) {
        ui->mSections->addItem( tr("Section %1: %2").arg(i+1).arg(mComponent->getSection(i)->getSymbolTitle()) );
        //If any section not present in library then set present to false
        if( !SdObjectFactory::isObjectPresent(mComponent->getSection(i)->getSymbolId()) )
          present = false;
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
    if( !SdObjectFactory::isObjectPresent( mComponent->getPartId() ) )
      present = false;
    }

  if( mProject ) {
    mSymbolView->setItem( mProject->getFirstSheet(), false );
    mPartView->setItem( mProject->getFirstPlate(), false );
    present = true;
    }

  ui->mAccept->setEnabled( present );
  ui->mLoadFromCentral->setDisabled( present );
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
  QStringSet set;
  SdObjectFactory::hierarchySet( path, set );
  mHeaderList.clear();
  SdObjectFactory::forEachHeader( [this, &set] (SdLibraryHeader &hdr) -> bool {
    if( hdr.mClass & mSort ) {
      //Test if tag match any part of object tag
      if( set.contains(hdr.mTag) ) {
        //Name matched, insert header in list
        mHeaderList.append( hdr );
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




//Pressed button "Load from central repository"
void SdDGetObject::onLoadFromCentral()
  {
  //Get current row
  int row = ui->mTable->currentRow();
  if( row >= 0 && row < mHeaderList.count() ) {
    //Header of selected object
    SdLibraryHeader hdr = mHeaderList.at(row);

    //If selected object not present in library then load it
    if( !SdObjectFactory::loadObject( hdr.uid(), hdr.mName, this ) )
      return;
    else
      onSelectItem( row, 0 );

    if( mComponent != nullptr ) {
      int sections = mComponent->getSectionCount();
      for( int s = 0; s < sections; s++ ) {
        //If selected object not present in library then load it
        if( !SdObjectFactory::loadObject( mComponent->getSectionSymbolId(s), mComponent->getSectionSymbolTitle(s), this ) )
          return;
        }

      if( !SdObjectFactory::loadObject( mComponent->getPartId(), mComponent->getPartTitle(), this ) )
        return;

      onSelectItem( row, 0 );
      }
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




void SdDGetObject::clearComponent()
  {
  if( mComponent != nullptr )
    delete mComponent;
  mComponent = nullptr;

  if( mProject != nullptr ) {
    //Remove views
    mSymbolView->setItem( nullptr, false );
    mPartView->setItem( nullptr, false );
    //At now delete project itself
    delete mProject;
    }
  mProject = nullptr;
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
    ui->mTable->setRowHeight( row, 20 );
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








void SdDGetObject::accept()
  {
  //Get current row
  int row = ui->mTable->currentRow();
  if( row >= 0 && row < mHeaderList.count() ) {
    //Extract name, author and id
    SdLibraryHeader hdr = mHeaderList.at(row);
    mObjName   = hdr.mName;
    mObjAuthor = hdr.mAuthor;
    mObjUid     = hdr.uid();
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




SdObject *SdDGetObject::getObject(quint64 sort, const QString title, QWidget *parent)
  {
  return SdObjectFactory::extractObject( getObjectUid( sort, title, parent), false, parent );
  }




QString SdDGetObject::getObjectUid(quint64 sort, const QString title, QWidget *parent)
  {
  SdDGetObject dget( sort, title, parent );
  if( dget.exec() )
    return dget.getObjUid();
  return QString();
  }




SdPItemComponent *SdDGetObject::getComponent(int *logSectionPtr, SdStringMap *param, const QString title, QWidget *parent)
  {
  SdDGetObject dget( dctComponent|dctInheritance, title, parent );
  if( dget.exec() ) {
    //If available pointer to logical section then set selected section
    if( logSectionPtr )
      *logSectionPtr = dget.getSectionIndex();
    //If available pointer to param then set component or instance params
    if( param )
      *param = dget.getParams();
    return sdObjectOnly<SdPItemComponent>( SdObjectFactory::extractObject( dget.getObjUid(), false, parent ) );
    }
  return nullptr;
  }
