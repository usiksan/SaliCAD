/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Intro page with previous files list, help and guide access and some fast commands
*/
#ifndef SDWEDITORINTRO_H
#define SDWEDITORINTRO_H


#include "SdWEditor.h"
#include "SdWHelp.h"

class SdWMain;


class SdWEditorIntro : public SdWEditor
  {
    Q_OBJECT

    SdWHelp *mHelp; //Help widget fill full editor space
  public:
    SdWEditorIntro( SdWMain *main, QWidget *parent = nullptr );

    //void helpIntro( SdWMain *main ) { mHelp->helpIntro( main ); }

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;
  };

#endif // SDWEDITORINTRO_H
