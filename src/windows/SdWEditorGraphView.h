/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget for viewing any object
*/
#ifndef SDWEDITORGRAPHVIEW_H
#define SDWEDITORGRAPHVIEW_H

#include "SdWEditorGraph.h"

class SdWEditorGraphView : public SdWEditorGraph
  {
    Q_OBJECT

    SdProjectItem *mItem;  //Item for view
    bool           mOwner; //True if widget is owner of item and must delete if unused
  public:
    SdWEditorGraphView( SdProjectItem *item, QWidget *parent );
    SdWEditorGraphView(QWidget *parent);
    ~SdWEditorGraphView() override;

    //Set new item
    void setItem(SdProjectItem *item , bool owner);

    //Set new item by item id
    void setItemById( const QString id );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;

    //Export command
    virtual void           cmFileExport() override;

    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;

    // SdWEditorGraph interface
  public:
    virtual bool showFields() override;
  };

#endif // SDWEDITORGRAPHVIEW_H
