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
    ~SdWEditorGraphView();

    //Set new item
    void setItem(SdProjectItem *item , bool owner);

    //Set new item by item name and item author
    void setItemByNameAndAuthor( const QString name, const QString author );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;
  };

#endif // SDWEDITORGRAPHVIEW_H
