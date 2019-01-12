/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for retrive object from dataBase
*/

#include "SdDGetObject.h"
#include "ui_SdDGetObject.h"
#include "SdWEditorGraphView.h"
#include "SdDNetClient.h"
#include "SdDRowValue.h"
#include "SdWCategoryList.h"
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
#include "objects/SdTime2x.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QMap>
#include <QSet>
#include <QTimer>
#include <QDebug>

//Storage for previous entered name filter
static QString sdNameFilter;
//Storage for previous entered param filter
static QString sdParamFilter;

//=========================================================================================
//Struct define individual field mask when find process
//It used to compare with field value of filtered pretendent
struct SdFieldMask {
    QString mFieldName;  //Field name on which matching executes
    QString mFieldValue; //Text field value for field match by text contents
    double  mMin;        //Min value for field match by great and between
    double  mMax;        //Max value for field match by less and between
    enum {
      txtMatch  = 0x0, //Field matched by text contents
      doubleMin = 0x1, //Field matched if value great or equal than mMin
      doubleMax = 0x2, //Field matched if value less or equal than mMax
      doubleDia = 0x3  //Field matched if value is between mMin and mMax
      }     mOper;

    //Constructor parses value
    SdFieldMask( const QString name, const QString val );

    //Match field mask and field in param map
    bool match( const SdStringMap &map ) const;
  };


//Field mask list
typedef QList<SdFieldMask> SdFieldMaskList;



//Constructor parses value
SdFieldMask::SdFieldMask( const QString name, const QString val ) :
  mFieldName(name),
  mFieldValue(val),
  mMin(0.0),
  mMax(0.0)
  {
  if( val.contains(QChar('<')) || val.contains(QChar('>')) ) {
    //Variant for less, great or between
    QString great;
    QString less;
    if( val.contains(QChar('<')) ) {
      QStringList lst = val.split(QChar('<'));
      great = lst.at(0);
      less  = lst.at(1);
      }
    else {
      QStringList lst = val.split(QChar('>'));
      great = lst.at(1);
      less  = lst.at(0);
      }
    mOper = doubleDia;
    if( !great.isEmpty() ) {
      mMin = SdDRowValue::phisToDouble( great );
      mOper = doubleMin;
      }
    if( !less.isEmpty() ) {
      mMax = SdDRowValue::phisToDouble( less );
      if( mOper == doubleDia ) mOper = doubleMax;
      else mOper = doubleDia;
      }
    }
  else mOper = txtMatch;
  }




//Match field mask and field in param map
bool SdFieldMask::match(const SdStringMap &map) const
  {
  //If no field with this name, then not matched
  if( !map.contains(mFieldName) )
    return false;

  //Value of field
  QString src = map.value(mFieldName);

  switch( mOper ) {
    case txtMatch :
      return src.indexOf( mFieldValue, 0, Qt::CaseInsensitive ) >= 0;
    case doubleMin :
      return mMin <= SdDRowValue::phisToDouble( src );
    case doubleMax :
      return mMax >= SdDRowValue::phisToDouble( src );
    case doubleDia : {
      double v = SdDRowValue::phisToDouble( src );
      return v >= mMin && v <= mMax;
      }
    }
  return false;
  }







//=========================================================================================
//A lot of static variables need because dialog must be start at axact it close point

//Field mask list
static SdFieldMaskList sdFieldMaskList;




static bool sdFieldMaskListMatch( const SdStringMap &map ) {
  for( const SdFieldMask &msk : sdFieldMaskList )
    if( !msk.match(map) ) return false;
  return true;
  }




static bool sdFieldListMatch( const QStringList &list, const SdStringMap &map ) {
  for( const QString &str : list ) {
    bool match = false;
    for( auto iter = map.cbegin(); iter != map.cend() && !match; iter++ )
      match = iter.value().indexOf( str, 0, Qt::CaseInsensitive ) >= 0;
    if( !match ) return false;
    }
  return true;
  }



//Fields width
static QMap<QString,int> sdFieldWidth;

static QSize prevDialogSize;
static int   prevFieldsBoxWidth[3] = { 40, 100, 130 };


QString                SdDGetObject::mObjName;      //Object name
QString                SdDGetObject::mObjUid;       //Unical object id
QString                SdDGetObject::mCompUid;      //Unical object id for components
int                    SdDGetObject::mSectionIndex; //Section index
SdStringMap            SdDGetObject::mParam;        //Component or instance params

quint64                SdDGetObject::mSort;         //Object select sort (class)
SdLibraryHeaderList    SdDGetObject::mHeaderList;   //Header list for filtered objects
bool                   SdDGetObject::mExpandVariant = false;    //Flag for find only in titles and not show variants

SdDGetObject::SdDGetObject(quint64 sort, const QString title, QWidget *parent) :
  QDialog(parent),
  mComponent(nullptr),
  mProject(nullptr),
  ui(new Ui::SdDGetObject)
  {
  if( !prevDialogSize.isEmpty() )
    resize( prevDialogSize );
  ui->setupUi(this);
  setWindowTitle( title );

  //Prepare field box
  ui->mFieldsBox->setColumnCount(3);
  ui->mFieldsBox->setHorizontalHeaderLabels( {tr("Show"), tr("Field"), tr("Filtr") } );
  ui->mFieldsBox->setColumnWidth( 0, prevFieldsBoxWidth[0] );
  ui->mFieldsBox->setColumnWidth( 1, prevFieldsBoxWidth[1] );
  ui->mFieldsBox->setColumnWidth( 2, prevFieldsBoxWidth[2] );

  //Preset filter
  ui->mNameFilter->setText( sdNameFilter );
  ui->mParamFilter->setText( sdParamFilter );

  //Create symbol and part view
  mSymbolView = new SdWEditorGraphView( ui->mSymbolBox );
  QHBoxLayout *lay = new QHBoxLayout();
  ui->mSymbolBox->setLayout( lay );
  lay->addWidget( mSymbolView );

  mPartView = new SdWEditorGraphView( ui->mPartBox );
  lay = new QHBoxLayout();
  ui->mPartBox->setLayout( lay );
  lay->addWidget( mPartView );

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

  //Category
  mCategoryList = new SdWCategoryList(nullptr);
  ui->mCategoryBox->addWidget( mCategoryList );
  connect( mCategoryList, &SdWCategoryList::category, this, &SdDGetObject::onCategory );


  connect( ui->mClearFields, &QPushButton::clicked, this, &SdDGetObject::onClearFieldFiltr );

  //Default filtr only title or by fields also
  ui->mTitleOnlyFiltr->setChecked( mExpandVariant );

  ui->mNameFilter->setFocus();

  if( mSort == sort )
    QTimer::singleShot( 300, this, [this] () {
      //Continue with previous filtrs
      int activeSection = mSectionIndex;
      int activeRow = 0;
      for( SdLibraryHeader &hdr : mHeaderList ) {
        if( hdr.uid() == mObjUid ) break;
        activeRow++;
        }
      //Fill visual tables
      fillTable();
      //Select visual object
      if( activeRow < mHeaderList.count() ) {
        ui->mTable->setCurrentCell( activeRow, 0 );
        onSelectItem( activeRow, 0 );
        //Select visual section
        ui->mSections->setCurrentRow( activeSection );
        onCurrentSection( activeSection );
        }
      });
  else {
    mSectionIndex = -1;
    mSort = sort;
    }
  }



SdDGetObject::~SdDGetObject()
  {
  //Store width of param table
  prevFieldsBoxWidth[0] = ui->mFieldsBox->columnWidth( 0 );
  prevFieldsBoxWidth[1] = ui->mFieldsBox->columnWidth( 1 );
  prevFieldsBoxWidth[2] = ui->mFieldsBox->columnWidth( 2 );

  //Store width of column find result table
  for( int i = 0; i < ui->mTable->columnCount(); i++ )
    sdFieldWidth.insert( ui->mTable->horizontalHeaderItem(i)->text(), ui->mTable->columnWidth(i) );

  delete ui;
  }




void SdDGetObject::find()
  {
  //Get fileds filtr
  sdFieldMaskList.clear();
  for( int i = 0; i < ui->mFieldsBox->rowCount(); i++ )
    if( !ui->mFieldsBox->item( i, 2 )->text().isEmpty() ) {
      SdFieldMask mask( ui->mFieldsBox->item( i, 1 )->text(), ui->mFieldsBox->item( i, 2 )->text() );
      sdFieldMaskList.append( mask );
      }
  sdNameFilter = ui->mNameFilter->text();
  QStringList list = sdNameFilter.split( QRegExp("\\s+"), QString::SkipEmptyParts );
  mExpandVariant = ui->mTitleOnlyFiltr->isChecked();
  mHeaderList.clear();

  //Accumulate uid's matched to filter
  QStringList uidList;
  SdObjectFactory::forEachUid( [list, &uidList] (const QString &uid) -> bool {
    //Split uid to name type and author
    QString name = uid.contains(sdUidDelimiter) ? uid.split( sdUidDelimiter ).at(1) : uid;
    //Test if all name filter sections match any part of object name
    for( const QString &flt : list )
      if( name.indexOf( flt, 0, Qt::CaseInsensitive ) < 0 )
        //Name not matched, continue with another uid
        return false;
    //Name matched
    uidList.append( uid );
    return uidList.count() > 1000;
    });

  sdParamFilter = ui->mParamFilter->text();
  list = sdParamFilter.split( QRegExp("\\s+"), QString::SkipEmptyParts );
  SdLibraryHeader hdr;
  for( const QString &str : uidList ) {
    SdObjectFactory::extractHeader( str, hdr );

    //test class
    if( hdr.mClass & mSort ) {
      //Match to fileds
      if( mExpandVariant && hdr.variantTableExist() ) {
        int c = hdr.variantCount();
        SdLibraryHeader vhdr;
        if( list.count() ) {
          //Match each list item to any param
          for( int i = 0; i < c; i++ ) {
            hdr.variant( vhdr, i );
            if( sdFieldListMatch( list, vhdr.mParamTable ) ) {
              //Params matched, insert header in list
              mHeaderList.append( vhdr );
              }
            }
          }
        else {
          for( int i = 0; i < c; i++ ) {
            hdr.variant( vhdr, i );
            if( sdFieldMaskListMatch(vhdr.mParamTable) ) {
              //Params matched, insert header in list
              mHeaderList.append( vhdr );
              }
            }
          }
        }
      else {
        if( list.count() ) {
          if( sdFieldListMatch( list, hdr.mParamTable ) ) {
            //Params matched, insert header in list
            mHeaderList.append( hdr );
            }
          }
        else {
          //Name matched, test params
          if( sdFieldMaskListMatch(hdr.mParamTable) ) {
            //Params matched, insert header in list
            mHeaderList.append( hdr );
            }
          }
        }
      }
    }

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
  else if( hdr.mClass == dctInheritance ) {
    mComponent = sdObjectOnly<SdPItemComponent>( SdObjectFactory::extractObject( hdr.mInherit, true, this ) );
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

  if( !present ) {
    mSymbolView->setItem( nullptr, true );
    mPartView->setItem( nullptr, true );
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





//When changed field filtr
void SdDGetObject::onFieldChanged(int row, int column )
  {
  if( column == 2 )
    find();
  if( column == 0 ) {
    //Field show flag changed
    bool show = ui->mFieldsBox->item( row, 0 )->checkState() == Qt::Checked;
    if( show )
      ui->mTable->showColumn( row + 3 );
    else
      ui->mTable->hideColumn( row + 3 );
    }
  }





//Clear all fields filtr
void SdDGetObject::onClearFieldFiltr()
  {
  disconnect( ui->mFieldsBox, &QTableWidget::cellChanged, this, &SdDGetObject::onFieldChanged );
  for( int i = 0; i < ui->mFieldsBox->rowCount(); i++ )
    ui->mFieldsBox->item( i, 2 )->setText( QString() );
  find();
  }






//On category selected
void SdDGetObject::onCategory(const QString str)
  {
  ui->mNameFilter->setText( str );
  find();
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




//Fill visual table with mHeaderList contents
void SdDGetObject::fillTable()
  {
  disconnect( ui->mFieldsBox, &QTableWidget::cellChanged, this, &SdDGetObject::onFieldChanged );

  //Accumulate used fields
  QSet<QString> fields;
  for( SdLibraryHeader &hdr : mHeaderList ) {
    for( auto iter = hdr.mParamTable.cbegin(); iter != hdr.mParamTable.cend(); iter++ )
      fields.insert( iter.key() );
    }

  //Leave in field list only fields with value
  int row = ui->mFieldsBox->rowCount();
  for( int i = 0; i < ui->mFieldsBox->rowCount(); )
    if( ui->mFieldsBox->item( i, 2 )->text().isEmpty() && !fields.contains(ui->mFieldsBox->item( i, 1 )->text()) )
      ui->mFieldsBox->removeRow(i);
    else {
      //Remove from accumulated fields fields with value which already in table
      fields.remove( ui->mFieldsBox->item( i, 1 )->text() );
      i++;
      }

  //Leaved fields append from set to visual table
  for( auto iter = fields.cbegin(); iter != fields.cend(); iter++ ) {
    row = ui->mFieldsBox->rowCount();
    //Append new field row
    ui->mFieldsBox->insertRow( row );
    ui->mFieldsBox->setRowHeight( row, 20 );
    //Setup field params
    //Field visibility
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
    item->setCheckState( Qt::Checked );
    ui->mFieldsBox->setItem( row, 0, item );
    //Field name
    ui->mFieldsBox->setItem( row, 1, new QTableWidgetItem( *iter ) );
    //Disable edit
    ui->mFieldsBox->item( row, 1 )->setFlags( Qt::ItemIsEnabled );
    //Field mask
    ui->mFieldsBox->setItem( row, 2, new QTableWidgetItem() );

    //Test if field in field width map
    if( !sdFieldWidth.contains( *iter ) ) {
      //Append default field width
      sdFieldWidth.insert( *iter, 100 );
      }
    }

  //Read current fields width
  for( int i = 0; i < ui->mTable->columnCount(); i++ )
    sdFieldWidth.insert( ui->mTable->horizontalHeaderItem(i)->text(), ui->mTable->columnWidth(i) );


  ui->mTable->clear();
  ui->mTable->setColumnCount(ui->mFieldsBox->rowCount() + 3);
  //Fill column header
  //First three columns - are name, author and time of creation
  ui->mTable->setHorizontalHeaderItem( 0, new QTableWidgetItem( tr("Name") ) );
  ui->mTable->setHorizontalHeaderItem( 1, new QTableWidgetItem( tr("Author") ) );
  ui->mTable->setHorizontalHeaderItem( 2, new QTableWidgetItem( tr("Created") ) );
  //Other columns - fields
  for( int i = 0; i < ui->mFieldsBox->rowCount(); i++ ) {
    ui->mTable->setHorizontalHeaderItem(i+3, new QTableWidgetItem( ui->mFieldsBox->item(i,1)->text() ) );
    if( ui->mFieldsBox->item( i, 0 )->checkState() != Qt::Checked )
      ui->mTable->hideColumn( i + 3 );
    }

  //Setup column width
  for( int i = 0; i < ui->mTable->columnCount(); i++ ) {
    QString name = ui->mTable->horizontalHeaderItem(i)->text();
    ui->mTable->setColumnWidth( i, sdFieldWidth.value(name, 100) );
    }

  //Fill table
  ui->mTable->setRowCount( mHeaderList.count() );
  row = 0;
  for( SdLibraryHeader &hdr : mHeaderList ) {
    ui->mTable->setRowHeight( row, 20 );
    ui->mTable->setItem( row, 0, new QTableWidgetItem(hdr.mName) );
    ui->mTable->setItem( row, 1, new QTableWidgetItem(hdr.mAuthor) );
    ui->mTable->setItem( row, 2, new QTableWidgetItem(SdTime2x::toLocalString(hdr.mTime)) );
    //Fill fields
    for( int i = 3; i < ui->mTable->columnCount(); i++ ) {
      QString name = ui->mTable->horizontalHeaderItem(i)->text();
      ui->mTable->setItem( row, i, new QTableWidgetItem(hdr.mParamTable.value(name)) );
      }
    row++;
    }

  clearComponent();
  mSymbolView->setItem(nullptr,true);
  mPartView->setItem(nullptr,true);
  ui->mSections->clear();
  mSectionIndex = -1;

  connect( ui->mFieldsBox, &QTableWidget::cellChanged, this, &SdDGetObject::onFieldChanged );
  }








void SdDGetObject::accept()
  {
  //Get current row
  int row = ui->mTable->currentRow();
  if( row >= 0 && row < mHeaderList.count() ) {
    //Extract name, author and id
    SdLibraryHeader hdr = mHeaderList.at(row);
    mObjName   = hdr.mName;
    mObjUid     = hdr.uid();
    if( hdr.mClass == dctInheritance )
      mCompUid = hdr.mInherit;
    else if( hdr.mClass == dctComponent )
      mCompUid = mObjUid;
    else
      mCompUid.clear();
    //Save current dialog state
    prevDialogSize = size();
    prevFieldsBoxWidth[0] = ui->mFieldsBox->columnWidth(0);
    prevFieldsBoxWidth[1] = ui->mFieldsBox->columnWidth(1);
    prevFieldsBoxWidth[2] = ui->mFieldsBox->columnWidth(2);
    QDialog::accept();
    }
  else QMessageBox::warning( this, tr("Error"), tr("You must select element or press Cancel") );
  }






SdObject *SdDGetObject::getObject(quint64 sort, const QString title, QWidget *parent)
  {
  return SdObjectFactory::extractObject( getObjectUid( sort, title, parent), false, parent );
  }




QString SdDGetObject::getObjectUid(quint64 sort, const QString title, QWidget *parent)
  {
  SdDGetObject dget( sort, title, parent );
  if( dget.exec() )
    return mObjUid;
  return QString();
  }




SdPItemComponent *SdDGetObject::getComponent(int *logSectionPtr, SdStringMap *param, const QString title, QWidget *parent)
  {
  SdDGetObject dget( dctComponent|dctInheritance, title, parent );
  if( dget.exec() ) {
    //If available pointer to logical section then set selected section
    if( logSectionPtr )
      *logSectionPtr = mSectionIndex;
    //If available pointer to param then set component or instance params
    if( param )
      *param = mParam;
    return sdObjectOnly<SdPItemComponent>( SdObjectFactory::extractObject( mCompUid, false, parent ) );
    }
  return nullptr;
  }

