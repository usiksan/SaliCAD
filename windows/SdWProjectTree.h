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

    QTreeWidgetItem     *mSymbolList;   //Схемные изображения компонентов
    QTreeWidgetItem     *mPartList;     //Корпуса компонентов
    QTreeWidgetItem     *mAliasList;    //Компоненты (схемное избр. + корпус + параметры)
    QTreeWidgetItem     *mSheetList;    //Листы схем
    QTreeWidgetItem     *mPlateList;    //Печатные платы
    QTreeWidgetItem     *mTextList;     //Текстовые документы

    QString              mFileName;     //Полное имя файла проекта проекта с путем
  public:
    explicit SdWProjectTree( const QString fname, QWidget *parent = 0);
    ~SdWProjectTree();

    QString      filePath() { return mFileName; }
    QString      fileName();
    SdProject   *getProject() const { return mProject; }

    bool         isProjectValid() const;

    bool         cmFileClose();

    bool         cmFileSave();
    bool         cmFileSaveAs();

    void         cmObjectNew();
    void         cmObjectRename();
    void         cmObjectDelete();
    virtual void cmObjectCopy() {}
    virtual void cmObjectPaste() {}
    virtual void cmObjectCut() {}
    virtual void cmObjectSort() {}

    void         cmClipboardChange();

    void         cmEditUndo();
    void         cmEditRedo();

  public slots:
    //AFTER rename item
    void renameItem( SdProjectItem *item );
    //AFTER insert item
    void insertItem( SdProjectItem *item );
    //BEFORE remove item
    void removeItem( SdProjectItem *item );

    void onCurrentItemChanged( QTreeWidgetItem *cur, QTreeWidgetItem *prev );

  protected:
    virtual void showEvent( QShowEvent *event );

  private:
    //Создать элемент дерева проекта
    QTreeWidgetItem* createItem( const QString sname, const QString stoolTip, const QString sstatusTip );

    //Заполнить элемент верхнего уровня
    void fillTopItem( QTreeWidgetItem *item, int classId );

    QTreeWidgetItem* classList( quint64 classId );
  };

#endif // SDWPROJECTTREE_H
