/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  This file contains list of all gui commands and toolBars
*/

#ifndef SDWCOMMAND_H
#define SDWCOMMAND_H

#include "SdConfig.h"
#include "modes/SdModeIdents.h"
#include <QAction>
#include <QMenu>
#include <QToolBar>

typedef QAction *QActionPtr;
typedef QToolBar *QToolBarPtr;

class SdWMain;
class SdPropBar;

class SdWCommand
  {
  public:
    //static QMenu *
    static QMenu *menuFile;
    static QMenu *menuFilePrevious;
    static QMenu *menuObject;
    static QMenu *menuEdit;
    static QMenu *menuView;
    static QMenu *menuViewPart3d;
    static QMenu *menuDraw;
    static QMenu *menuInsertSymbol;
    static QMenu *menuInsertSheet;
    static QMenu *menuInsertPart;
    static QMenu *menuInsertPart3d;
    static QMenu *menuInsertPcb;
    static QMenu *menuInsertComp;
    static QMenu *menuInstruments;
    static QMenu *menuRules;
    static QMenu *menuHelp;

    static QMenu *menuSelect;

    static QActionPtr cmMenuInsertSymbol;
    static QActionPtr cmMenuInsertSheet;
    static QActionPtr cmMenuInsertPart;
    static QActionPtr cmMenuInsertPart3d;
    static QActionPtr cmMenuInsertPcb;
    static QActionPtr cmMenuInsertComp;
    static QActionPtr cmMenuRules;
    static QActionPtr cmMenuViewPart3d;


    //Tool bars for editor command
    static QToolBar *barMain;
    static QToolBar *barSymbol;
    static QToolBar *barPart;
    static QToolBar *barPart3d;
    static QToolBar *barPart3dView;
    static QToolBar *barComp;
    static QToolBar *barSheet;
    static QToolBar *barPcb;
    static QToolBar *barView;
    static QToolBar *barHelp;

    static void addEditCommands( QToolBar *bar );
    static void addViewCommands( QToolBar *bar );
    static void addDrawCommands( QToolBar *bar );



    static QActionPtr  cmFileNew;
    static QActionPtr  cmFileOpen;
    static QActionPtr  cmFileClose;
    static QActionPtr  cmFileCloseAll;
    static QActionPtr  cmFileSave;
    static QActionPtr  cmFileSaveAs;
    static QActionPtr  cmFileSaveAll;
    static QActionPtr  cmFilePrint;
    static QActionPtr  cmFileExit;
    static QActionPtr  cmFileImportPis;
    static QActionPtr  cmFileImport;
    static QActionPtr  cmFileExport;
    static QActionPtr  cmFileExportGerber;
    static QActionPtr  cmFileStoreToLibrary;
    static QActionPtr  cmFileLoadFromLibrary;
    static QActionPtr  cmFileCloud;

    static QActionPtr  cmFilePrevious[PREVIOUS_FILES_COUNT];

    static QActionPtr  cmObjectNew;
    static QActionPtr  cmObjectLoad;
    static QActionPtr  cmObjectRename;
    static QActionPtr  cmObjectDelete;
    static QActionPtr  cmObjectCopy;
    static QActionPtr  cmObjectPaste;
    static QActionPtr  cmObjectCut;
    static QActionPtr  cmObjectDuplicate;
    static QActionPtr  cmObjectSort;
    static QActionPtr  cmObjectParam;
    static QActionPtr  cmObjectEditEnable;
    static QActionPtr  cmObjectEditDisable;
    static QActionPtr  cmProjectParam;
    static QActionPtr  cmProjectUpgrade;

    static QActionPtr  cmEditUndo;
    static QActionPtr  cmEditRedo;
    static QActionPtr  cmEditCut;
    static QActionPtr  cmEditCopy;
    static QActionPtr  cmEditCopySvg;
    static QActionPtr  cmEditCopyWeb;
    static QActionPtr  cmEditPaste;
    static QActionPtr  cmEditDelete;
    static QActionPtr  cmEditSelectAll;
    static QActionPtr  cmEditUnSelect;
    static QActionPtr  cmEditSelectSave;
    static QActionPtr  cmEditFind;
    static QActionPtr  cmEditReplace;
    static QActionPtr  cmEditRotateGroup;
    static QActionPtr  cmEditProperties;
    static QActionPtr  cmEditCalculations;
    static QActionPtr  cmEditFragments;

    static QActionPtr  cmViewProject;
    static QActionPtr  cmView3d;
    static QActionPtr  cmViewFill;
    static QActionPtr  cmViewNets;
    static QActionPtr  cmViewGrid;
    static QActionPtr  cmViewLayers;

    static QActionPtr  cm3dMaster;
    static QActionPtr  cm3dProgram;
    static QActionPtr  cm3dImportStl;
    static QActionPtr  cm3dImportStep;
    static QActionPtr  cm3dImportVrml;
    static QActionPtr  cm3dShow2d;
    static QActionPtr  cm3dShowPads;

    static QActionPtr  cmNetSetup;
    static QActionPtr  cmModeLink;
    static QActionPtr  cmModeNetName;
    static QActionPtr  cmModeNetList;
    static QActionPtr  cmModePack;
    static QActionPtr  cmPads;

    static QActionPtr  cmShowRatNet;
    static QActionPtr  cmShowRuleErrors;
    static QActionPtr  cmRenumeration;
    static QActionPtr  cmShowField;
    static QActionPtr  cmShowPads;

    static QActionPtr  cmRulesEdit;
    static QActionPtr  cmRulesCheck;
    static QActionPtr  cmRulesErrorNext;

    static QActionPtr  cmSheetExpression;

    static QActionPtr  cmOption;
    static QActionPtr  cmTools;

    static QActionPtr  cmHelpContents;
    static QActionPtr  cmHelpIndex;
    static QActionPtr  cmHelpAbout;
    static QActionPtr  cmHelpRegistration;
    static QActionPtr  cmHelpHome;
    static QActionPtr  cmHelpBackward;
    static QActionPtr  cmHelpForward;

    static QActionPtr  cmGuiderCapture;
    static QActionPtr  cmGuiderPause;

    //Full mode action table
    static QActionPtr  cmModeTable[MD_LAST];

    //Full list mode tool bars
    static QToolBarPtr mbarTable[PB_LAST];

    //Set visible properties bar for barId
    static void        setModeBar( int barId );
    //Get properties bar for barId
    static QToolBar   *getModeBar( int barId );
    //Get current visible barId
    static int         getModeBarId();

    //!
    //! \brief getModeToolBar Helper function which convert QToolBar returned by getModeBar to derived class
    //! \param barId          Ident of bar
    //! \return               Tool bar reduced to needed type
    //!
    template<typename ToolBar>
    static ToolBar    *getModeToolBar( int barId ) { return dynamic_cast<ToolBar*>( getModeBar(barId) ); }


    static void        createMenu( SdWMain *frame );
    static void        updatePreviousMenu();
    static void        addToPreviousMenu( const QString fname );
    static void        projectState( bool enable );
    static void        createToolBars( SdWMain *frame );
    static void        hideEditorContext();
    static void        selectMode( int md );
  };

#endif // SDWCOMMAND_H
