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
    SdWEditorGraphView(QWidget *parent);

    //Set new item
    void setItem(SdProjectItem *item , bool owner);

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() override;

    // SdWEditorGraph interface
  public:
    virtual double getPPM() const override;
  };

#endif // SDWEDITORGRAPHVIEW_H