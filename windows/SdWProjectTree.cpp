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
#include <QFileInfo>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

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
  return QFileInfo(mFileName).fileName();
  }

bool SdWProjectTree::isProjectValid() const
  {
  return mProject != 0;
  }

void SdWProjectTree::cmObjectNew()
  {

  }

void SdWProjectTree::renameItem(SdProjectItem *item)
  {

  }

void SdWProjectTree::insertItem(SdProjectItem *item)
  {

  }

void SdWProjectTree::removeItem(SdProjectItem *item)
  {

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



class SdiFillTopItem : public SdIterator {
    QTreeWidgetItem *mRootItem;
  public:
    SdiFillTopItem( QTreeWidgetItem *item, int classId ) : SdIterator(classId), mRootItem(item) {}

    virtual bool operation(SdObjectPtr obj) {
      //Образовать элемент списка
      QTreeWidgetItem *item = new QTreeWidgetItem();
      SdProjectItem *ctr = dynamic_cast<SdProjectItem*>( obj );
      if( ctr ) {
        item->setText( 0, ctr->getTitle() );
        mRootItem->addChild( item );
        }
      return true;
      }
  };




void SdWProjectTree::fillTopItem(QTreeWidgetItem *item, int classId)
  {
  SdiFillTopItem i( item, classId );
  mProject->forEach( i );
  }

