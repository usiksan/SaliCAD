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

#include <QMouseEvent>
#include <QMenu>
#include <QInputDialog>

SdWCategory::SdWCategory(QWidget *parent) :
  QTreeWidget(parent)
  {
  //Fill category list with items
  setColumnCount(1);
  addTopLevelItems( SdObjectFactory::hierarchyGet(QString("root")) );
  }



//Select and activate this category
void SdWCategory::setCategory(const QString category)
  {
  //Find category
  }


//Add category to root item
void SdWCategory::addCategory2Root()
  {
  QString category = QInputDialog::getText( this, tr("Enter category name"), tr("Category") );
  if( !category.isEmpty() ) {
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
    QString category = QInputDialog::getText( this, tr("Enter category name for '%1'").arg(cur->text(0)), tr("Category") );

    if( !category.isEmpty() ) {
      //Item category entered successfully

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
      menu.addAction( tr("Rename '%1' category").arg(cur->text(0)), this, &SdWCategory::translateCategory );
      }
    menu.exec( QCursor::pos() );
    }
  }

