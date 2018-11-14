/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget to display and navigate on category hierarchy

  One category is QJsonObject with
    parent - category parent id. Id is id which child contains this category
    name   - category name. It is that we see in QListWidget
    key    - category key. It insert to filtr
    child  - array of id's which child to this category

  All category objects contained in mCategory object with id map
*/
#ifndef SDWCATEGORYLIST_H
#define SDWCATEGORYLIST_H

#include <QListWidget>
#include <QJsonObject>
#include <QMenu>
#include <QAction>

class SdWCategoryList : public QListWidget
  {
    Q_OBJECT

    static QJsonObject     mCategoryMap;   //List of all categories
    static int             mCurrentId;     //Current category id

    static QMenu   *menuCategory;   //Context category menu
    static QAction *cmdPaste;       //Paste from clipboard command
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

    //Edit category title
    void cmEditTitle();

    //Edit category key
    void cmEditKey();

    //Cut category from list
    void cmCut();

    //Paste category to list
    void cmPaste();

    //Load categories from file
    void cmLoad();

    //Save categories to file
    void cmSave();

    //Save categories to file with other file name
    void cmSaveAs();

    //On select category in list
    void cmCategory( QListWidgetItem *item );

    //On double click category
    void cmEnter( QListWidgetItem *item );

  private:
    //Fill list with contens of current category path
    void fill();

    //Add item to visual category list
    void addItemInt( bool dir, const QString title, const QString key, int id );

    //Add item to visual category list from category object
    void addItem(const QJsonObject &obj, int id);

    //Add item to visual category list by its id
    void addItemById( int id );

    //Remove object's child id's
    void removeAllChild( int id );

    // QWidget interface
  protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
  };

#endif // SDWCATEGORYLIST_H
