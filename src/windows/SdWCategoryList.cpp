/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget to display and navigate on category hierarchy
*/
#include "SdConfig.h"
#include "objects/SdEnvir.h"
#include "SdWCategoryList.h"

#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QMouseEvent>

#define CAT_TITLE    QStringLiteral("title")
#define CAT_KEY      QStringLiteral("key")
#define CAT_CHILD    QStringLiteral("child")
#define ROLE_KEY     (Qt::UserRole + 1)
#define ROLE_PATH    (Qt::UserRole + 2)

QJsonArray  SdWCategoryList::mCategoryMap;   //List of all categories


static bool isDirty;
static QMenu *menuCategory;


SdWCategoryList::SdWCategoryList(QWidget *parent) :
  QTreeWidget(parent)
  {
  if( mCategoryMap.isEmpty() ) {
    //Category map not loaded yet
    //Load language
    QString fname = sdEnvir->mCategoryPath + SdEnvir::languageGet() + QStringLiteral(SD_CATEGORY_FILE);

    //If current language file not accecible then use default language "en"
    if( !QFile::exists(fname) )
      fname = sdEnvir->mCategoryPath + QStringLiteral( "en" SD_CATEGORY_FILE );

    if( QFile::exists( fname ) ) {
      //Category file exist. Load it
      QFile file( fname );
      if( file.open( QIODevice::ReadOnly ) ) {
        mCategoryMap = QJsonDocument::fromJson( file.readAll() ).array();
        isDirty = false;
        }
      }
    }

  //Fill with contents
  fill();

  connect( this, &SdWCategoryList::itemClicked, this, &SdWCategoryList::cmCategory );
//  connect( this, &SdWCategoryList::itemDoubleClicked, this, &SdWCategoryList::cmEnter );
  }





SdWCategoryList::~SdWCategoryList()
  {
  if( isDirty && QMessageBox::question( this, tr("Warning!"), tr("Category tree changed. Do you want to save it?") ) == QMessageBox::Yes )
    cmSaveAs();
  }





//Append new category
void SdWCategoryList::cmAppend()
  {
  //Current item
  QTreeWidgetItem *current = currentItem();

  //Enter title for category
  QString title = QInputDialog::getText( this, tr("Append category"), tr("Enter category title") );

  //If no title then nothind done
  if( title.isEmpty() )
    return;

  //QString key = QInputDialog::getText( this, tr("Append key"), tr("Enter category subkey") );

  //Insert to item
  QTreeWidgetItem *item = new QTreeWidgetItem( {title} );
  item->setToolTip( 0, title );
  item->setIcon( 0, QIcon( QString(":/pic/brFile.png") ) );

  int kc;
  if( current && current != topLevelItem(0) ) {
    kc = current->childCount();
    current->setIcon( 0, QIcon( QString(":/pic/brDir.png") ) );
    current->addChild( item );
    }
  else {
    kc = topLevelItemCount();
    addTopLevelItem( item );
    }
  QChar ch('a' + kc);
  QString key(ch);
  item->setData( 0, ROLE_KEY, key );

  isDirty = true;
  }




void SdWCategoryList::cmRemove()
  {
  QTreeWidgetItem *item = currentItem();
  if( item != nullptr ) {
    if( item->text(0) == tr("All") )
      QMessageBox::warning( this, tr("Error"), tr("This item can't be deleted. Select another.") );
    else {
      if( QMessageBox::question( this, tr("Warning"), tr("Are You sure to delete '%1' category and all its subcategories").arg( item->text(0) ) ) == QMessageBox::Yes ) {
        isDirty = true;
        QTreeWidgetItem *parent = item->parent();
        if( parent == nullptr )
          //Remove from top
          removeItemWidget( item, 0 );
        else
          parent->removeChild( item );
        }
      }
    }
  else
    QMessageBox::warning( this, tr("Error"), tr("No selected item to delete. Select and reply.") );
  }





//Delete all categories
void SdWCategoryList::cmClear()
  {
  if( QMessageBox::question( this, tr("Warning!"), tr("Are You sure to delete ALL categories? This operation can not be undo!")) == QMessageBox::Yes ) {
    mCategoryMap = QJsonArray();
    fill();
    }
  }






//Edit category title
void SdWCategoryList::cmEditTitle()
  {
  QTreeWidgetItem *item = currentItem();
  if( item != nullptr ) {
    if( item->text(0) == tr("All") )
      QMessageBox::warning( this, tr("Error"), tr("This item can't be edited. Select another.") );
    else {
      QString title = item->text(0);
      title = QInputDialog::getText( this, tr("Category title edit"), tr("Enter new category title"), QLineEdit::Normal, title );
      if( !title.isEmpty() ) {
        //Update title
        item->setText( 0, title );
        isDirty = true;
        }
      }
    }
  else
    QMessageBox::warning( this, tr("Error"), tr("No selected item to edit. Select and reply.") );
  }





//Edit category key
void SdWCategoryList::cmEditKey()
  {
  QTreeWidgetItem *item = currentItem();
  if( item != nullptr ) {
    if( item->text(0) == tr("All") )
      QMessageBox::warning( this, tr("Error"), tr("This item can't be edited. Select another.") );
    else {
      QString key = item->data( 0, ROLE_KEY ).toString();
      key = QInputDialog::getText( this, tr("Category key edit"), tr("Enter new category key"), QLineEdit::Normal, key );
      if( !key.isEmpty() ) {
        //Update key
        item->setData( 0, ROLE_KEY, key );
        isDirty = true;
        }
      }
    }
  else
    QMessageBox::warning( this, tr("Error"), tr("No selected item to edit. Select and reply.") );
  }








//Load categories from file
void SdWCategoryList::cmLoad()
  {
  if( isDirty && QMessageBox::question( this, tr("Warning!"), tr("Current category list not saved. If You continue then all changed will be lost. Continue")) != QMessageBox::Yes )
    return;

  QString str = QFileDialog::getOpenFileName( this, tr("Category hierarchy file"), sdEnvir->mCategoryPath, QString("*" SD_CATEGORY_EXTENSION) );
  if( !str.isEmpty() ) {
    //Test if file exist
    if( QFile::exists( str ) ) {
      //File exist. Load it
      QFile file( str );
      if( file.open( QIODevice::ReadOnly ) ) {
        mCategoryMap = QJsonDocument::fromJson( file.readAll() ).array();
        isDirty = false;
        }
      }
    }
  }








//Save categories to file with other file name
void SdWCategoryList::cmSaveAs()
  {
  QString str = QFileDialog::getSaveFileName( this, tr("Category hierarchy file"), sdEnvir->mCategoryPath, QString("*" SD_CATEGORY_EXTENSION) );
  if( !str.isEmpty() ) {
    if( !str.endsWith( QString(SD_CATEGORY_EXTENSION)) )
      str.append( QString(SD_CATEGORY_EXTENSION) );
    //Scan current tree
    mCategoryMap = QJsonArray();
    for( int i = 1; i < topLevelItemCount(); i++ )
      mCategoryMap.append( scanItem(topLevelItem(i)) );
    QFile file( str );
    if( file.open( QIODevice::WriteOnly ) ) {
      QJsonDocument doc( mCategoryMap );
      file.write( doc.toJson() );
      isDirty = false;
      }
    }
  }





//On select category in list
void SdWCategoryList::cmCategory(QTreeWidgetItem *item, int column)
  {
  Q_UNUSED(column);
  if( item )
    emit category( item->data( 0, ROLE_PATH).toString() );
  }






//Locate category
void SdWCategoryList::cmLocate(const QString path)
  {
  if( path.isEmpty() )
    setCurrentItem( topLevelItem(0), 0 );
  else
    setCurrentItem( mItemMap.value(path, topLevelItem(0)), 0 );
  }







//On double click category
//void SdWCategoryList::cmEnter(QTreeWidgetItem *item)
//  {
//  if( item ) {
//    mCurrentId = item->data( Qt::UserRole ).toInt();
//    fill();
//    }
//  }




//Fill list with contents of current category path
void SdWCategoryList::fill()
  {
  //Remove all previous items
  clear();

  //This is root object, append All
  addTopLevelItem( buildItem( true, tr("All"), QString(), QString() ) );

  //Get top object array
  for( const QJsonValue v : mCategoryMap ) {
    addTopLevelItem( buildItem( v.toObject(), QStringLiteral(SD_CATEGORY_PREFIX) ) );
    }
  }




//Build item from its description
QTreeWidgetItem *SdWCategoryList::buildItem(bool dir, const QString title, const QString key, const QString parent)
  {
  //Full path of item
  QString path(parent + QStringLiteral(".") + key);

  //Insert to item
  QTreeWidgetItem *item = new QTreeWidgetItem( {title} );

  if( parent.isEmpty() && key.isEmpty() )
    path.clear();
  else
    mItemMap.insert( path, item );
  item->setToolTip( 0, title );
  item->setIcon( 0, QIcon( dir ? QString(":/pic/brDir.png") : QString(":/pic/brFile.png") ) );
  item->setData( 0, ROLE_KEY, key );
  item->setData( 0, ROLE_PATH, path );
  return item;
  }





//Build item and its children on json object
QTreeWidgetItem *SdWCategoryList::buildItem( const QJsonObject &obj, const QString parent )
  {
  const QJsonArray child = obj.value( CAT_CHILD ).toArray();
  QTreeWidgetItem *item = buildItem( child.count() > 0, obj.value(CAT_TITLE).toString(), obj.value(CAT_KEY).toString(), parent );
  QString path = parent + QChar('.') + obj.value(CAT_KEY).toString();
  //Build all child items
  for( const QJsonValue v : child )
    item->addChild( buildItem( v.toObject(), path ) );
  return item;
  }





//Retrive info from tree item build json object
QJsonObject SdWCategoryList::scanItem(QTreeWidgetItem *item)
  {
  QJsonObject obj;
  //Retrive title
  obj.insert( CAT_TITLE, item->text(0) );
  //Retrive key
  obj.insert( CAT_KEY, item->data( 0, ROLE_KEY ).toString() );
  //If item has children then scan all children and append it to child array
  if( item->childCount() ) {
    QJsonArray arr;
    for( int i = 0; i < item->childCount(); i++ ) {
      arr.append( scanItem( item->child(i) )  );
      }
    obj.insert( CAT_CHILD, arr );
    }
  return obj;
  }







void SdWCategoryList::mousePressEvent(QMouseEvent *event)
  {
  if( event->button() == Qt::RightButton ) {
    QTreeWidget::mousePressEvent( event );
    //On right button display context menu
    if( menuCategory == nullptr ) {
      menuCategory = new QMenu( tr("Category") );
      //Category manipulations
      menuCategory->addAction( QIcon(QString(":/pic/objectNew.png")), tr("Add new category"), this, &SdWCategoryList::cmAppend );
      menuCategory->addAction( QIcon(QString(":/pic/objectRename.png")), tr("Edit title..."), this, &SdWCategoryList::cmEditTitle );
      menuCategory->addAction( QIcon(QString(":/pic/objectParam.png")), tr("Edit category key..."), this, &SdWCategoryList::cmEditKey );
      menuCategory->addAction( QIcon(QString(":/pic/objectDelete.png")), tr("Delete category"), this, &SdWCategoryList::cmRemove );
      //menuCategory->addAction( QIcon(QString(":/pic/objectCut.png")), tr("Cut"), this, &SdWCategoryList::cmCut );
      //cmdPaste = menuCategory->addAction( QIcon(QString(":/pic/objectPaste.png")), tr("Paste"), this, &SdWCategoryList::cmPaste );
      menuCategory->addSeparator();

      //Load-save
      menuCategory->addAction( QIcon(QString(":/pic/fileOpen.png")), tr("Load category tree..."), this, &SdWCategoryList::cmLoad );
      //menuCategory->addAction( QIcon(QString(":/pic/save.png")), tr("Save category tree"), this, &SdWCategoryList::cmSave );
      menuCategory->addAction( QIcon(QString(":/pic/save_as.png")), tr("Save category tree as..."), this, &SdWCategoryList::cmSaveAs );

      menuCategory->addSeparator();
      menuCategory->addAction( QIcon(QString(":/pic/new.png")), tr("Delete ALL categories"), this, &SdWCategoryList::cmClear );
      }
    //When no clipboard data we disabled paste cmd
    //cmdPaste->setDisabled( clipboardId );
    menuCategory->exec( QCursor::pos() );
    }
  else
    //On other buttons - default
    QTreeWidget::mousePressEvent( event );
  }
