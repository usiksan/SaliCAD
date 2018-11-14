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

#include "objects/SdEnvir.h"
#include "SdWCategoryList.h"

#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>

#define CAT_PARENT   QStringLiteral("parent")
#define CAT_TITLE    QStringLiteral("title")
#define CAT_KEY      QStringLiteral("key")
#define CAT_CHILD    QStringLiteral("child")
#define CAT_ID_COUNT QStringLiteral("idCount")

QJsonObject  SdWCategoryList::mCategoryMap;   //List of all categories
int          SdWCategoryList::mCurrentId;     //Current category id
QMenu       *SdWCategoryList::menuCategory;  //Context category menu
QAction     *SdWCategoryList::cmdPaste;      //Paste from clipboard command

static int clipboardId;
static bool isDirty;

SdWCategoryList::SdWCategoryList(QWidget *parent) :
  QListWidget(parent)
  {
  if( mCategoryMap.isEmpty() ) {
    //Category map not loaded yet
    if( QFile::exists( sdEnvir->mCategoryPath ) ) {
      //Default category file exist. Load it
      QFile file( sdEnvir->mCategoryPath );
      if( file.open( QIODevice::ReadOnly ) ) {
        mCategoryMap = QJsonDocument::fromJson( file.readAll() ).object();
        mCurrentId = 0;
        isDirty = false;
        }
      }
    }

  //Fill with contens
  fill();

  connect( this, &SdWCategoryList::itemClicked, this, &SdWCategoryList::cmCategory );
  connect( this, &SdWCategoryList::itemDoubleClicked, this, &SdWCategoryList::cmEnter );
  }





SdWCategoryList::~SdWCategoryList()
  {
  if( clipboardId ) {
    removeAllChild( clipboardId );
    clipboardId = 0;
    }
  if( isDirty && QMessageBox::question( this, tr("Warning!"), tr("Category tree changed. Do you want to save it?") ) == QMessageBox::Yes )
    cmSave();
  }





//Append new category
void SdWCategoryList::cmAppend()
  {
  //Enter title for category
  QString title = QInputDialog::getText( this, tr("Append category"), tr("Enter category title") );

  //If no title then nothind done
  if( title.isEmpty() )
    return;

  //Get id for new category
  int id = 1;
  if( mCategoryMap.contains( CAT_ID_COUNT ) )
    id = mCategoryMap.value( CAT_ID_COUNT ).toInt();

  //Build category object
  QJsonObject obj;
  obj.insert( CAT_PARENT, mCurrentId );
  obj.insert( CAT_TITLE, title );
  obj.insert( CAT_KEY, title );

  //Insert new category
  mCategoryMap.insert( QString::number(id), obj );

  //Append category to visual list
  addItem( obj, id );

  //Append category to list
  obj = mCategoryMap.value( QString::number(mCurrentId) ).toObject();
  QJsonArray arr = obj.value( CAT_CHILD ).toArray();
  arr.append( id );
  obj.insert( CAT_CHILD, arr );
  mCategoryMap.insert( QString::number(mCurrentId), obj );

  //Update category id count
  id++;
  mCategoryMap.insert( CAT_ID_COUNT, id );

  isDirty = true;
  }




void SdWCategoryList::cmRemove()
  {
  int row = currentRow();
  if( row > 0 ) {
    if( QMessageBox::question( this, tr("Warning"), tr("Are You sure to delete '%1' category and all its subcategories") ) == QMessageBox::Yes ) {
      isDirty = true;

      //Get id to delete
      int id = item(row)->data( Qt::UserRole ).toInt();
      //Remove from visual list
      removeItemWidget( item(row) );

      //Remove all child
      removeAllChild( id );

      //Remove from parent
      QJsonObject obj = mCategoryMap.value( QString::number(mCurrentId) ).toObject();
      QJsonArray arr = obj.value( CAT_CHILD ).toArray();
      row--;
      if( arr.at(row).toInt() == id )
        arr.removeAt(row);
      else {
        qDebug() << "cmRemove something wrong";
        for( int i = 0; i < arr.count(); i++ )
          if( arr.at(i).toInt() == id ) {
            //found
            arr.removeAt(i);
            break;
            }
        }
      //Save updated array
      obj.insert( CAT_CHILD, arr );
      //Save updated object
      mCategoryMap.insert( QString::number(mCurrentId), obj );
      }
    }
  else if( row == 0 )
    QMessageBox::warning( this, tr("Error"), tr("This item can't be deleted. Select another.") );
  else
    QMessageBox::warning( this, tr("Error"), tr("No selected item to delete. Select and reply.") );
  }






//Edit category title
void SdWCategoryList::cmEditTitle()
  {
  int row = currentRow();
  if( row > 0 ) {
    isDirty = true;

    //Get id to edit
    int id = item(row)->data( Qt::UserRole ).toInt();
    //Edit title
    QJsonObject obj = mCategoryMap.value( QString::number(id) ).toObject();
    QString str = QInputDialog::getText( this, tr("Category title edit"), tr("Enter new category title"), QLineEdit::Normal, obj.value( CAT_TITLE ).toString() );
    if( !str.isEmpty() ) {
      //Update title
      obj.insert( CAT_TITLE, str );
      //Update visual representation
      item(row)->setText( str );
      //Save updated object
      mCategoryMap.insert( QString::number(id), obj );
      }
    }
  else if( row == 0 )
    QMessageBox::warning( this, tr("Error"), tr("This item can't be edited. Select another.") );
  else
    QMessageBox::warning( this, tr("Error"), tr("No selected item to edit. Select and reply.") );
  }




//Edit category key
void SdWCategoryList::cmEditKey()
  {
  int row = currentRow();
  if( row > 0 ) {
    isDirty = true;

    //Get id to edit
    int id = item(row)->data( Qt::UserRole ).toInt();
    //Edit title
    QJsonObject obj = mCategoryMap.value( QString::number(id) ).toObject();
    QString str = QInputDialog::getText( this, tr("Category key edit"), tr("Enter new category key"), QLineEdit::Normal, obj.value( CAT_KEY ).toString() );
    if( !str.isEmpty() ) {
      //Update title
      obj.insert( CAT_KEY, str );
      //Update visual representation
      item(row)->setToolTip( str );
      //Save updated object
      mCategoryMap.insert( QString::number(id), obj );
      }
    }
  else if( row == 0 )
    QMessageBox::warning( this, tr("Error"), tr("This item can't be edited. Select another.") );
  else
    QMessageBox::warning( this, tr("Error"), tr("No selected item to edit. Select and reply.") );
  }





//Cut category from list
void SdWCategoryList::cmCut()
  {
  int row = currentRow();
  if( row > 0 ) {
    isDirty = true;

    //Get id to delete
    clipboardId = item(row)->data( Qt::UserRole ).toInt();
    //Remove from visual list
    removeItemWidget( item(row) );

    //Remove from parent
    QJsonObject obj = mCategoryMap.value( QString::number(mCurrentId) ).toObject();
    QJsonArray arr = obj.value( CAT_CHILD ).toArray();
    row--;
    if( arr.at(row).toInt() == clipboardId )
      arr.removeAt(row);
    else {
      qDebug() << "cmRemove something wrong";
      for( int i = 0; i < arr.count(); i++ )
        if( arr.at(i).toInt() == clipboardId ) {
          //found
          arr.removeAt(i);
          break;
          }
      }
    //Save updated array
    obj.insert( CAT_CHILD, arr );
    //Save updated object
    mCategoryMap.insert( QString::number(mCurrentId), obj );
    }
  else if( row == 0 )
    QMessageBox::warning( this, tr("Error"), tr("This item can't be deleted. Select another.") );
  else
    QMessageBox::warning( this, tr("Error"), tr("No selected item to delete. Select and reply.") );
  }




//Paste category to list
void SdWCategoryList::cmPaste()
  {
  if( clipboardId ) {
    isDirty = true;

    //Append category to list
    QJsonObject obj = mCategoryMap.value( QString::number(mCurrentId) ).toObject();
    QJsonArray arr = obj.value( CAT_CHILD ).toArray();
    arr.append( clipboardId );
    obj.insert( CAT_CHILD, arr );
    mCategoryMap.insert( QString::number(mCurrentId), obj );

    //Append catogory to visual list
    addItemById( clipboardId );
    clipboardId = 0;
    }
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
        mCategoryMap = QJsonDocument::fromJson( file.readAll() ).object();
        mCurrentId = 0;
        //Update default category
        sdEnvir->mCategoryPath = str;
        isDirty = false;
        }
      }
    }
  }




//Save categories to file
void SdWCategoryList::cmSave()
  {
  //Save with default file name
  QFile file( sdEnvir->mCategoryPath );
  if( file.open( QIODevice::WriteOnly ) ) {
    QJsonDocument doc( mCategoryMap );
    file.write( doc.toJson() );
    isDirty = false;
    }
  }




//Save categories to file with other file name
void SdWCategoryList::cmSaveAs()
  {
  QString str = QFileDialog::getSaveFileName( this, tr("Category hierarchy file"), sdEnvir->mCategoryPath, QString("*" SD_CATEGORY_EXTENSION) );
  if( !str.isEmpty() ) {
    if( !str.endsWith( QString(SD_CATEGORY_EXTENSION)) )
      str.append( QString(SD_CATEGORY_EXTENSION) );
    QFile file( str );
    if( file.open( QIODevice::WriteOnly ) ) {
      QJsonDocument doc( mCategoryMap );
      file.write( doc.toJson() );
      sdEnvir->mCategoryPath = str;
      isDirty = false;
      }
    }
  }





//On select category in list
void SdWCategoryList::cmCategory(QListWidgetItem *item)
  {
  if( item && row( item ) )
    emit category( item->toolTip() );
  }




//On double click category
void SdWCategoryList::cmEnter(QListWidgetItem *item)
  {
  if( item ) {
    mCurrentId = item->data( Qt::UserRole ).toInt();
    fill();
    }
  }




//Fill list with contens of current category path
void SdWCategoryList::fill()
  {
  //Remove all previous items
  clear();

  if( mCurrentId == 0 ) {
    //This is root object, append All
    addItemInt( false, tr("All"), QString(), 0 );
    }
  //Get needed object
  QJsonObject obj = mCategoryMap.value( QString::number(mCurrentId) ).toObject();
  if( obj.isEmpty() ) {
    //No category with this id
    if( mCurrentId ) {
      //Structure failure
      mCurrentId = 0;
      fill();
      }
    }
  else {
    if( mCurrentId != 0 )
      addItemInt( true, tr("[Up]"), QString(), obj.value(CAT_PARENT).toInt() );
    QJsonArray arr = obj.value( CAT_CHILD ).toArray();
    for( auto iter = arr.constBegin(); iter != arr.constEnd(); iter++ )
      addItemById( iter->toInt() );
    }
  }



//Add item to category list
void SdWCategoryList::addItemInt(bool dir, const QString title, const QString key, int id)
  {
  QListWidgetItem *item = new QListWidgetItem( QIcon( dir ? QString(":/pic/brDir.png") : QString(":/pic/brFile.png") ), title );
  item->setToolTip(key);
  item->setData( Qt::UserRole, id );
  insertItem( count(), item );
  }




//Add item to category list from category object
void SdWCategoryList::addItem( const QJsonObject &obj, int id )
  {
  bool isDir = obj.value( CAT_CHILD ).toArray().count() != 0;
  addItemInt( isDir, obj.value( CAT_TITLE ).toString(), obj.value( CAT_KEY ).toString(), id );
  }




//Add item to visual category list by its id
void SdWCategoryList::addItemById(int id)
  {
  QJsonObject obj = mCategoryMap.value( QString::number(id) ).toObject();
  if( !obj.isEmpty() )
    addItem( obj, id );
  }



//Remove object's child id's
void SdWCategoryList::removeAllChild(int id)
  {
  //Retrive object to delete
  QJsonObject obj = mCategoryMap.value( QString::number(id) ).toObject();
  //Remove object from map
  mCategoryMap.remove( QString::number(id) );
  //Get array of child
  QJsonArray arr = obj.value( CAT_CHILD ).toArray();
  //Remove all child objects
  for( auto iter = arr.constBegin(); iter != arr.constEnd(); iter++ )
    removeAllChild( iter->toInt() );
  }



void SdWCategoryList::mousePressEvent(QMouseEvent *event)
  {
  if( event->button() == Qt::RightButton ) {
    QListWidget::mousePressEvent( event );
    //On right button display context menu
    if( menuCategory == nullptr ) {
      menuCategory = new QMenu( tr("Category") );
      //Category manipulations
      menuCategory->addAction( QIcon(QString(":/pic/objectNew.png")), tr("Add new category"), this, &SdWCategoryList::cmAppend );
      menuCategory->addAction( QIcon(QString(":/pic/objectRename.png")), tr("Edit title..."), this, &SdWCategoryList::cmEditTitle );
      menuCategory->addAction( QIcon(QString(":/pic/objectParam.png")), tr("Edit category key..."), this, &SdWCategoryList::cmEditKey );
      menuCategory->addAction( QIcon(QString(":/pic/objectDelete.png")), tr("Delete category"), this, &SdWCategoryList::cmRemove );
      menuCategory->addAction( QIcon(QString(":/pic/objectCut.png")), tr("Cut"), this, &SdWCategoryList::cmCut );
      cmdPaste = menuCategory->addAction( QIcon(QString(":/pic/objectPaste.png")), tr("Paste"), this, &SdWCategoryList::cmPaste );
      menuCategory->addSeparator();

      //Load-save
      menuCategory->addAction( QIcon(QString(":/pic/fileOpen.png")), tr("Load category tree..."), this, &SdWCategoryList::cmLoad );
      menuCategory->addAction( QIcon(QString(":/pic/save.png")), tr("Save category tree"), this, &SdWCategoryList::cmSave );
      menuCategory->addAction( QIcon(QString(":/pic/save_as.png")), tr("Save category tree as..."), this, &SdWCategoryList::cmSaveAs );
      }
    //When no clipboard data we disabled paste cmd
    cmdPaste->setDisabled( clipboardId );
    menuCategory->exec( QCursor::pos() );
    }
  else
    //On other buttons - default
    QListWidget::mousePressEvent( event );
  }
