/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdWCategory.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdUtil.h"

#include <QMouseEvent>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

SdWCategory::SdWCategory(QWidget *parent) :
  QTreeWidget(parent)
  {
  //Fill category list with items
  setColumnCount(1);
  addTopLevelItems( SdObjectFactory::hierarchyGet(QString("root")) );

  connect( this, &SdWCategory::currentItemChanged, this, &SdWCategory::onCurrentItemChanged );
  setHeaderHidden(true);
  }



//Select and activate this category
void SdWCategory::setCategory(const QString category)
  {
  //Find category
  QTreeList list = findItems( category, Qt::MatchFixedString, 1 );
  //If found then select first item
  if( !list.empty() )
    setCurrentItem( list.at(0) );
  }


//Add category to root item
void SdWCategory::addCategory2Root()
  {
  QString category = QInputDialog::getText( this, tr("Enter category name in English"), tr("Category") );

  if( !category.isEmpty() ) {
    //Test if name in English
    if( !SdUtil::isEnglish( category ) ) {
      QMessageBox::warning( this, tr("Error!"), tr("Category must be in English. After you may translate it to your language.") );
      return;
      }

    //Item category entered successfully

    //Add category to database
    SdObjectFactory::hierarchyAddItem( QString("root"), category );
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText( 0, category );
    item->setText( 1, category );
    addTopLevelItem( item );
    }
  }




//Append new category to item
void SdWCategory::addCetagory2Item()
  {
  QTreeWidgetItem *cur = currentItem();
  if( cur != nullptr ) {
    QString category = QInputDialog::getText( this, tr("Enter category name for '%1' in English").arg(cur->text(0)), tr("Category") );

    if( !category.isEmpty() ) {
      //Test if name in English
      if( !SdUtil::isEnglish( category ) ) {
        QMessageBox::warning( this, tr("Error!"), tr("Category must be in English. After you may translate it to your language.") );
        return;
        }

      //Item category entered successfully
      //Check if category in database already
      if( SdObjectFactory::hierarchyIsPresent(category) )
        //Category present. Show query to replace category
        if( QMessageBox::warning( this, tr("Warning!"), tr("Category already exist. Replace it?"), QMessageBox::Yes | QMessageBox::No ) != QMessageBox::Yes )
          return;

      //Add category to database
      SdObjectFactory::hierarchyAddItem( cur->text(1), category );
      QTreeWidgetItem *item = new QTreeWidgetItem();
      item->setText( 0, category );
      item->setText( 1, category );
      cur->addChild( item );
      }
    }
  }




//Rename (translate) current category
void SdWCategory::translateCategory()
  {
  QTreeWidgetItem *cur = currentItem();
  if( cur != nullptr ) {
    QString category = QInputDialog::getText( this, tr("Enter category translation for '%1'").arg(cur->text(0)), tr("Category") );

    if( !category.isEmpty() ) {
      //Item category entered successfully

      //Change translation in database
      SdObjectFactory::hierarchyTranslate( cur->text(1), category );
      cur->setText( 0, category );
      }
    }
  }





void SdWCategory::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
  {
  Q_UNUSED(previous )
  if( current ) {
    //Send category name change
    emit categorySelected( current->text(0) );

    //Send tag path
    emit tagPathSelected( current->text(1)  );
    }
  }



#if 0
//Rename current category
void SdWCategory::renameCategory()
  {
  QTreeWidgetItem *cur = currentItem();
  if( cur != nullptr ) {
    QString category = QInputDialog::getText( this, tr("Enter new category for '%1'").arg(cur->text(1)), tr("Category") );

    if( !category.isEmpty() ) {
      //Test if name in English
      if( !SdUtil::isEnglish( category ) ) {
        QMessageBox::warning( this, tr("Error!"), tr("Category must be in English. After you may translate it to your language.") );
        return;
        }

      //Item category entered successfully

      //Rename category in database
      SdObjectFactory::hi
      SdObjectFactory::hierarchyAddItem( cur->text(1), category );
      QTreeWidgetItem *item = new QTreeWidgetItem();
      item->setText( 0, category );
      item->setText( 1, category );
      cur->addChild( item );
      }
    }
  }
#endif




void SdWCategory::mousePressEvent(QMouseEvent *event)
  {
  //Default reaction
  QTreeWidget::mousePressEvent( event );

  //On right button show context menu
  if( event->button() == Qt::RightButton ) {
    //Show context menu
    QMenu menu( tr("Category menu") );
    menu.addAction( tr("Add to root category"), this, &SdWCategory::addCategory2Root );
    QTreeWidgetItem *cur = currentItem();
    if( cur != nullptr ) {
      menu.addAction( tr("Add to '%1' category").arg(cur->text(0)), this, &SdWCategory::addCetagory2Item );
      menu.addAction( tr("Translate '%1' category").arg(cur->text(0)), this, &SdWCategory::translateCategory );
      }
    menu.exec( QCursor::pos() );
    }
  }

