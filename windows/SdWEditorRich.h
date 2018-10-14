/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rich text object editor.
*/
#ifndef SDWEDITORRICH_H
#define SDWEDITORRICH_H

#include "SdWEditor.h"
#include "objects/SdPItemRich.h"

#include <QTextEdit>

class SdWEditorRich : public SdWEditor
  {
    Q_OBJECT

    SdPItemRich  *mRich;
    QTextEdit    *mTextEdit;
    bool          mDirty;
  public:
    SdWEditorRich(SdPItemRich *item, QWidget *parent = nullptr );
    ~SdWEditorRich() override;

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;

  public slots:
    //On text changed
    void onTextChanged();
  };

#endif // SDWEDITORRICH_H
