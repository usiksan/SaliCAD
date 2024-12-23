﻿/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Project tree view. In this widget project presented as tree of projects items.
  Allow navigate on tree with edit operations.
*/

#include "SdWProjectTree.h"
#include "objects/SdPulsar.h"
#include "objects/SdProjectItem.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdObjectNetClient.h"
#include "objects/SdCopyMapProject.h"
#include "windows/SdPNewProjectItem_SelectType.h"
#include "windows/SdPNewProjectItem_EnterName.h"
#include "windows/SdPNewProjectItem_Master.h"
#include "windows/SdPNewProjectItem_3dMaster.h"
#include "windows/SdPNewProjectItem_Copy.h"
#include "windows/SdPNewProjectItem.h"
#include "windows/SdWCommand.h"
#include "windows/SdDParamEditor.h"
#include "library/SdLibraryStorage.h"
#include "SdDGetObject.h"

#include <QFileInfo>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QWizard>
#include <QtDebug>
#include <QJsonDocument>
#include <QMouseEvent>

SdWProjectTree::SdWProjectTree(const QString fname, SdProject *prj, QWidget *parent) :
  QTreeWidget(parent),
  mFileName(fname)
  {
  //setColumnCount(2);

  if( fname.isEmpty() ) {
    //Creating empty project [Создаем пустой проект]
    mFileName = SD_DEFAULT_FILE_NAME;
    mProject = new SdProject();
    }
  else {
    if( prj == nullptr ) {
      //Loading project from file [Загружаем проект из файла]
      mProject = SdProject::load( fname );
      if( mProject == nullptr ) {
        //Project is not loaded
        mFileName = SD_DEFAULT_FILE_NAME;
        mProject = new SdProject();
        }
      }
    else
      //Project loaded outside. Simple assign it
      mProject = prj;
    }
  //Check and mark newer objects
  mProject->newerCheckAndMark();
  //If project builded then display its contents [Если проект построен, то отобразить содержимое]
  buildVisualTree();

  mCurrentItem = mSheetList;

  connect( SdPulsar::sdPulsar, &SdPulsar::insertItem, this, &SdWProjectTree::insertItem );
  connect( SdPulsar::sdPulsar, &SdPulsar::removeItem, this, &SdWProjectTree::removeItem );
  connect( SdPulsar::sdPulsar, &SdPulsar::renameItem, this, &SdWProjectTree::renameItem );
  connect( SdPulsar::sdPulsar, &SdPulsar::highlightItem, this, &SdWProjectTree::highlightItem );
  connect( this, &SdWProjectTree::currentItemChanged, this, &SdWProjectTree::onCurrentItemChanged );
  connect( SdLibraryStorage::instance(), &SdLibraryStorage::updateNewestMark, this, &SdWProjectTree::updateNewestMark );
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
  return mProject != nullptr;
  }




bool SdWProjectTree::cmFileClose() {
  //Test if file saved
  if( mProject->isDirty() ) {
    int res = QMessageBox::question( this, tr("Warning!"),
                                     tr("Project \"%1\" changed! Save it?").arg( fileName() ),
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
  //Check if project is library project
  if( !mProject->isEditEnable() && mProject->isDirty() ) {
    if( mProject->getAuthor() != mProject->getDefaultAuthor() ) {
      if( QMessageBox::question( this, tr("Warning!"), tr("This project is library project and created another author. You can not edit it. Change author to You to enable editing?") ) != QMessageBox::Yes )
        return false;
      }
    mProject->titleSet( mProject->getTitle() );
    }
  return mProject->save( mFileName );
  }




bool SdWProjectTree::cmFileSaveAs()
  {
  QString title = QFileDialog::getSaveFileName(this, tr("Save project file"), QString(), tr("SaliCAD binary Files (*%1);; SaliCAD text Files (*%2)").arg(SD_BINARY_EXTENSION, SD_BASE_EXTENSION) );

  if( title.isEmpty() ) return false;

  mFileName = title;
  if( !mFileName.endsWith(SD_BASE_EXTENSION) && !mFileName.endsWith(SD_BINARY_EXTENSION) )
    mFileName.append( SD_BINARY_EXTENSION );

  SdWCommand::addToPreviousMenu( mFileName );
  SdPulsar::sdPulsar->emitRenameProject( mProject, fileName() );

  return cmFileSave();
  }




//Create new object in project hierarchy with wizard
void SdWProjectTree::cmObjectNew()
  {
  //Place for object
  SdProjectItemPtr item = nullptr;

  //Wizard
  QWizard wizard(this);
  //Fill it with pages
  wizard.setPage( SDP_NPI_TYPE,      new SdPNewProjectItem_SelectType( &item, mProject, &wizard) );
  wizard.setPage( SDP_NPI_NAME,      new SdPNewProjectItem_EnterName( &item, mProject, &wizard, false ) );
  wizard.setPage( SDP_NPI_MASTER,    new SdPNewProjectItem_Master( &item, mProject, &wizard) );
  wizard.setPage( SDP_NPI_3D_MASTER, new SdPNewProjectItem_3dMaster( &item, mProject, &wizard ) );
  wizard.setPage( SDP_NPI_COPY,      new SdPNewProjectItem_Copy( &item, mProject, &wizard) );
  if( wizard.exec() ) {
    //Append item to the project
    item->setHand();
    mProject->getUndo()->begin( tr("Creating object"), nullptr, false );
    mProject->insertChild( item, mProject->getUndo() );
    //Open window to edit item
    SdPulsar::sdPulsar->emitActivateItem( item );
    //Update status undo and redo commands
    cmUndoRedoUpdate();
    }
  else {
    //Delete created item
    if( item ) delete item;
    }
  }




//Load object from library
void SdWProjectTree::cmObjectLoad()
  {
  SdProjectItem *item = sdObjectOnly<SdProjectItem>( SdDGetObject::getObject( dctProjectItems, tr("Select object to load"), this ) );
  if( item ) {
    mProject->getUndo()->begin( tr("Load object"), nullptr, false );
    SdProjectItem *it = mProject->getFixedProjectItem( item );
    //Escape auto deletion
    if( it != nullptr ) it->setHand();
    //Delete source item
    delete item;
    //Update status undo and redo commands
    cmUndoRedoUpdate();
    }
  }




void SdWProjectTree::cmObjectRename()
  {
  SdProjectItemPtr item = dynamic_cast<SdProjectItem*>( mProject->item( mCurrentItem ) );
  if( item ) {
    if( item->isEditEnable() ) {
      SdPNewProjectItem_EnterName::nameProjectItem( &item, mProject, this, false );

      //Update status undo and redo commands
      cmUndoRedoUpdate();
      }
    else
      QMessageBox::warning( this, tr("Error!"), tr("To rename object it must be edit enable") );
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("This is not object. Select object to rename.") );
  }








void SdWProjectTree::cmObjectDelete()
  {
  SdProjectItemPtr item = dynamic_cast<SdProjectItem*>( mProject->item( mCurrentItem ) );
  if( item ) {
    if( mProject->isUsed(item) )
      QMessageBox::warning( this, tr("Warning!"), tr("Object is used by other objects. You can not delete it until dereferenced.") );
    else if( QMessageBox::question(this, tr("Warning!"), tr("Do You realy want to delete \'%1\'").arg(item->getTitle())) == QMessageBox::Yes ) {
      mProject->getUndo()->begin( tr("Deleting object"), nullptr, false );
      mProject->deleteChild( item, mProject->getUndo() );
      //Update status undo and redo commands
      cmUndoRedoUpdate();
      }
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("This is not object. Select object to delete.") );
  }




void SdWProjectTree::cmObjectCopy()
  {
  SdPtr<SdProjectItem> item( mProject->item( mCurrentItem ) );
  if( item.isValid() && (item->getClass() & (dctComponent | dctPart | dctSymbol) ) ) {
    //Prepare Json object with project and selection
    QJsonObject obj = mProject->jsonObjectTo();

    //Store object id to json
    obj.insert( QStringLiteral("ProjectItem UID"), item->getUid() );

    //Convert to byteArray
    QByteArray array = QJsonDocument( obj ).toJson(QJsonDocument::Compact);

    //Prepare mime data
    QMimeData *mime = new QMimeData();
    mime->setData( QStringLiteral(SD_CLIP_FORMAT_PITEM), array );

    //Insert into clipboard
    QGuiApplication::clipboard()->setMimeData( mime );
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("You can copy only symbols, parts and components. Select some from that object to copy.") );
  }




//Paste object from clipboard
void SdWProjectTree::cmObjectPaste()
  {
  const QMimeData *mime = QGuiApplication::clipboard()->mimeData();
  if( mime != nullptr && mime->hasFormat( QStringLiteral(SD_CLIP_FORMAT_PITEM)) ) {
    //Data with appropriate format present, read it

    //Retrive Json object from clipboard
    QJsonObject obj = QJsonDocument::fromJson( mime->data(QStringLiteral(SD_CLIP_FORMAT_PITEM)) ).object();

    //Create project
    SdProject *project = sdObjectOnly<SdProject>( SdObject::jsonObjectFrom(obj) );

    //selection reading
    QString uid = obj.value( QStringLiteral("ProjectItem UID") ).toString();

    if( project != nullptr && !uid.isEmpty() ) {

      mProject->getUndo()->begin( tr("Paste from clipboard"), nullptr, false );
      if( mProject->itemByUid(uid) )
        //Same object already present in project duplicate it
        duplicate( uid );

      else {
        //Simple insert object
        SdCopyMapProject copyMap(mProject);
        mProject->insertChild( project->itemByUid(uid)->copy( copyMap, false ), mProject->getUndo() );
        }


      delete project;

      }

    //Update status undo and redo commands
    cmUndoRedoUpdate();

    //Update newest marks
    updateNewestMark();
    }
  }





//Cut object to clipboard
void SdWProjectTree::cmObjectCut()
  {
  SdPtr<SdProjectItem> item( mProject->item( mCurrentItem ) );
  if( item.isValid() && (item->getClass() & (dctComponent | dctPart | dctSymbol) ) ) {
    if( mProject->isUsed(item.ptr()) )
      QMessageBox::warning( this, tr("Warning!"), tr("Object is used by other objects. You can not cut it until dereferenced, only copy.") );
    else {
      cmObjectCopy();
      mProject->deleteChild( item.ptr(), mProject->getUndo() );
      //Update status undo and redo commands
      cmUndoRedoUpdate();
      }
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("You can copy only symbols, parts and components. Select some from that object to copy.") );
  }




//Duplicate current object
void SdWProjectTree::cmObjectDuplicate()
  {
  SdPtr<SdProjectItem> item( mProject->item( mCurrentItem ) );
  if( item.isValid() && (item->getClass() & (dctComponent | dctPart | dctSymbol) ) ) {
    mProject->getUndo()->begin( tr("Duplicate object"), nullptr, false );
    duplicate( item->getUid() );
    //Update status undo and redo commands
    cmUndoRedoUpdate();
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("You can duplicate only symbols, parts and components. Select some from that object to copy.") );
  }




void SdWProjectTree::cmObjectSort()
  {
  //Sort project items by alphabet
  mProject->sort( [] (SdObject *obj1, SdObject *obj2) -> bool {
    SdProjectItem *item1 = dynamic_cast<SdProjectItem*>(obj1);
    SdProjectItem *item2 = dynamic_cast<SdProjectItem*>(obj2);
    if( item1 == nullptr || item2 == nullptr ) return false;
    //Special case for sheets
    //We compare it by its numbers
    if( item1->getClass() == dctSheet && item2->getClass() == dctSheet )
      return item1->paramGet( SD_SHEET_INDEX_PARAM ).compare( item2->paramGet( SD_SHEET_INDEX_PARAM ), Qt::CaseInsensitive ) < 0;
    return item1->getTitle().compare( item2->getTitle(), Qt::CaseInsensitive ) < 0;
    } );

  //Rebuild visual tree
  buildVisualTree();
  }




//Edit object param
void SdWProjectTree::cmObjectParam()
  {
  //Get current item and show its param
  SdPtr<SdProjectItem> item( mProject->item( mCurrentItem ) );
  if( item.isValid() ) {
    SdDParamEditor editor( tr("Edit param for [%1]").arg( item->getTitle() ), item->paramTable(), mProject, item->isEditEnable(), false, this );
    if( editor.exec() ) {
      //Edit successfull. Apply changes
      item->paramTableSet( editor.paramTable(), mProject->getUndo() );
      //Update status undo and redo commands
      cmUndoRedoUpdate();
      }
    }
  }




//Edit project param
void SdWProjectTree::cmProjectParam()
  {
  SdDParamEditor editor( tr("Edit project param"), mProject->paramTable(), mProject, true, true, this );
  if( editor.exec() ) {
    //Edit successfull. Apply changes
    mProject->paramTableSet( editor.paramTable(), mProject->getUndo() );
    //Update status undo and redo commands
    cmUndoRedoUpdate();
    //Update visual representation
    SdPulsar::sdPulsar->emitRenameProject( mProject, fileName() );
    }
  }




//Upgrade items with newer versions
void SdWProjectTree::cmProjectUpgrade()
  {
  //Perform upgrading
  mProject->upgradeNewerItems( mProject->getUndo(), this );
  //Update markers
  mProject->newerCheckAndMark();
  }




void SdWProjectTree::cmClipboardChange()
  {
  SdWCommand::cmObjectPaste->setEnabled( QApplication::clipboard()->mimeData()->hasFormat(SD_CLIP_FORMAT_PITEM) );
  }




void SdWProjectTree::cmEditUndo()
  {
  getProject()->getUndo()->undoStep();
  }




void SdWProjectTree::cmEditRedo()
  {
  getProject()->getUndo()->redoStep();
  }




//Update status undo and redo commands
void SdWProjectTree::cmUndoRedoUpdate()
  {
  getProject()->getUndo()->undoRedoUpdate();
  }





//AFTER rename item
void SdWProjectTree::renameItem(SdProjectItem *item)
  {
  if( item && item->getProject() == mProject ) {
    //Item from this project
    item->mTreeItem->setIcon( 0, QIcon(item->getIconName()) );
    item->mTreeItem->setText( 0, item->getExtendTitle() );
    item->mTreeItem->setToolTip( 0, item->getToolTip() );
    }
  }




void SdWProjectTree::insertItem(SdProjectItem *item)
  {
  if( item && item->getProject() == mProject ) {
    //Item from this project
    item->mTreeItem = new QTreeWidgetItem();
    item->mTreeItem->setText( 0, item->getExtendTitle() );
    item->mTreeItem->setToolTip( 0, item->getToolTip() );
    item->mTreeItem->setIcon( 0, QIcon(item->getIconName()) );
    QTreeWidgetItem *ch = classList( item->getClass() );
    if( ch ) ch->addChild( item->mTreeItem );
    if( item->is3dAllowed() ) {
      item->m3dTreeItem = new QTreeWidgetItem();
      item->mTreeItem->addChild( item->m3dTreeItem );
      item->m3dTreeItem->setText( 0, tr("3d view") );
      }
    else item->m3dTreeItem = nullptr;
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




//When editor activated send this signal
//on it we highlight appropriate line in project tree
void SdWProjectTree::highlightItem(SdProjectItem *item)
  {
  if( item && item->getProject() == mProject && mCurrentItem != item->mTreeItem ) {
    setCurrentItem( item->mTreeItem );
    }
  }





//When project view editor activated send this signal
//on it we highlight sheets
void SdWProjectTree::highlightSheets()
  {
  if( mCurrentItem && mProject->item(mCurrentItem) ) {
    setCurrentItem( mSheetList );
    }
  }




void SdWProjectTree::onCurrentItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev)
  {
  Q_UNUSED(prev)
  mCurrentItem = cur;
  bool disable = cur == mSheetList || cur == mPlateList || cur == mSymbolList ||
                 cur == mComponentList || cur == mPartList || cur == mTextList;
  bool enable = !disable && cur != nullptr;

  //qDebug() << "onCurrentItem" << cur << disable << enable;
  SdWCommand::cmObjectRename->setEnabled(enable);
  SdWCommand::cmObjectParam->setEnabled(enable);
  SdWCommand::cmObjectDelete->setEnabled(enable);
  SdWCommand::cmObjectCopy->setEnabled(enable);
  SdWCommand::cmObjectCut->setEnabled(enable);
  SdWCommand::cmObjectDuplicate->setEnabled(enable);

  SdWCommand::cmFilePrint->setEnabled(enable);
  SdWCommand::cmFileExport->setEnabled(enable);
  SdWCommand::cmFileImport->setEnabled(enable);

  if( enable && cur != nullptr ) {
    SdProjectItem *item = dynamic_cast<SdProjectItem*>( mProject->item( cur ) );
    if( item != nullptr ) {
      //qDebug() << "activate item" << item->getTitle();
      SdPulsar::sdPulsar->emitActivateItem( item );
      }
    else {
      //Try find in 3d
      item = dynamic_cast<SdProjectItem*>( mProject->item3d( cur ) );
      if( item != nullptr ) {
        qDebug() << "activate 3d item" << item->getTitle();
        SdPulsar::sdPulsar->emitActivateItem3d( item );
        }
      }
    }
  else if( cur != nullptr ) {
    //qDebug() << "activate project" << fileName() << cur << prev;
    SdPulsar::sdPulsar->emitActivateProject( mProject, fileName() );
    }
  }





//When received new objects we update newest mark
void SdWProjectTree::updateNewestMark()
  {
  //Check and mark newer objects
  mProject->newerCheckAndMark();
  }






void SdWProjectTree::showEvent(QShowEvent *event)
  {
  qDebug() << "show event" << fileName();

  //Установить пункты меню в соответствии со своим состоянием
  SdWCommand::cmFileSave->setEnabled( true );

  if( mCurrentItem )
    onCurrentItemChanged( mCurrentItem, nullptr );
  else {
//    qDebug() << "activate project" << fileName();
    SdPulsar::sdPulsar->emitActivateProject( mProject, fileName() );
    }


  //Проверить доступность в Clipboard объекта
  //MainMenu::cmObjectPaste->setEnabled( QApplication::clipboard()->mimeData()->hasFormat(SALICAD_CLIP_FORMAT_OBJECT) );

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





//!
//! \brief fillTopItem Fill node of top level [Заполнить элемент верхнего уровня]
//! \param item        Top level node
//! \param classId     Class id for this top level
//! \param add3d       Append 3d view
//!
void SdWProjectTree::fillTopItem(QTreeWidgetItem *item, quint64 classId, bool add3d)
  {
  mProject->forEach( classId, [item, add3d](SdObject *obj) -> bool {
    //Create list element [Образовать элемент списка]
    SdPtr<SdProjectItem> ctr( obj );
    if( ctr.isValid() ) {
      QTreeWidgetItem *it = new QTreeWidgetItem();
      it->setText( 0, ctr->getExtendTitle() );
      it->setToolTip( 0, ctr->getToolTip() );
      it->setIcon( 0, QIcon(ctr->getIconName()) );
      ctr->mTreeItem = it;
      item->addChild( it );
      if( add3d ) {
        //For objects which has 3d view append 3d view subitem
        ctr->m3dTreeItem = new QTreeWidgetItem();
        it->addChild( ctr->m3dTreeItem );
        ctr->m3dTreeItem->setText( 0, tr("3d view") );
        }
      else ctr->m3dTreeItem = nullptr;
      }
    return true;
    }
  );
  }




void SdWProjectTree::buildVisualTree()
  {
  clear();
  if( mProject ) {
    setHeaderHidden(true);
    setRootIsDecorated(true);
    setItemsExpandable(true);
    setLineWidth(1);

    //Fill root level list [Заполнить список корневого уровня]
    mSymbolList    = createItem( tr("Symbols"), tr("Project symbols list"), tr("Contains project symbols list") );   //!< Схемные изображения компонентов
    mPartList      = createItem( tr("Parts"), tr("Project parts list"), tr("Contains project parts list") );     //!< Корпуса компонентов
    mComponentList = createItem( tr("Components"), tr("Project components list"), tr("Contains project components list, each of them is agregation symbols width appropriate part") );    //!< Компоненты (схемное избр. + корпус + параметры)
    mSheetList     = createItem( tr("Sheets"), tr("Project shematic sheets"), tr("Contains project shematic sheet list") );    //!< Листы схем
    mPlateList     = createItem( tr("Construct"), tr("Project constructs and pcb list"), tr("Contains project construct and pcb list") );    //!< Печатные платы
    mTextList      = createItem( tr("Other docs"), tr("Project text and table docs list"), tr("Contains project text, table and others docs") );     //!< Текстовые документы

    addTopLevelItem( mSheetList );
    addTopLevelItem( mPlateList );
    addTopLevelItem( mComponentList );
    addTopLevelItem( mSymbolList );
    addTopLevelItem( mPartList );
    addTopLevelItem( mTextList );

    fillTopItem( mSheetList, dctSheet, false );
    fillTopItem( mPlateList, dctPlate, true );
    fillTopItem( mComponentList, dctComponent, false );
    fillTopItem( mSymbolList, dctSymbol, false );
    fillTopItem( mPartList, dctPart, true );
    fillTopItem( mTextList, dctRich, false );
    }
  }




QTreeWidgetItem *SdWProjectTree::classList(quint64 classId)
  {
  switch( classId ) {
    case dctSheet       : return mSheetList;
    case dctPlate       : return mPlateList;
    case dctSymbol      : return mSymbolList;
    case dctPart        : return mPartList;
    case dctComponent   : return mComponentList;
    case dctRich        : return mTextList;
    }
  return nullptr;
  }




//Duplicate object with id
void SdWProjectTree::duplicate(const QString &uid)
  {
  SdPtr<SdProjectItem> item( mProject->itemByUid( uid ) );
  if( item.isValid() && (item->getClass() & (dctComponent | dctPart | dctSymbol) ) ) {
    //Create copy of object
    SdCopyMapProject copyMap(mProject);
    SdProjectItem *copy = dynamic_cast<SdProjectItem*>( item->copy( copyMap, false ) );
    copy->setUnicalTitle( QString() );
    copy->setEditEnable( true, QString() );
    //Insert item to project
    mProject->insertChild( copy, mProject->getUndo() );
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("You can duplicate only symbols, parts and components. Select some from that object to copy.") );
  }



//Reimplement to display context menu
void SdWProjectTree::mousePressEvent(QMouseEvent *event)
  {
  if( event->button() == Qt::RightButton ) {
    QTreeWidget::mousePressEvent( event );
    //On right button display context menu
    SdWCommand::menuObject->exec( QCursor::pos() );
    }
  else
    //On other buttons - default
    QTreeWidget::mousePressEvent( event );
  }




//Reimplement to support keys
void SdWProjectTree::keyPressEvent(QKeyEvent *event)
  {
  switch( event->key() ) {
    //Help key
    case Qt::Key_F1 :
      SdPulsar::sdPulsar->emitHelpTopic( QStringLiteral("SdWProjectTree.htm") );
      break;

    //Delete key
    case Qt::Key_Delete :
      cmObjectDelete();
      break;

    //Insert key
    case Qt::Key_Insert :
      cmObjectNew();
      break;

    //Enter key
    case Qt::Key_Return :
      cmObjectRename();
      break;
    }
  QTreeWidget::keyPressEvent( event );
  }
