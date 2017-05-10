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

    QWidget              *mPartTable;
    QPushButton          *mPartAdd;
    QPushButton          *mPartSelect;
    QPushButton          *mPartDelete;
  public:
    SdWEditorComponent(SdPItemComponent *comp, QWidget *parent);

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() override;
    virtual void           onActivateEditor() override;

  public slots:
    void sectionAdd();
    void sectionDubl();
    void sectionSelect();
    void sectionDelete();
  private:
    void fillSections();
    void renameSymbolTabs();
  };

#endif // SDWEDITORCOMPONENT_H
