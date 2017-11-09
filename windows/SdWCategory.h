/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDWCATEGORY_H
#define SDWCATEGORY_H

#include <QWidget>
#include <QTreeWidget>

class SdWCategory : public QTreeWidget
  {
    Q_OBJECT
  public:
    explicit SdWCategory(QWidget *parent = nullptr);

  signals:
    //Send when user select any category
    void categorySelected( const QString category );

    //Send when user select any category
    void tagPathSelected( const QString path );

  public slots:
    //Select and activate this category
    void setCategory( const QString category );

    //Add category to root item
    void addCategory2Root();

    //Append new category to item
    void addCetagory2Item();

    //Translate current category
    void translateCategory();

    //Rename current category
    //void renameCategory();

    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous );

    // QWidget interface
  protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

  };

#endif // SDWCATEGORY_H
