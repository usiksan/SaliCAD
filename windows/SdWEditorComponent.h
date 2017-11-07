/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Visual Editor for component
*/
#ifndef SDWEDITORCOMPONENT_H
#define SDWEDITORCOMPONENT_H

#include "SdWEditor.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdUndo.h"
#include "windows/SdWEditorGraphView.h"

#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>


class SdWEditorComponent : public SdWEditor
  {
    Q_OBJECT

    SdPItemComponent     *mComponent;         //Editing component
    SdUndo               *mUndo;              //Undo for editing

    SdWEditorGraphView   *mSymbolViewer;      //View for symbol sections
    SdWEditorGraphView   *mPartViewer;        //View for part of this component

    QTabWidget           *mSectionsTab;       //Sections of component
    QPushButton          *mSectionAdd;
    QPushButton          *mSectionDubl;
    QPushButton          *mSectionSelect;
    QPushButton          *mSectionDelete;
    QPushButton          *mSectionDeleteAll;

    QLineEdit            *mPart;
    QPushButton          *mPartSelect;
    QPushButton          *mPartDelete;

    bool                  mAnotherAuthor;   //True if component crated another author and edit prohibited
  public:
    SdWEditorComponent(SdPItemComponent *comp, QWidget *parent);

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;

  public slots:
    void sectionAdd();
    void sectionDubl();
    void sectionSelect();
    void sectionDelete();
    void sectionDeleteAll();
    void onCurrentSection( int index );

    void partSelect();
    void partDelete();
  private:
    void fillSections();
    void renameSymbolTabs();
    void fillPart();
  };

#endif // SDWEDITORCOMPONENT_H
