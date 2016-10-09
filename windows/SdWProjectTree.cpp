/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdWProjectTree.h"
#include "objects/SdPulsar.h"
#include "windows/SdPNewProjectItem_SelectType.h"
#include "windows/SdPNewProjectItem_EnterName.h"
#include <QFileInfo>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QWizard>

SdWProjectTree::SdWProjectTree(const QString fname, QWidget *parent) :
  QTreeWidget(parent),
  mFileName(fname)
  {
  if( fname.isEmpty() ) {
    //Создаем пустой проект
    mFileName = SD_DEFAULT_FILE_NAME;
    mProject = new SdProject();
    }
  else {
    //Загружаем проект из файла
    mProject = SdProject::load( fname );
    }
  //Если проект построен, то отобразить содержимое
  if( mProject ) {
    setHeaderHidden(true);
    setRootIsDecorated(true);
    setItemsExpandable(true);
    setLineWidth(1);

    //Заполнить список корневого уровня
    mSymbolList = createItem( tr("Symbols"), tr("Project symbols list"), tr("Contains project symbols list") );   //!< Схемные изображения компонентов
    mPartList   = createItem( tr("Parts"), tr("Project parts list"), tr("Contains project parts list") );     //!< Корпуса компонентов
    mAliasList  = createItem( tr("Components"), tr("Project components list"), tr("Contains project components list, each of them is agregation symbols width appropriate part") );    //!< Компоненты (схемное избр. + корпус + параметры)
    mSheetList  = createItem( tr("Sheets"), tr("Project shematic sheets"), tr("Contains project shematic sheet list") );    //!< Листы схем
    mPlateList  = createItem( tr("Construct"), tr("Project constructs and pcb list"), tr("Contains project construct and pcb list") );    //!< Печатные платы
    mTextList   = createItem( tr("Other docs"), tr("Project text and table docs list"), tr("Contains project text, table and others docs") );     //!< Текстовые документы

    addTopLevelItem( mSheetList );
    addTopLevelItem( mPlateList );
    addTopLevelItem( mAliasList );
    addTopLevelItem( mSymbolList );
    addTopLevelItem( mPartList );
    addTopLevelItem( mTextList );
    }

  connect( SdPulsar::pulsar, &SdPulsar::insertItem, this, &SdWProjectTree::insertItem );
  connect( SdPulsar::pulsar, &SdPulsar::removeItem, this, &SdWProjectTree::removeItem );
  connect( SdPulsar::pulsar, &SdPulsar::renameItem, this, &SdWProjectTree::renameItem );
  }




SdWProjectTree::~SdWProjectTree()
  {
  if( mProject )
    delete mProject;
  }




QString SdWProjectTree::fileName()
  {
  if( mFileName == SD_DEFAULT_FILE_NAME )
    return mFileName;
  return QFileInfo(mFileName).completeBaseName();
  }




bool SdWProjectTree::isProjectValid() const
  {
  return mProject != 0;
  }




bool SdWProjectTree::cmFileClose() {
  //Test if file saved
  if( mProject->isDirty() ) {
    int res = QMessageBox::question( this, tr("Warning!"),
                                     tr("Project \"%1\" changed! Save it?"),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
    if( res == QMessageBox::No ) return true;
    if( res == QMessageBox::Yes ) return cmFileSave();
    return false;
    }
  return true;
  }




bool SdWProjectTree::cmFileSave()
  {
  //Test if file name default
  if( mFileName == SD_DEFAULT_FILE_NAME )
    return cmFileSaveAs();
  return mProject->save( mFileName );
  }




bool SdWProjectTree::cmFileSaveAs()
  {
  QString title = QFileDialog::getSaveFileName(this, tr("Save project file"), QString(), tr("SaliCAD 3D Files (*%1)").arg(SD_BASE_EXTENSION) );

  if( title.isEmpty() ) return false;

  mFileName = title;
  if( !mFileName.endsWith(SD_BASE_EXTENSION) )
    mFileName.append( SD_BASE_EXTENSION );

  SdPulsar::pulsar->emitRenameProject( mProject );

  return cmFileSave();
  }




void SdWProjectTree::cmObjectNew()
  {
  SdProjectItemPtr item = 0;
  QWizard wizard(this);
  wizard.setPage( 0, new SdPNewProjectItem_SelectType( &item, mProject, &wizard) );
  wizard.setPage( 1, new SdPNewProjectItem_EnterName( &item, mProject, &wizard) );

  if( wizard.exec() ) {
    //Append item to the project
    mProject->insert( item );
    }
  }

void SdWProjectTree::renameItem(SdProjectItem *item)
  {
  if( item && item->getProject() == mProject ) {
    //Item from this project
    item->mTreeItem->setText( 0, item->getTitle() );
    }
  }

void SdWProjectTree::insertItem(SdProjectItem *item)
  {
  if( item && item->getProject() == mProject ) {
    //Item from this project
    item->mTreeItem = new QTreeWidgetItem();
    item->mTreeItem->setText( 0, item->getTitle() );
    QTreeWidgetItem *ch = classList( item->getClass() );
    if( ch ) ch->addChild( item->mTreeItem );
    }
  }

void SdWProjectTree::removeItem(SdProjectItem *item)
  {
  if( item && item->getProject() == mProject ) {
    //Item from this project
    QTreeWidgetItem *ch = classList( item->getClass() );
    ch->removeChild( item->mTreeItem );
    }
  }

void SdWProjectTree::showEvent(QShowEvent *event)
  {

  //  //Установить пункты меню в соответствии со своим состоянием
  //  MainMenu::cmFileSave->setEnabled( mProject->isModified() );

  //  bool enable = currentIndex().internalId() == 100;
  //  MainMenu::cmObjectRename->setEnabled(enable);
  //  MainMenu::cmObjectDelete->setEnabled(enable);
  //  MainMenu::cmObjectCopy->setEnabled(enable);
  //  MainMenu::cmObjectCut->setEnabled(enable);

  //  //Проверить доступность в Clipboard объекта
  //  MainMenu::cmObjectPaste->setEnabled( QApplication::clipboard()->mimeData()->hasFormat(SALICAD_CLIP_FORMAT_OBJECT) );

  QTreeView::showEvent( event );
  }




QTreeWidgetItem *SdWProjectTree::createItem(const QString sname, const QString stoolTip, const QString sstatusTip)
  {
  QTreeWidgetItem *item = new QTreeWidgetItem();
  Q_CHECK_PTR(item);
  item->setText(0,sname);
  item->setToolTip(0,stoolTip);
  item->setStatusTip(0,sstatusTip);
  return item;
  }




void SdWProjectTree::fillTopItem(QTreeWidgetItem *item, int classId)
  {
  mProject->forEach( classId, [item](SdObject *obj) -> bool {
      //Образовать элемент списка
      QTreeWidgetItem *it = new QTreeWidgetItem();
      SdProjectItem *ctr = dynamic_cast<SdProjectItem*>( obj );
      if( ctr ) {
        it->setText( 0, ctr->getTitle() );
        ctr->mTreeItem = it;
        //it->setData(0, Qt::UserRole, QVariant(ctr->getId()) );
        item->addChild( it );
        }
      return true;
      }
      );
  }

QTreeWidgetItem *SdWProjectTree::classList(quint64 classId)
  {
  switch( classId ) {
    case dctSheet  : return mSheetList;
    case dctPlate  : return mPlateList;
    case dctSymbol : return mSymbolList;
    case dctPart   : return mPartList;
    case dctAlias  : return mAliasList;
    case dctText   : return mTextList;
    }
  return 0;
  }

