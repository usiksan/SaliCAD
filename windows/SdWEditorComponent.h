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
#include <QListWidget>
#include <QTableWidget>
#include <QSet>


class SdWEditorComponent : public SdWEditor
  {
    Q_OBJECT

    SdPItemComponent     *mComponent;         //Editing component
    SdUndo               *mUndo;              //Undo for editing

    SdWEditorGraphView   *mSymbolViewer;      //View for symbol sections
    SdWEditorGraphView   *mPartViewer;        //View for part of this component

    QListWidget          *mSectionList;       //Sections of component
    QTableWidget         *mPackTable;         //Packing table
    QPushButton          *mSectionAdd;
    QPushButton          *mSectionDubl;
    QPushButton          *mSectionSymbol;
    QPushButton          *mSectionDelete;
    QPushButton          *mSectionDeleteAll;
    QPushButton          *mPackSelect;        //Shows dialog with available pin numbers

    QLineEdit            *mPart;
    QPushButton          *mPartSelect;

    QTableWidget         *mParamTable;        //Table of user params
    QPushButton          *mParamAdd;
    QPushButton          *mParamAddDefault;
    QPushButton          *mParamDelete;
    QPushButton          *mParamCopy;

    SdStringMap           mPackNumbers;
    QSet<QString>         mDuplicateNumbers;

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

    void onPackChanged( int row, int column );
    void onParamChanged( int row, int column );

    void partSelect();

    void paramAdd();
    void paramAddDefault();
    void paramDelete();
    void paramCopy();
  private:
    void fillSections();
    void fillPart();
    void fillParams();
    void fillUsedPins();
    void paramAppend(int row, const QString key, const QString value );
    void paramAddInt( const QString key, const QString value = QString() );

    QString packetPin( int section, const QString pinName );

    // SdWEditor interface
  public:
    virtual void cmEditUndo() override;
    virtual void cmEditRedo() override;
  };

#endif // SDWEDITORCOMPONENT_H
