/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Visual Editor for inheritance
*/
#ifndef SDWEDITORINHERITANCE_H
#define SDWEDITORINHERITANCE_H

#include "SdWEditor.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemInheritance.h"
#include "objects/SdUndo.h"
#include "windows/SdWEditorGraphView.h"

#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>


class SdWEditorInheritance : public SdWEditor
  {
    Q_OBJECT

    SdPItemInheritance   *mInheritance;         //Editing inheritance
    SdPItemComponent     *mComponent;           //Base component
    SdUndo               *mUndo;                //Undo for editing

    SdWEditorGraphView   *mSymbolViewer;        //View for symbol sections
    SdWEditorGraphView   *mPartViewer;          //View for part of this component

    QLineEdit            *mComponentTitle;      //Title of component for this inheritance
    QPushButton          *mComponentSelect;     //Select component for this inheritance

    QTableWidget         *mComponentParamTable; //Table of user params of component
    QPushButton          *mParamInherit;        //Inherit current component param to inheritance
    QPushButton          *mParamInheritAll;     //Inherit all component params to inheritance
    QTableWidget         *mParamTable;          //Table of user params of inheritance
    QPushButton          *mParamAdd;
    QPushButton          *mParamAddDefault;
    QPushButton          *mParamDelete;
    QPushButton          *mParamCopy;
    QPushButton          *mParamVariant;
  public:
    SdWEditorInheritance(SdPItemInheritance *inh, QWidget *parent);
    ~SdWEditorInheritance() override;

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;
    virtual void           cmEditUndo() override;
    virtual void           cmEditRedo() override;

  public slots:
    void onParamChanged( int row, int column );

    void componentSelect();

    void paramInherit();
    void paramInheritAll();

    void paramAdd();
    void paramAddDefault();
    void paramDelete();
    void paramCopy();
    void paramVariant();

  private:
    void fillComponent();
    void fillParams();
    void paramAppend(int row, const QString key, const QString value );
    void paramAddInt( const QString key, const QString value = QString() );
  };

#endif // SDWEDITORINHERITANCE_H
