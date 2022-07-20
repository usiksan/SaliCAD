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
#include "SdWEditor3dPartView.h"
#include "SdDRowValue.h"
#include "SdWCategoryList.h"
#include "library/SdLibraryStorage.h"
#include "objects/SdProjectItem.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemPart.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdSection.h"
#include "objects/SdPartVariant.h"
#include "objects/SdProject.h"
#include "SvLib/SvTime2x.h"
#include "SdDHelp.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QMap>
#include <QSet>
#include <QTimer>
#include <QSettings>
#include <QDataStream>
#include <QDebug>

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






bool sdFieldMapMatch( const QString src, const SdStringMap &map ) {
  for( auto iter = map.cbegin(); iter != map.cend(); iter++ )
    if( iter.value().indexOf( src, 0, Qt::CaseInsensitive ) >= 0 ) return true;
  return false;
  }



//Fields width
static QMap<QString,int> sdFieldWidth;




QString                SdDGetObject::mObjName;      //Object name
QString                SdDGetObject::mObjUid;       //Unical object id
QString                SdDGetObject::mCompUid;      //Unical object id for components
int                    SdDGetObject::mSectionIndex; //Section index
SdStringMap            SdDGetObject::mParam;        //Component or instance params

quint64                SdDGetObject::mSort;         //Object select sort (class)
SdLibraryHeaderList    SdDGetObject::mHeaderList;   //Header list for filtered objects
bool                   SdDGetObject::mExpandVariant = false;    //Flag for find only in titles and not show variants

SdDGetObject::SdDGetObject(quint64 sort, const QString title, const QString &defFilter, QWidget *parent) :
  QDialog(parent),
  mComponent(nullptr),
  mProject(nullptr)
  {
  setWindowTitle( title );


  //Build dialog
  QVBoxLayout *centralLayout = new QVBoxLayout();
  setLayout( centralLayout );

  mSplitCentral = new QSplitter();
  centralLayout->addWidget( mSplitCentral );

  mDialogButtons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
  centralLayout->addWidget( mDialogButtons, 0, Qt::AlignRight );

  //Category
  QWidget *category = new QWidget();
  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->setMargin(0);
  vbox->setContentsMargins( 0,0,0,0 );
  vbox->addWidget( new QLabel( tr("Category") )  );
  category->setLayout( vbox );
  mSplitCentral->addWidget( category );
  mCategoryList = new SdWCategoryList(nullptr);
  vbox->addWidget( mCategoryList );
  connect( mCategoryList, &SdWCategoryList::category, this, &SdDGetObject::onCategory );

  QWidget *fieldsBox = new QWidget();
  vbox = new QVBoxLayout();
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget( new QLabel( tr("Fields filter") )  );
  QPushButton *but = new QPushButton( tr("Clear all") );
  hbox->addWidget( but, 0, Qt::AlignRight );
  connect( but, &QPushButton::clicked, this, &SdDGetObject::onClearFieldFiltr );
  vbox->addLayout( hbox );
  vbox->addWidget( mFieldsBox = new QTableWidget() );
  fieldsBox->setLayout( vbox );
  mSplitCentral->addWidget( fieldsBox );

  //Prepare field box
  mFieldsBox->setColumnCount(3);
  mFieldsBox->setHorizontalHeaderLabels( {tr("Show"), tr("Field"), tr("Filtr") } );
  mFieldsBox->setColumnWidth( 0, 40 );
  mFieldsBox->setColumnWidth( 1, 100 );
  mFieldsBox->setColumnWidth( 2, 130 );

  //Find results table
  mSplitFinder = new QSplitter(Qt::Vertical);
  mSplitCentral->addWidget( mSplitFinder );

  QWidget *finder = new QWidget();
  vbox = new QVBoxLayout();
  hbox = new QHBoxLayout();
  hbox->addWidget( new QLabel( tr("Enter object to find:") )  );
  hbox->addWidget( mNameFilter = new QLineEdit() );
  hbox->addWidget( but = new QPushButton( tr("Find") ) );
  connect( but, &QPushButton::clicked, this, &SdDGetObject::find );
  but->setDefault(true);
  vbox->addLayout( hbox );
  vbox->addWidget( mTable = new QTableWidget() );
  connect( mTable, &QTableWidget::cellClicked, this, &SdDGetObject::onSelectItem );
  finder->setLayout( vbox );
  mSplitFinder->addWidget( finder );

  //Section selection and preview
  mSplitView = new QSplitter();
  mSplitFinder->addWidget( mSplitView );

  QGroupBox *gbox = new QGroupBox( tr("Sections") );
  hbox = new QHBoxLayout();
  gbox->setLayout( hbox );
  hbox->addWidget( mSections = new QListWidget() );
  mSplitView->addWidget( gbox );
  connect( mSections, &QListWidget::currentRowChanged, this, &SdDGetObject::onCurrentSection );
  mSections->setSortingEnabled(false);

  //Create symbol and part view
  gbox = new QGroupBox( tr("Symbol") );
  hbox = new QHBoxLayout();
  gbox->setLayout( hbox );
  hbox->addWidget( mSymbolView = new SdWEditorGraphView( gbox ) );
  mSplitView->addWidget( gbox );

  gbox = new QGroupBox( tr("Part") );
  hbox = new QHBoxLayout();
  gbox->setLayout( hbox );
  hbox->addWidget( mPartView = new SdWEditorGraphView( gbox ) );
  mSplitView->addWidget( gbox );

  gbox = new QGroupBox( tr("3D") );
  hbox = new QHBoxLayout();
  gbox->setLayout( hbox );
  hbox->addWidget( m3dView = new SdWEditor3d( nullptr, gbox ) );
  mSplitView->addWidget( gbox );


  mNameFilter->setFocus();

  //Help system
  connect( mDialogButtons->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDGetObject.htm"), this );
    } );

  connect( mDialogButtons->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &SdDGetObject::accept );
  mDialogButtons->button(QDialogButtonBox::Ok)->setDisabled(true);
  connect( mDialogButtons->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &SdDGetObject::reject );

  mNameFilter->setText( defFilter );

  QSettings s;
  if( s.contains( QStringLiteral(SDK_GET_OBJECT_DLG_CFG) )  ) {
    QByteArray ar = s.value( QStringLiteral(SDK_GET_OBJECT_DLG_CFG) ).toByteArray();
    QDataStream is(ar);
    QSize dlgSize;
    QString nameFilter;
    int fieldsBoxWidth0, fieldsBoxWidth1, fieldsBoxWidth2;
    QByteArray centralSplitter;
    QByteArray findSplitter;
    QByteArray viewSplitter;

    is >> dlgSize >> nameFilter >> fieldsBoxWidth0 >> fieldsBoxWidth1 >> fieldsBoxWidth2 >> centralSplitter >> findSplitter >> viewSplitter;

    resize( dlgSize );

    mFieldsBox->setColumnWidth( 0, fieldsBoxWidth0 );
    mFieldsBox->setColumnWidth( 1, fieldsBoxWidth1 );
    mFieldsBox->setColumnWidth( 2, fieldsBoxWidth2 );
    //Preset filter
    if( defFilter.isEmpty() )
      mNameFilter->setText( nameFilter );
    mSplitCentral->restoreState( centralSplitter );
    mSplitFinder->restoreState( findSplitter );
    mSplitView->restoreState( viewSplitter );
    }


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
        mTable->setCurrentCell( activeRow, 0 );
        onSelectItem( activeRow, 0 );
        //Select visual section
        mSections->setCurrentRow( activeSection );
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
  QByteArray ar;
  QDataStream os(&ar, QIODevice::WriteOnly );
  QSize dlgSize( size() );
  QString nameFilter( mNameFilter->text() );
  int fieldsBoxWidth0(mFieldsBox->columnWidth(0)), fieldsBoxWidth1(mFieldsBox->columnWidth(1)), fieldsBoxWidth2(mFieldsBox->columnWidth(2));
  QByteArray centralSplitter( mSplitCentral->saveState() );
  QByteArray findSplitter( mSplitFinder->saveState() );
  QByteArray viewSplitter( mSplitView->saveState() );

  os << dlgSize << nameFilter << fieldsBoxWidth0 << fieldsBoxWidth1 << fieldsBoxWidth2 << centralSplitter << findSplitter << viewSplitter;

  QSettings s;
  s.setValue( QStringLiteral(SDK_GET_OBJECT_DLG_CFG), ar );

  //Store width of column find result table
  for( int i = 0; i < mTable->columnCount(); i++ )
    sdFieldWidth.insert( mTable->horizontalHeaderItem(i)->text(), mTable->columnWidth(i) );
  }




void SdDGetObject::find()
  {
  //Get fileds filtr
  sdFieldMaskList.clear();
  for( int i = 0; i < mFieldsBox->rowCount(); i++ )
    if( !mFieldsBox->item( i, 2 )->text().isEmpty() ) {
      SdFieldMask mask( mFieldsBox->item( i, 1 )->text(), mFieldsBox->item( i, 2 )->text() );
      sdFieldMaskList.append( mask );
      }
  auto sdNameFilter = mNameFilter->text();
  QStringList list = sdNameFilter.split( QRegExp("\\s+"), Qt::SkipEmptyParts );
  mHeaderList.clear();

  //Accumulate headers matched to filter
  SdLibraryStorage::instance()->forEachHeader( [list] ( SdLibraryHeader &hdr ) -> bool {
    //test class
    if( hdr.mClass & mSort ) {
      //Split uid to name type and author
      QString uid( hdr.uid() );
      QString name = uid.contains(sdUidDelimiter) ? uid.split( sdUidDelimiter ).at(1) : uid;

      //Test for all variants
      if( hdr.variantTableExist() ) {
        //Header has variant table - append all variants
        int c = hdr.variantCount();
        SdLibraryHeader vhdr;
        for( int i = 0; i < c; i++ ) {
          hdr.variant( vhdr, i );
          //Test if matched to name or params
          bool matched = true;
          for( const QString &flt : list )
            if( name.indexOf( flt, 0, Qt::CaseInsensitive ) < 0 && !sdFieldMapMatch( flt, vhdr.mParamTable ) ) {
              matched = false;
              break;
              }
          if( matched && sdFieldMaskListMatch( vhdr.mParamTable ) )
            mHeaderList.append( vhdr );
          }
        }
      else {
        //Header has only one param set - append header
        //Test if matched to name or params
        bool matched = true;
        for( const QString &flt : list )
          if( name.indexOf( flt, 0, Qt::CaseInsensitive ) < 0 && !sdFieldMapMatch( flt, hdr.mParamTable ) ) {
            matched = false;
            break;
            }
        if( matched && sdFieldMaskListMatch( hdr.mParamTable ) )
          mHeaderList.append( hdr );
        }
      }

    return mHeaderList.count() > 1000;
    });

  //Fill visual table
  fillTable();
  }




void SdDGetObject::onSelectItem(int row, int column)
  {
  Q_UNUSED(column)
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
    mComponent = sdObjectOnly<SdPItemComponent>( SdLibraryStorage::instance()->cfObjectGet( hdr.uid() ) );
    }
  else if( hdr.mClass == dctProject ) {
    mProject = sdObjectOnly<SdProject>( SdLibraryStorage::instance()->cfObjectGet( hdr.uid() ) );
    }

  mParam = hdr.mParamTable;

  mSections->clear();
  mSectionIndex = -1;

  bool present = SdLibraryStorage::instance()->cfObjectContains( hdr.uid() );

  if( mComponent ) {
    //Get section count
    int sectionCount = mComponent->getSectionCount();
    //If sections present, then fill visual list with sections
    if( sectionCount ) {
      for( int i = 0; i < sectionCount; i++ ) {
        mSections->addItem( tr("Section %1: %2").arg(i+1).arg(mComponent->getSection(i)->getSymbolTitle()) );
        //If any section not present in library then set present to false
        if( !SdLibraryStorage::instance()->cfObjectContains(mComponent->getSection(i)->getSymbolId()) )
          present = false;
        }
      mSections->setCurrentRow(0);
      mSectionIndex = 0;
      mSymbolView->setItemById( mComponent->getSectionSymbolId(0) );
      }
    else {
      mSymbolView->setItem( mComponent, true );
      }

    //Setup part for view
    mPartView->setItemById( mComponent->getPartId() );
    if( !SdLibraryStorage::instance()->cfObjectContains( mComponent->getPartId() ) )
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

  m3dView->setProjectItem( mPartView->getProjectItem() );

  mDialogButtons->button(QDialogButtonBox::Ok)->setEnabled( present );
  }




//On change component section selection
void SdDGetObject::onCurrentSection(int row)
  {
  if( mComponent != nullptr ) {
    mSymbolView->setItemById( mComponent->getSectionSymbolId(row) );
    mSectionIndex = row;
    }
  }








//When changed field filtr
void SdDGetObject::onFieldChanged(int row, int column )
  {
  if( column == 2 )
    find();
  if( column == 0 ) {
    //Field show flag changed
    bool show = mFieldsBox->item( row, 0 )->checkState() == Qt::Checked;
    if( show )
      mTable->showColumn( row + 3 );
    else
      mTable->hideColumn( row + 3 );
    }
  }





//Clear all fields filtr
void SdDGetObject::onClearFieldFiltr()
  {
  disconnect( mFieldsBox, &QTableWidget::cellChanged, this, &SdDGetObject::onFieldChanged );
  for( int i = 0; i < mFieldsBox->rowCount(); i++ )
    mFieldsBox->item( i, 2 )->setText( QString() );
  find();
  }






//On category selected
void SdDGetObject::onCategory(const QString str)
  {
  //Remove previous category
  QStringList list = mNameFilter->text().split( QChar(' '), Qt::SkipEmptyParts );
  for( int i = 0; i < list.count(); i++ )
    if( list.at(i).startsWith( QStringLiteral(SD_CATEGORY_PREFIX) )   ) {
      list.removeAt(i);
      break;
      }
  //Append new category to current filter
  mNameFilter->setText( list.join( QChar(' ') ) + QChar(' ') + str );
  find();
  }






void SdDGetObject::changeEvent(QEvent *e)
  {
  QDialog::changeEvent(e);
//  switch (e->type()) {
//    case QEvent::LanguageChange:
//      ui->retranslateUi(this);
//      break;
//    default:
//      break;
//    }
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
    m3dView->setProjectItem( nullptr );
    //At now delete project itself
    delete mProject;
    }
  mProject = nullptr;
  }




//Fill visual table with mHeaderList contents
void SdDGetObject::fillTable()
  {
  disconnect( mFieldsBox, &QTableWidget::cellChanged, this, &SdDGetObject::onFieldChanged );

  //Accumulate used fields
  QSet<QString> fields;
  for( SdLibraryHeader &hdr : mHeaderList ) {
    for( auto iter = hdr.mParamTable.cbegin(); iter != hdr.mParamTable.cend(); iter++ )
      fields.insert( iter.key() );
    }

  //Leave in field list only fields with value
  //int row = mFieldsBox->rowCount();
  for( int i = 0; i < mFieldsBox->rowCount(); )
    if( mFieldsBox->item( i, 2 )->text().isEmpty() && !fields.contains(mFieldsBox->item( i, 1 )->text()) )
      mFieldsBox->removeRow(i);
    else {
      //Remove from accumulated fields fields with value which already in table
      fields.remove( mFieldsBox->item( i, 1 )->text() );
      i++;
      }

  //Leaved fields append from set to visual table
  for( auto iter = fields.cbegin(); iter != fields.cend(); iter++ ) {
    int row = mFieldsBox->rowCount();
    //Append new field row
    mFieldsBox->insertRow( row );
    mFieldsBox->setRowHeight( row, 20 );
    //Setup field params
    //Field visibility
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
    item->setCheckState( Qt::Checked );
    mFieldsBox->setItem( row, 0, item );
    //Field name
    mFieldsBox->setItem( row, 1, new QTableWidgetItem( *iter ) );
    //Disable edit
    mFieldsBox->item( row, 1 )->setFlags( Qt::ItemIsEnabled );
    //Field mask
    mFieldsBox->setItem( row, 2, new QTableWidgetItem() );

    //Test if field in field width map
    if( !sdFieldWidth.contains( *iter ) ) {
      //Append default field width
      sdFieldWidth.insert( *iter, 100 );
      }
    }

  //Read current fields width
  for( int i = 0; i < mTable->columnCount(); i++ )
    sdFieldWidth.insert( mTable->horizontalHeaderItem(i)->text(), mTable->columnWidth(i) );


  mTable->clear();
  mTable->setColumnCount( mFieldsBox->rowCount() + 3);
  //Fill column header
  //First three columns - are name, author and time of creation
  mTable->setHorizontalHeaderItem( 0, new QTableWidgetItem( tr("Name") ) );
  mTable->setHorizontalHeaderItem( 1, new QTableWidgetItem( tr("Author") ) );
  mTable->setHorizontalHeaderItem( 2, new QTableWidgetItem( tr("Created") ) );
  //Other columns - fields
  for( int i = 0; i < mFieldsBox->rowCount(); i++ ) {
    mTable->setHorizontalHeaderItem( i+3, new QTableWidgetItem( mFieldsBox->item(i,1)->text() ) );
    if( mFieldsBox->item( i, 0 )->checkState() != Qt::Checked )
      mTable->hideColumn( i + 3 );
    }

  //Setup column width
  for( int i = 0; i < mTable->columnCount(); i++ ) {
    QString name = mTable->horizontalHeaderItem(i)->text();
    mTable->setColumnWidth( i, sdFieldWidth.value(name, 100) );
    }

  //Fill table
  mTable->setRowCount( mHeaderList.count() );
  int row = 0;
  for( SdLibraryHeader &hdr : mHeaderList ) {
    mTable->setRowHeight( row, 20 );
    mTable->setItem( row, 0, new QTableWidgetItem(hdr.mName) );
    mTable->setItem( row, 1, new QTableWidgetItem(hdr.mAuthor) );
    mTable->setItem( row, 2, new QTableWidgetItem(SvTime2x::toLocalString(hdr.mTime)) );
    //Fill fields
    for( int i = 3; i < mTable->columnCount(); i++ ) {
      QString name = mTable->horizontalHeaderItem(i)->text();
      mTable->setItem( row, i, new QTableWidgetItem(hdr.mParamTable.value(name)) );
      }
    row++;
    }

  clearComponent();
  mSymbolView->setItem(nullptr,true);
  mPartView->setItem(nullptr,true);
  m3dView->setProjectItem(nullptr);
  mSections->clear();
  mSectionIndex = -1;

  connect( mFieldsBox, &QTableWidget::cellChanged, this, &SdDGetObject::onFieldChanged );
  }








void SdDGetObject::accept()
  {
  //Get current row
  int row = mTable->currentRow();
  if( row >= 0 && row < mHeaderList.count() ) {
    //Extract name, author and id
    SdLibraryHeader hdr = mHeaderList.at(row);
    mObjName   = hdr.mName;
    mObjUid     = hdr.uid();
    if( hdr.mClass == dctComponent )
      mCompUid = mObjUid;
    else
      mCompUid.clear();
    QDialog::accept();
    }
  else QMessageBox::warning( this, tr("Error"), tr("You must select element or press Cancel") );
  }






SdObject *SdDGetObject::getObject(quint64 sort, const QString title, QWidget *parent, const QString defFiltr)
  {
  return SdLibraryStorage::instance()->cfObjectGet( getObjectUid( sort, title, parent, defFiltr) );
  }




QString SdDGetObject::getObjectUid(quint64 sort, const QString title, QWidget *parent, const QString defFiltr)
  {
  SdDGetObject dget( sort, title, defFiltr, parent );
  if( !defFiltr.isEmpty() )
    dget.find();
  if( dget.exec() )
    return mObjUid;
  return QString();
  }




SdPItemComponent *SdDGetObject::getComponent(int *logSectionPtr, SdStringMap *param, const QString title, QWidget *parent)
  {
  SdDGetObject dget( dctComponent, title, QString{}, parent );
  if( dget.exec() ) {
    //If available pointer to logical section then set selected section
    if( logSectionPtr )
      *logSectionPtr = mSectionIndex;
    //If available pointer to param then set component or instance params
    if( param )
      *param = mParam;
    return sdObjectOnly<SdPItemComponent>( SdLibraryStorage::instance()->cfObjectGet( mCompUid ) );
    }
  return nullptr;
  }

