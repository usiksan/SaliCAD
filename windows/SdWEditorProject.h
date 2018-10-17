/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Project view in editors tab workspace
*/
#ifndef SDWEDITORPROJECT_H
#define SDWEDITORPROJECT_H

#include "objects/SdProject.h"
#include "SdWEditor.h"

#include <QTextBrowser>

class SdWEditorProject : public SdWEditor
  {
    Q_OBJECT

    SdProject     *mProject;   //Project which view
    QString        mShortName; //Project short name - file name without extension and without path
    QTextBrowser  *mTitle;     //Project title - visual representation
  public:
    SdWEditorProject( SdProject *prj, const QString shortName );



    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual SdProject     *getProject() override;
    virtual QString        getTitle() override;
    virtual QString        getToolTip() override;
    virtual QString        getIconName() override;
    virtual void           onActivateEditor() override;
    virtual void           cmEditUndo() override;
    virtual void           cmEditRedo() override;

  public slots:
    //When project renamed we must rebuild project visual representation
    void onProjectRename( SdProject *prj, const QString shortName );

    //Parse click in visual representation
    void onLinkClick( QUrl url );

  private:
    void visualBuild();
  };

#endif // SDWEDITORPROJECT_H
