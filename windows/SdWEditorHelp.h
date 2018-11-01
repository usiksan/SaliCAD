/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Help system in editor space
*/
#ifndef SDWEDITORHELP_H
#define SDWEDITORHELP_H

#include "SdWEditor.h"
#include "SdWHelp.h"

class SdWMain;

class SdWEditorHelp : public SdWEditor
  {
    Q_OBJECT

    SdWHelp *mHelp; //Help widget fill full editor space
  public:
    SdWEditorHelp( QWidget *parent = nullptr);

    void helpTopic( const QString topic ) { mHelp->helpTopic(topic); }

    void helpIntro( SdWMain *main ) { mHelp->helpIntro( main ); }

    void helpBackward();

    void helpForward();

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;
  };

#endif // SDWEDITORHELP_H
