/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget to display and navigate on category hierarchy
  Category hieararchy is QJsonObject.
  Each object contains key path for category

  One category is QJsonObject with
    parent - category parent id. Id is id which child contains this category
    name   - category name. It is that we see in QListWidget
    key    - category key. It insert to filtr
    child  - array of id's which child to this category

  All category objects contained in mCategory object with id map
*/
#ifndef SDWCATEGORYLIST_H
#define SDWCATEGORYLIST_H

#include <QTreeWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QMenu>
#include <QAction>
#include <QMap>

class SdWCategoryList : public QTreeWidget
  {
    Q_OBJECT

    QMap<QString,QTreeWidgetItem*> mItemMap;

    static QJsonArray mCategoryMap;   //List of all categories
  public:
    SdWCategoryList( QWidget *parent );
    ~SdWCategoryList() override;

  signals:
    //Send when selected new category to find
    void category( const QString key );

  public slots:
    //Append new category
    void cmAppend();

    //Remove category
    void cmRemove();

    //Delete all categories
    void cmClear();

    //Edit category title
    void cmEditTitle();

    //Edit category key
    void cmEditKey();

    //Cut category from list
    //void cmCut();

    //Paste category to list
    //void cmPaste();

    //Load categories from file
    void cmLoad();

    //Save categories to file
    //void cmSave();

    //Save categories to file with other file name
    void cmSaveAs();

    //On select category in tree
    void cmCategory( QTreeWidgetItem *item, int column );

    //Locate category
    void cmLocate( const QString path );

    //On double click category
    //void cmEnter( QTreeWidgetItem *item );

  private:
    //Fill list with contents of current category path
    void fill();

    //Build item from its description
    QTreeWidgetItem *buildItem( bool dir, const QString title, const QString key, const QString parent );

    //Build item and its children on json object
    QTreeWidgetItem *buildItem(const QJsonObject &obj , const QString parent);

    //Retrive info from tree item build json object
    QJsonObject      scanItem( QTreeWidgetItem *item );

    // QWidget interface
  protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
  };

#endif // SDWCATEGORYLIST_H
