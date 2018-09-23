/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDWPROJECTTREE_H
#define SDWPROJECTTREE_H

#include "SdConfig.h"
#include "objects/SdProject.h"
#include "objects/SdProjectItem.h"
#include <QWidget>
#include <QTreeWidget>

class SdWProjectTree : public QTreeWidget
  {
    Q_OBJECT

    SdProject           *mProject;

    QTreeWidgetItem     *mSymbolList;    //Схемные изображения компонентов
    QTreeWidgetItem     *mPartList;      //Корпуса компонентов
    QTreeWidgetItem     *mComponentList; //Компоненты (схемное избр. + корпус + параметры)
    QTreeWidgetItem     *mSheetList;     //Листы схем
    QTreeWidgetItem     *mPlateList;     //Печатные платы
    QTreeWidgetItem     *mTextList;      //Текстовые документы

    QString              mFileName;      //Полное имя файла проекта проекта с путем
  public:
    explicit SdWProjectTree( const QString fname, SdProject *prj, QWidget *parent = nullptr);
    ~SdWProjectTree() override;

    QString      filePath() { return mFileName; }
    QString      fileName();
    SdProject   *getProject() const { return mProject; }

    bool         isProjectValid() const;

    bool         cmFileClose();

    bool         cmFileSave();
    bool         cmFileSaveAs();


    //Create new object in project hierarchy with wizard
    void         cmObjectNew();

    //Load object from library
    void         cmObjectLoad();

    //Rename object or/and change category
    void         cmObjectRename();

    //Delete object
    void         cmObjectDelete();

    //Copy object to clipboard
    void         cmObjectCopy();

    //Paste object from clipboard
    void         cmObjectPaste();

    //Cut object to clipboard
    void         cmObjectCut();

    //Duplicate current object
    void         cmObjectDuplicate();

    //Sort object list by alphabet
    void         cmObjectSort();

    //Edit object param
    void         cmObjectParam();

    //Edit project param
    void         cmProjectParam();



    void         cmClipboardChange();

    void         cmEditUndo();
    void         cmEditRedo();

    //Update status undo and redo commands
    void         cmUndoRedoUpdate();

  public slots:
    //AFTER rename item
    void renameItem( SdProjectItem *item );
    //AFTER insert item
    void insertItem( SdProjectItem *item );
    //BEFORE remove item
    void removeItem( SdProjectItem *item );
    //When editor activated send this signal
    //on it we highlight appropriate line in project tree
    void highlightItem( SdProjectItem *item );

    void onCurrentItemChanged( QTreeWidgetItem *cur, QTreeWidgetItem *prev );

  protected:
    virtual void      showEvent( QShowEvent *event ) override;

  private:
    //Create object node [Создать элемент дерева проекта]
    QTreeWidgetItem*  createItem( const QString sname, const QString stoolTip, const QString sstatusTip );

    //Fill node of top level [Заполнить элемент верхнего уровня]
    void              fillTopItem(QTreeWidgetItem *item, quint64 classId );

    //Build visual tree
    void              buildVisualTree();

    //Object class node
    QTreeWidgetItem*  classList( quint64 classId );

    //Duplicate object with id
    void              duplicate(const QString &uid );

    // QWidget interface
  protected:
    //Reimplement to display context menu
    virtual void      mousePressEvent(QMouseEvent *event) override;
  };

#endif // SDWPROJECTTREE_H
