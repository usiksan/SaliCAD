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
#include "SdWEditorGraphView.h"
#include "SdWEditor3dPartView.h"
#include "SdDRowValue.h"
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
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QMap>
#include <QSet>
#include <QTimer>
#include <QSettings>
#include <QDataStream>
#include <QDebug>



//Fields width
static QMap<QString,int> sdFieldWidth;


bool sdFieldMapMatch( const QString src, const SdStringMap &map ) {
  for( auto iter = map.cbegin(); iter != map.cend(); iter++ )
    if( iter.value().indexOf( src, 0, Qt::CaseInsensitive ) >= 0 ) return true;
  return false;
  }


QString                SdDGetObject::mObjName;      //Object name
QString                SdDGetObject::mItemName;
QString                SdDGetObject::mObjUid;       //Unical object id
QString                SdDGetObject::mCompUid;      //Unical object id for components
int                    SdDGetObject::mSectionIndex; //Section index
SdStringMap            SdDGetObject::mParam;        //Component or instance params

quint64                SdDGetObject::mSort;         //Object select sort (class)
SdLibraryHeaderList    SdDGetObject::mHeaderList;   //Header list for filtered objects
bool                   SdDGetObject::mExpandVariant = false;    //Flag for find only in titles and not show variants
bool                   SdDGetObject::mWithPartOnly;

SdDGetObject::SdDGetObject(quint64 sort, const QString title, const QString &defFilter, QWidget *parent) :
  QDialog(parent),
  mComponent(nullptr),
  mProject(nullptr)
  {
  setWindowTitle( title );


  //Build dialog
  QVBoxLayout *centralLayout = new QVBoxLayout();
  setLayout( centralLayout );

  mSplitFinder = new QSplitter(Qt::Vertical);
  centralLayout->addWidget( mSplitFinder );

  mDialogButtons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
  centralLayout->addWidget( mDialogButtons, 0, Qt::AlignRight );

  //Find results table
  QPushButton *but;
  QWidget *finder = new QWidget();
  QVBoxLayout *vbox = new QVBoxLayout();
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget( new QLabel( tr("Enter object to find:") )  );
  hbox->addWidget( mNameFilter = new QLineEdit() );
  hbox->addWidget( but = new QPushButton( tr("Find") ) );
  connect( but, &QPushButton::clicked, this, &SdDGetObject::find );
  but->setDefault(true);
  if( sort & dctComponent ) {
    QCheckBox *cb = new QCheckBox( tr("With part only") );
    hbox->addWidget( cb );
    cb->setChecked( mWithPartOnly );
    connect( cb, &QCheckBox::clicked, this, [this] ( bool checked ) {
      mWithPartOnly = checked;
      find();
      } );
    }
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
    //qDebug() << "Restore GetObject";
    QByteArray ar = s.value( QStringLiteral(SDK_GET_OBJECT_DLG_CFG) ).toByteArray();
    QDataStream is(ar);
    QSize dlgSize;
    QString nameFilter;
    QByteArray findSplitter;
    QByteArray viewSplitter;

    is >> dlgSize >> nameFilter >> findSplitter >> viewSplitter;

    resize( dlgSize );

    //Preset filter
    if( defFilter.isEmpty() )
      mNameFilter->setText( nameFilter );
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
    QTimer::singleShot( 300, this, &SdDGetObject::find );
    }


  connect( this, &QDialog::finished, this, [this] ( int ) {
    //qDebug() << "Store GetObject";
    //Store current settings
    QByteArray ar;
    QDataStream os(&ar, QIODevice::WriteOnly );
    QSize dlgSize( size() );
    QString nameFilter( mNameFilter->text() );
    QByteArray findSplitter( mSplitFinder->saveState() );
    QByteArray viewSplitter( mSplitView->saveState() );

    os << dlgSize << nameFilter << findSplitter << viewSplitter;

    QSettings s;
    s.setValue( QStringLiteral(SDK_GET_OBJECT_DLG_CFG), ar );

    //Store width of column find result table
    for( int i = 0; i < mTable->columnCount(); i++ )
      sdFieldWidth.insert( mTable->horizontalHeaderItem(i)->text(), mTable->columnWidth(i) );
    });
  }






void SdDGetObject::find()
  {
  auto sdNameFilter = mNameFilter->text();
  QStringList list = sdNameFilter.split( QRegularExpression("\\s+"), Qt::SkipEmptyParts );
  mHeaderList.clear();

  //Accumulate headers matched to filter
  SdLibraryStorage::instance()->forEachHeader( [list] ( SdLibraryHeader &hdr ) -> bool {
    //test class
    if( hdr.mClass & mSort ) {
      //Skip component or symbol without part
      if( mWithPartOnly && !hdr.mPartPresent )
        return false;

      //Split uid to name type and author
      QString uid( hdr.uid() );
      QString name = hdr.mName; // uid.contains(sdUidDelimiter) ? uid.split( sdUidDelimiter ).at(1) : uid;

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
          if( matched )
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
        if( matched )
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

  mSymbolView->setItem( nullptr, true );
  mPartView->setItem( nullptr, true );

  SdLibraryHeader hdr = mHeaderList.at(row);
  if( hdr.mClass == dctSheet ) {
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
  else if( hdr.mClass == dctSymbol ) {
    mComponent = sdObjectOnly<SdPItemSymbol>( SdLibraryStorage::instance()->cfObjectGet( hdr.uid() ) );
    }

  mParam = hdr.mParamTable;

  mSections->clear();
  mSectionIndex = -1;

  bool present = SdLibraryStorage::instance()->cfObjectContains( hdr.uid() );

  if( mComponent ) {
    //Get section count
    int sectionCount = mComponent->sectionCount();
    //If sections present, then fill visual list with sections
    if( sectionCount ) {
      for( int i = 0; i < sectionCount; i++ ) {
        mSections->addItem( tr("Section %1: %2").arg(i+1).arg(mComponent->sectionSymbolTitleGet(i)) );
        //If any section not present in library then set present to false
        if( !SdLibraryStorage::instance()->cfObjectContains(mComponent->sectionSymbolIdGet(i)) )
          present = false;
        }
      mSections->setCurrentRow(0);
      mSectionIndex = 0;
      mSymbolView->setItemById( mComponent->sectionSymbolIdGet(0) );
      }
    // else {
    //   mSymbolView->setItem( mComponent, true );
    //   }

    //Setup part for view
    mPartView->setItemById( mComponent->partIdGet() );
    if( !SdLibraryStorage::instance()->cfObjectContains( mComponent->partIdGet() ) )
      present = false;
    }

  if( mProject ) {
    mSections->clear();
    mProject->forEachConst( dctSheet, [this] (SdObject *obj) -> bool {
      SdPtrConst<SdPItemSheet> sheet(obj);
      if( sheet.isValid() )
        mSections->addItem( sheet->getTitle() );
      return true;
      });
    mSections->setCurrentRow(0);
    onCurrentSection(0);
    present = true;
    }

  m3dView->setProjectItem( mPartView->getProjectItem() );

  mDialogButtons->button(QDialogButtonBox::Ok)->setEnabled( present );
  }




//On change component section selection
void SdDGetObject::onCurrentSection(int row)
  {
  if( row < 0 )
    return;
  if( mComponent != nullptr ) {
    mSymbolView->setItemById( mComponent->sectionSymbolIdGet(row) );
    mSectionIndex = row;
    }
  else if( mProject != nullptr ) {
    mItemName = mSections->currentItem()->text();
    mSymbolView->setItem( mProject->getSheet( mItemName ), false );
    mPartView->setItem( mProject->getPlate( mItemName ), false );
    }
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

  //Accumulate used fields
  QSet<QString> fields;
  for( SdLibraryHeader &hdr : mHeaderList ) {
    for( auto iter = hdr.mParamTable.cbegin(); iter != hdr.mParamTable.cend(); iter++ )
      fields.insert( iter.key() );
    }

  QStringList fieldsList;

  //Leaved fields append from set to visual table
  for( auto iter = fields.cbegin(); iter != fields.cend(); iter++ ) {
    fieldsList.append( *iter );

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
  mTable->setColumnCount( fieldsList.count() + 3 );
  //Fill column header
  //First three columns - are name, author and time of creation
  mTable->setHorizontalHeaderItem( 0, new QTableWidgetItem( tr("Name") ) );
  mTable->setHorizontalHeaderItem( 1, new QTableWidgetItem( tr("Author") ) );
  mTable->setHorizontalHeaderItem( 2, new QTableWidgetItem( tr("Created") ) );
  //Other columns - fields
  for( int i = 0; i < fieldsList.count(); i++ ) {
    mTable->setHorizontalHeaderItem( i+3, new QTableWidgetItem( fieldsList.at(i) ) );
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
    if( hdr.mClass == dctComponent || hdr.mClass == dctSymbol )
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




SdPItemVariant *SdDGetObject::getComponent(int *logSectionPtr, SdStringMap *param, const QString title, QWidget *parent)
  {
  SdDGetObject dget( dctComponent | dctSymbol, title, QString{}, parent );
  if( dget.exec() ) {
    //If available pointer to logical section then set selected section
    if( logSectionPtr )
      *logSectionPtr = mSectionIndex;
    //If available pointer to param then set component or instance params
    if( param )
      *param = mParam;
    return sdObjectOnly<SdPItemVariant>( SdLibraryStorage::instance()->cfObjectGet( mCompUid ) );
    }
  return nullptr;
  }




SdProject *SdDGetObject::getProject(QString &itemName, const QString title, QWidget *parent, const QString defFiltr)
  {
  SdDGetObject dget( dctProject, title, defFiltr, parent );
  if( !defFiltr.isEmpty() )
    dget.find();
  if( dget.exec() ) {
    itemName = mItemName;
    return sdObjectOnly<SdProject>( SdLibraryStorage::instance()->cfObjectGet(mObjUid) );
    }
  return nullptr;
  }


