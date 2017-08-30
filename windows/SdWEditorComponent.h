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

#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>

class SdWEditorGraphSymbol;
class SdWEditorGraphPart;

class SdWEditorComponent : public SdWEditor
  {
    Q_OBJECT

    SdPItemComponent     *mComponent;
    SdUndo               *mUndo;

    SdWEditorGraphSymbol *mSymbolViewer;
    SdWEditorGraphPart   *mPartViewer;

    QTabWidget           *mSectionsTab;
    QPushButton          *mSectionAdd;
    QPushButton          *mSectionDubl;
    QPushButton          *mSectionSelect;
    QPushButton          *mSectionDelete;
    QPushButton          *mSectionDeleteAll;

    QListWidget          *mPartTable;
    QPushButton          *mPartAdd;
    QPushButton          *mPartSelect;
    QPushButton          *mPartDelete;
    QPushButton          *mPartDefault;
    QPushButton          *mPartDeleteAll;

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

    void partAdd();
    void partSelect();
    void partDelete();
    void partDefault();
    void partDeleteAll();
    void onCurrentPart( int index );
  private:
    void fillSections();
    void renameSymbolTabs();
    void fillParts();
    SdPartVariant *getPartVariant( int index );
  };

#endif // SDWEDITORCOMPONENT_H
