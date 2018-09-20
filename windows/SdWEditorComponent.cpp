/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Visual Editor for component
*/
#include "SdWEditorComponent.h"
#include "SdWEditorGraphSymbol.h"
#include "SdWEditorGraphPart.h"
#include "SdDGetObject.h"
#include "SdWCommand.h"
#include "SdDParamEditor.h"
#include "SdDParamDefault.h"
#include "objects/SdProject.h"
#include "objects/SdPartVariant.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdGraphPartPin.h"

#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include <QTimer>
#include <QDebug>

SdWEditorComponent::SdWEditorComponent(SdPItemComponent *comp, QWidget *parent) :
  SdWEditor( parent ),
  mComponent(comp)
  {
  mUndo = mComponent->getUndo();

  // symbol preview         packing info
  //                    (sections, pin pack, buttons)
  // part preview       part name and selection
  //                        param editor
  //                      (param table, buttons)
  QHBoxLayout *root = new QHBoxLayout();
  //QSplitter *split = new QSplitter();

    //Left column contains preview widgets
    QVBoxLayout *lay = new QVBoxLayout();
      //Symbol
      QGroupBox *preview = new QGroupBox( tr("Symbol preview") );
        QHBoxLayout *hbox = new QHBoxLayout();
        preview->setLayout( hbox );
        hbox->addWidget( mSymbolViewer = new SdWEditorGraphView( nullptr, preview ) );
      lay->addWidget( preview );

      //Part
      preview = new QGroupBox( tr("Part preview") );
        hbox = new QHBoxLayout();
        preview->setLayout( hbox );
        hbox->addWidget( mPartViewer = new SdWEditorGraphView( nullptr, preview ) );
      lay->addWidget( preview );
    root->addLayout( lay );

    //Right column contains edit widgets
    lay = new QVBoxLayout();

      //Row with sections, pack, and buttons
      hbox = new QHBoxLayout();
        //Section title and sections list
        QVBoxLayout *vbox = new QVBoxLayout();
          vbox->addWidget( new QLabel(tr("Sections list")) );
          vbox->addWidget( mSectionList = new QListWidget() );
        hbox->addLayout( vbox );

        //Pin pack info
        vbox = new QVBoxLayout();
          vbox->addWidget( new QLabel(tr("Pin pack info")) );
          vbox->addWidget( mPackTable = new QTableWidget() );
        hbox->addLayout( vbox );

        //Buttons
        vbox = new QVBoxLayout();
          vbox->addWidget( mSectionAdd = new QPushButton( tr("Add section") )  );
          vbox->addWidget( mSectionDubl = new QPushButton( tr("Dubl section") ) );
          vbox->addWidget( mSectionSymbol = new QPushButton( tr("Select symbol") ) );
          vbox->addWidget( mSectionDelete = new QPushButton( tr("Delete section") ) );
          vbox->addWidget( mSectionDeleteAll = new QPushButton( tr("Delete all sections") ) );
        hbox->addLayout( vbox );
      lay->addLayout( hbox );

      //Row with part
      hbox = new QHBoxLayout();
        hbox->addWidget( new QLabel(tr("Part:")) );
        hbox->addWidget( mPart = new QLineEdit() );
        hbox->addWidget( mPartSelect = new QPushButton( tr("Select part")) );
      lay->addLayout( hbox );

      //Param table, buttons
      hbox = new QHBoxLayout();
        //Param table
        vbox = new QVBoxLayout();
          vbox->addWidget( new QLabel(tr("Param table")) );
          vbox->addWidget( mParamTable = new QTableWidget() );
        hbox->addLayout( vbox );

        //Buttons
        vbox = new QVBoxLayout();
          vbox->addWidget( mParamAdd = new QPushButton( tr("Add param")) );
          vbox->addWidget( mParamAddDefault = new QPushButton( tr("Add defaults")) );
          vbox->addWidget( mParamCopy = new QPushButton( tr("Copy param")) );
          vbox->addWidget( mParamDelete = new QPushButton( tr("Delete param")) );
        hbox->addLayout( vbox );
      lay->addLayout( hbox );

    root->addLayout( lay );

  setLayout( root );
  connect( mSectionList, &QListWidget::currentRowChanged, this, &SdWEditorComponent::onCurrentSection );
  if( mComponent->isEditEnable() ) {
    //Connect signals when edit enabled
    connect( mSectionAdd, &QPushButton::clicked, this, &SdWEditorComponent::sectionAdd );
    connect( mSectionDubl, &QPushButton::clicked, this, &SdWEditorComponent::sectionDubl );
    connect( mSectionSymbol, &QPushButton::clicked, this, &SdWEditorComponent::sectionSelect );
    connect( mSectionDelete, &QPushButton::clicked, this, &SdWEditorComponent::sectionDelete );
    connect( mSectionDeleteAll, &QPushButton::clicked, this, &SdWEditorComponent::sectionDeleteAll );

    connect( mPartSelect, &QPushButton::clicked, this, &SdWEditorComponent::partSelect );

    connect( mParamAdd, &QPushButton::clicked, this, &SdWEditorComponent::paramAdd );
    connect( mParamAddDefault, &QPushButton::clicked, this, &SdWEditorComponent::paramAddDefault );
    connect( mParamDelete, &QPushButton::clicked, this, &SdWEditorComponent::paramDelete );
    connect( mParamCopy, &QPushButton::clicked, this, &SdWEditorComponent::paramCopy );
    }
  else {
    //Disable buttons when edit disabled
    mSectionAdd->setEnabled(false);
    mSectionDubl->setEnabled(false);
    mSectionSymbol->setEnabled(false);
    mSectionDelete->setEnabled(false);
    mSectionDeleteAll->setEnabled(false);

    mPartSelect->setEnabled(false);

    mParamAdd->setEnabled(false);
    mParamAddDefault->setEnabled(false);
    mParamDelete->setEnabled(false);
    mParamCopy->setEnabled(false);
    }

  QTimer::singleShot( 300, this, [this]() {
    fillSections();
    fillPart();
    fillParams();
    });
  }




SdProjectItem *SdWEditorComponent::getProjectItem() const
  {
  return mComponent;
  }




void SdWEditorComponent::onActivateEditor()
  {
  SdWEditor::onActivateEditor();

  //Activate menu
  SdWCommand::cmObjectEditEnable->setVisible( !mComponent->isEditEnable() );
  SdWCommand::cmObjectEditDisable->setVisible( mComponent->isEditEnable() );

  //Update info from component
//  fillSections();
//  fillPart();
  }




void SdWEditorComponent::sectionAdd()
  {
  //Query from user symbol for section
  QString uid = SdDGetObject::getObjectUid( dctSymbol, tr("Select symbol for section"), this );
  //If user selected symbol then append it as section
  if( !uid.isEmpty() ) {
    //Symbol selected
    mUndo->begin( tr("Append section for component"), mComponent );
    mComponent->appendSection( uid, mUndo );
    dirtyProject();
    fillSections();
    mSectionList->setCurrentRow(mSectionList->count() - 1);
    onCurrentSection(mSectionList->count() - 1);
    }
  }




void SdWEditorComponent::sectionDubl()
  {
  int row = mSectionList->currentRow();
  if( row >= 0 ) {
    mUndo->begin( tr("Duplicate section for component"), mComponent );
    mComponent->appendSection( mComponent->getSectionSymbolId(row), mUndo );
    dirtyProject();
    fillSections();
    mSectionList->setCurrentRow(row+1);
    onCurrentSection(row+1);
    }
  }





void SdWEditorComponent::sectionSelect()
  {
  int row = mSectionList->currentRow();
  if( row >= 0 ) {
    mUndo->begin( tr("Set section for component"), mComponent );
    mComponent->setSectionSymbolId( SdDGetObject::getObjectUid( dctSymbol, tr("Select symbol for section"), this ),
                                    row, mUndo );
    dirtyProject();
    fillSections();
    mSectionList->setCurrentRow(row);
    onCurrentSection(row);
    }
  }




void SdWEditorComponent::sectionDelete()
  {
  mUndo->begin( tr("Delete component section"), mComponent );
  int row = mSectionList->currentRow();
  if( row >= 0 ) {
    if( QMessageBox::question( this, tr("Attention!"), tr("You attempting to delete section %1. Delete?").arg(row+1) ) == QMessageBox::Yes ) {
      mComponent->removeSection( row, mUndo );
      dirtyProject();
      fillSections();
      }
    }
  }




void SdWEditorComponent::sectionDeleteAll()
  {
  if( QMessageBox::question( this, tr("Attention!"), tr("Are You sure delete All sections?") ) == QMessageBox::Yes ) {
    mUndo->begin( tr("Delete all sections"), mComponent );
    int count = mComponent->getSectionCount() - 1;
    while( count >= 0 )
      mComponent->removeSection( count--, mUndo );
    dirtyProject();
    fillSections();
    }
  }




void SdWEditorComponent::onCurrentSection(int index)
  {
  disconnect( mPackTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onPackChanged );
  //Update graph preview
  mSymbolViewer->setItemById( mComponent->getSectionSymbolId(index) );
  //Update pins table
  SdPinAssociation pins = mComponent->getSectionPins(index);
  mPackTable->clear();
  mPackTable->setColumnCount(2);
  mPackTable->setRowCount(pins.count());
  //Setup header
  QStringList header;
  header << tr("Pin name") << tr("Pin number");
  mPackTable->setHorizontalHeaderLabels( header );
  int row = 0;
  for( auto iter = pins.constBegin(); iter != pins.constEnd(); iter++ ) {
    QTableWidgetItem *item;

    //Item cell with pin name
    mPackTable->setItem( row, 0, item = new QTableWidgetItem(iter.key()) );
    //Pin name disable for editing
    item->setFlags(Qt::ItemIsEnabled);

    //Item cell with pin number
    mPackTable->setItem( row, 1, item = new QTableWidgetItem(iter.value()) );
    //If componen editing disabled then disable edit item
    if( !mComponent->isEditEnable() )
      item->setFlags(Qt::ItemIsEnabled);
    //pin number cell background indicate errors in packing
    if( !mPackNumbers.contains(iter.value()) ) {
      item->setBackgroundColor( QColor(Qt::red).lighter() );
      item->setToolTip( tr("No pin with this number in part") );
      }
    else if( mPackNumbers.value(iter.value()) != packetPin(index,iter.key()) ) {
      item->setBackgroundColor( QColor(Qt::yellow).lighter() );
      item->setToolTip( tr("Duplicate pin number assignment") );
      }
    mPackTable->setRowHeight( row, 20 );
    row++;
    }
  if( mComponent->isEditEnable() )
    connect( mPackTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onPackChanged );
  }




void SdWEditorComponent::onPackChanged(int row, int column)
  {
  disconnect( mPackTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onPackChanged );
  int section = mSectionList->currentRow();
  QString number = mPackTable->item(row,column)->text();
  QString name = mPackTable->item(row,0)->text();
  if( name.isEmpty() ) return;
  mUndo->begin( tr("Edit pin pack"), mComponent );
  if( !number.isEmpty() ) {
    if( !mPackNumbers.contains(number) ) {
      QMessageBox::warning( this, tr("Error"), tr("Component part has no pin with this number") );
      mPackTable->item(row,column)->setBackgroundColor( QColor(Qt::red).lighter() );
      mPackTable->item(row,column)->setToolTip( tr("No pin with this number in part") );
      }
    else if( !mPackNumbers.value(number).isEmpty() && mPackNumbers.value(number) != packetPin(section,name) ) {
      QMessageBox::warning( this, tr("Error"), tr("This number always assigned to %1").arg(mPackNumbers.value(number)) );
      mPackTable->item(row,column)->setBackgroundColor( QColor(Qt::yellow).lighter() );
      mPackTable->item(row,column)->setToolTip( tr("Duplicate pin number assignition") );
      }
    else {
      //Correct assignment
      mPackTable->setItem(row,column, new QTableWidgetItem(number) );
      mPackNumbers.insert( number, packetPin(section,name) );
      }
    mComponent->setSectionPinNumber( section, name, number, mUndo );
    }
  else {
    //Pin was cleared
    QString previousNumber = mComponent->getSectionPinNumber( section, name );
    if( mPackNumbers.value(previousNumber) == packetPin(section,name) )
      mPackNumbers.insert(previousNumber,QString());
    mComponent->setSectionPinNumber( section, name, number, mUndo );
    mPackTable->setItem(row,column, new QTableWidgetItem(number) );
    }
  connect( mPackTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onPackChanged );
  updateUndoRedoStatus();
  dirtyProject();
  }






void SdWEditorComponent::onParamChanged(int row, int column)
  {
  Q_UNUSED(column)
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onParamChanged );
  QString key = mParamTable->item(row,0)->text();
  QString value = mParamTable->item(row,1)->text();
  //qDebug() << Q_FUNC_INFO << key <<value;
  mComponent->paramSet( key, value, mUndo );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onParamChanged );
  updateUndoRedoStatus();
  dirtyProject();
  }




void SdWEditorComponent::partSelect()
  {
  //Get part uid from user
  QString uid = SdDGetObject::getObjectUid( dctPart, tr("Select part for component"), this );
  if( !uid.isEmpty() ) {
    //If user selected part then assign it
    mUndo->begin( tr("Part select for component"), mComponent );
    SdPartVariant *part = mComponent->getPart();
    if( part == nullptr ) {
      part = new SdPartVariant();
      mComponent->insertChild( part, mUndo );
      }
    part->setPartId( uid, mUndo );
    dirtyProject();
    fillPart();
    }
  }




void SdWEditorComponent::paramAdd()
  {
  QString key = QInputDialog::getText( this, tr("Param name"), tr("Enter param name") );
  if( !key.isEmpty() ) {
    if( mComponent->paramContains(key) )
      QMessageBox::warning( this, tr("Warning!"), tr("Param with this name already exist. Enter another name.") );
    else
      paramAddInt( key );
    }
  }



//Append default param group
void SdWEditorComponent::paramAddDefault()
  {
  SdDParamDefault def( this );
  if( def.exec() ) {
    QStringList paramList = def.defParamList();
    for( const QString &param : paramList )
      if( !mComponent->paramContains(param) )
        paramAddInt( param, SdDParamEditor::defParamValue(param, mComponent, this) );
    }
  }





void SdWEditorComponent::paramDelete()
  {
  int paramIndex = mParamTable->currentRow();
  if( paramIndex >= 0 ) {
    QString key = mParamTable->item( paramIndex, 0 )->text();
    mComponent->paramDelete( key, mUndo );
    mParamTable->removeRow( paramIndex );
    dirtyProject();
    }
  else
    QMessageBox::warning( this, tr("Warning!"), tr("Select param to delete") );
  }





//Copy params from another component
void SdWEditorComponent::paramCopy()
  {
  SdStringMap param; //Params to get in
  SdPItemComponent *comp = SdDGetObject::getComponent( nullptr, &param, tr("Select component to copy param from"), this );
  if( comp != nullptr ) {
    //Append params
    for( auto iter = param.cbegin(); iter != param.cend(); iter++ )
      mComponent->paramSet( iter.key(), iter.value(), mUndo );
    dirtyProject();
    fillParams();
    }
  }









void SdWEditorComponent::fillSections()
  {
  //Remove previous section list
  mSectionList->clear();

  //Fill with actual sections
  int count = mComponent->getSectionCount();
  for( int i = 0; i < count; i++ )
    mSectionList->addItem( mComponent->getSectionSymbolTitle(i) );

  //Enable-disable buttons on editing enable
  if( mComponent->isEditEnable() ) {
    mSectionDelete->setEnabled( count );
    mSectionDeleteAll->setEnabled( count );
    mSectionSymbol->setEnabled( count );
    }

  //Fill used pin numbers
  fillUsedPins();

  //Sections present, select first one
  mSectionList->setCurrentRow(0);
  onCurrentSection(0);
  }






void SdWEditorComponent::fillPart()
  {
  mPartViewer->setItemById( mComponent->getPartId() );  
  mPart->setText( mComponent->getPartTitle() );
  //Fill available pin numbers
  mPackNumbers.clear();
  mDuplicateNumbers.clear();
  SdPItemPart *part = dynamic_cast<SdPItemPart*>( mPartViewer->getProjectItem() );
  if( part != nullptr ) {
    part->forEachConst( dctPartPin, [this] (SdObject *obj) ->bool {
      SdGraphPartPin *pin = dynamic_cast<SdGraphPartPin*>(obj);
      Q_ASSERT( pin != nullptr );
      if( mPackNumbers.contains( pin->getPinNumber() ) )
        mDuplicateNumbers.insert( pin->getPinNumber() );
      else
        mPackNumbers.insert( pin->getPinNumber(), QString() );
      return true;
      });
    }

  fillUsedPins();
  onCurrentSection( mSectionList->currentRow() );
  }




void SdWEditorComponent::fillParams()
  {
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onParamChanged );
  mParamTable->clear();
  SdStringMap tab = mComponent->paramTable();
  mParamTable->setColumnCount(2);
  mParamTable->setRowCount( tab.count() );
  //Setup header
  mParamTable->setHorizontalHeaderLabels( {tr("Param name"), tr("Param value")} );
  mParamTable->setColumnWidth(1,400);
  int row = 0;
  for( auto iter = tab.constBegin(); iter != tab.constEnd(); iter++ )
    paramAppend( row++, iter.key(), iter.value() );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onParamChanged );
  }




void SdWEditorComponent::fillUsedPins()
  {
  for( QMap<QString,QString>::iterator iter = mPackNumbers.begin(); iter != mPackNumbers.end(); iter++ )
    iter.value() = QString();
  int sectionCount = mComponent->getSectionCount();
  for( int section = 0; section < sectionCount; section++ ) {
    SdStringMap pack = mComponent->getSectionPins(section);
    for( auto iter = pack.constBegin(); iter != pack.constEnd(); iter++ ) {
      if( mPackNumbers.contains(iter.value()) && mPackNumbers.value(iter.value()).isEmpty() )
        mPackNumbers.insert( iter.value(), packetPin(section,iter.key()) );
      }
    }
  }




void SdWEditorComponent::paramAppend( int row, const QString key, const QString value)
  {
  mParamTable->setRowHeight( row, 25 );
  QTableWidgetItem *item = new QTableWidgetItem(key);
  mParamTable->setItem( row, 0, item );
  item->setToolTip( SdDParamEditor::defParamDescription(key) );
  item->setFlags( Qt::ItemIsEnabled );
  mParamTable->setItem( row, 1, new QTableWidgetItem(value) );
  }




void SdWEditorComponent::paramAddInt(const QString key, const QString value)
  {
  mComponent->paramSet( key, value, mUndo );
  int row = mParamTable->rowCount();
  mParamTable->insertRow( row );
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onParamChanged );
  paramAppend( row, key, value );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorComponent::onParamChanged );
  dirtyProject();
  }




QString SdWEditorComponent::packetPin(int section, const QString pinName)
  {
  return QString("%1:%2").arg(section+1).arg(pinName);
  }






void SdWEditorComponent::cmEditUndo()
  {
  //Refill all visual elements
  int currSection = mSectionList->currentRow();
  int currPin = mPackTable->currentRow();
  int currParam = mParamTable->currentRow();
  fillSections();
  fillPart();
  fillParams();
  mSectionList->setCurrentRow(currSection);
  mPackTable->setCurrentCell(currPin,1);
  mParamTable->setCurrentCell(currParam,1);
  dirtyProject();
  }




void SdWEditorComponent::cmEditRedo()
  {
  cmEditUndo();
  }
