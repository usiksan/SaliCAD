/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdWMain - Main application window
*/

#ifndef SBWMAIN_H
#define SBWMAIN_H

#include "SdConfig.h"
#include "SdWProjectList.h"
#include "SdWEditor.h"
#include "SdWLabel.h"
#include "SdWHelp.h"

#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QStringList>
#include <QClipboard>
#include <QLabel>

class SdProjectItem;
class SdWProjectTree;

class SdWMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter      *mWSplitter;     //Central part of programm windows - is splitter with projects and redactors
    SdWProjectList *mWProjectList;  //Project list
    QTabWidget     *mWEditors;      //Actived visual editors
    SdWHelp        *mWHelp;         //Edge help

    //Status bar infos
    SdWLabel       *mXLabel;        //X axiz title (X or col)
    SdWLabel       *mXPos;          //X axiz position
    SdWLabel       *mYLabel;        //Y axiz title (Y or row)
    SdWLabel       *mYPos;          //Y axiz position
    SdWLabel       *mMessage;       //Message
  public:
    explicit SdWMain( QStringList args, QWidget *parent = nullptr );

  signals:

  public slots:
    //Status bar
    void setStatusLabels( const QString xlabel, const QString ylabel );
    void setStatusPositions( const QString x, const QString y );
    void setStatusMessage( const QString msg );

    void activateProjectName( const QString name, bool dirty );

    void onActivateProjectItem( SdProjectItem *item );
    void onCloseEditView( SdProjectItem *item );
    void onUpdateItemTitle( SdProjectItem *item );
    void onRemoveProjectItem( SdProjectItem *item );
    void onCloseProject( SdProject *prj );

    void onClipboardChanged( QClipboard::Mode mode );

    //Browse part in sheet
    void onBrowseSheetPart( SdProjectItem *sheet, SdProjectItem *plate );
    //Components, selected from sheet
    void onSelectedParts( SdProjectItem *plate, QStringList list );



    //Calling when press tab of editor
    void onActivateEditor( int index );
    //Calling when press close tab of editor
    void onCloseEditor( int index );

    //Реакции на команды
    void cmFileNew();
    void cmFileLoad();
    void cmFileOpen();
    void cmFilePrevious();
    void cmFileClose();
    void cmFileCloseAll();

    void cmFileSave();
    void cmFileSaveAs();
    void cmFileSaveAll();
    void cmFileStore();
    void cmFilePrint();
    void cmFileExit();

    void cmFileImportPis();
    void cmFileImport();
    void cmFileExport();

    void cmObjectNew();
    void cmObjectLoad();
    void cmObjectRename();
    void cmObjectCategory();
    void cmObjectDelete();
    void cmObjectCopy();
    void cmObjectPaste();
    void cmObjectCut();
    void cmObjectSort();
    void cmObjectParam();
    void cmObjectEditEnable();
    void cmObjectEditDisable();
    void cmProjectParam();

    void cmEditUndo();
    void cmEditRedo();
    void cmEditCut();
    void cmEditCopy();
    void cmEditPaste();
    void cmEditDelete();
    void cmEditSelectAll();
    void cmEditUnSelect();
    void cmEditFind();
    void cmEditReplace();
    void cmEditProperties();

    void cmViewProject();
    void cmViewFill();
    void cmViewNets();
    void cmViewGrid();
    void cmViewLayers();
    void cmViewZoomIn();
    void cmViewZoomOut();
    void cmViewArea();
    void cmViewMeasurement();
    void cmShowFields( bool st );

    void cmModeSelect();

    void cmModeLine();
    void cmModeRect();
    void cmModeFilledRect();
    void cmModeRegion();
    void cmModeFilledRegion();
    void cmModeCircle();
    void cmModeFilledCircle();
    void cmModeArc();
    void cmModeText();
    void cmModeField();

    //3d
    void cmBall();
    void cmPinWired();
    void cmPinFlat();
    void cmBodyCylinder();
    void cmBodyBrick();
    void cmBodyContur();

    //Symbol Part
    void cmModePin();
    void cmModeReference();
    void cmModeOrigin();
    void cmModeValue();

    //Sheet and pcb
    void cmRenumeration();
    void cmModeReferenceMove();
    void cmModeValueMove();

    //Sheet
    void cmModeFragment();
    void cmModeComponent();
    void cmModeNet();
    void cmNetSetup();
    void cmModeBus();
    void cmModeDisconnect();
    void cmModePcbArea();
    void cmModeLink();
    void cmModeNetName();
    void cmModeNetList();
    void cmModePack();

    //Pcb
    void cmShowRatNet(bool st);
    void cmShowRuleErrors(bool st);
    void cmPads();
    void cmModeLineSize();
    void cmModeRadiusSize();
    void cmModeMovePart();
    void cmModePartSelect( QStringList list );

    void cmModeEditWire();
    void cmModeRoadEnter();
    void cmModePolygon();
    void cmModeDeleteWire();
    void cmModePad();

    void cmRulesEdit();
    void cmRulesCheck();
    void cmRulesErrorNext();

    void cmOption();
    void cmTools();

    void cmHelpContens();
    void cmHelpIndex();
    void cmHelpAbout();
    void cmHelpRegistration();

    void cmClipboardChange();
    void cmPropertiesChange();

    void cmEnterPosition();

    void cmHelpTopic( const QString topic );

    // QWidget interface
  protected:
    virtual void closeEvent(QCloseEvent *ev) override;

  private:
    SdWEditor*      getEditor( int index );
    SdWEditor*      activeEditor();
    //Return help widget in editor area. If none it created
    SdWEditor*      helpWidget();
    //Find help widget in editor area. Return nullptr if none
    SdWEditor*      findHelpWidget();
    SdWProjectTree *activeProject() { return mWProjectList->activeProject(); }
    SdWProjectTree *project( int index ) { return mWProjectList->project(index); }
    void            createMenu();
    void            destroyProject( SdWProjectTree *prj );
    void            setupTitle();
  };

#endif // SBWMAIN_H
