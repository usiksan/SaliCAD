/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDWCOMMAND_H
#define SDWCOMMAND_H

#include "SdConfig.h"
#include "modes/SdModeIdents.h"
#include <QAction>
#include <QMenu>
#include <QToolBar>

typedef QAction *QActionPtr;

class SdWMain;

class SdWCommand
  {
  public:
    //static QMenu *
    static QMenu *menuFile;
    static QMenu *menuFilePrevious;
    static QMenu *menuObject;
    static QMenu *menuEdit;
    static QMenu *menuView;
    static QMenu *menuDraw;
    static QMenu *menuInsertSymbol;
    static QMenu *menuInsertSheet;
    static QMenu *menuInsertPart;
    static QMenu *menuInsertPart3d;
    static QMenu *menuInsertPcb;
    static QMenu *menuInsertComp;
    static QMenu *menuInstruments;
    static QMenu *menuHelp;

    static QActionPtr cmMenuInsertSymbol;
    static QActionPtr cmMenuInsertSheet;
    static QActionPtr cmMenuInsertPart;
    static QActionPtr cmMenuInsertPart3d;
    static QActionPtr cmMenuInsertPcb;
    static QActionPtr cmMenuInsertComp;

    static QToolBar *barMain;
    static QToolBar *barSymbol;
    static QToolBar *barPart;
    static QToolBar *barComp;
    static QToolBar *barSheet;
    static QToolBar *barPcb;

    static QToolBar *mbarLinear;

    static void addEditCommands( QToolBar *bar );
    static void addViewCommands( QToolBar *bar );
    static void addDrawCommands( QToolBar *bar );
    static void setModeBar( quint64 propType );



    static QActionPtr cmFileNew;
    static QActionPtr cmFileOpen;
    static QActionPtr cmFileClose;
    static QActionPtr cmFileCloseAll;
    static QActionPtr cmFileSave;
    static QActionPtr cmFileSaveAs;
    static QActionPtr cmFileSaveAll;
    static QActionPtr cmFilePrint;
    static QActionPtr cmFileExit;
    static QActionPtr cmFileImport;
    static QActionPtr cmFileExport;

    static QActionPtr cmFilePrevious[PREVIOUS_FILES_COUNT];

    static QActionPtr cmObjectNew;
    static QActionPtr cmObjectRename;
    static QActionPtr cmObjectDelete;
    static QActionPtr cmObjectCopy;
    static QActionPtr cmObjectPaste;
    static QActionPtr cmObjectCut;
    static QActionPtr cmObjectSort;

    static QActionPtr cmEditUndo;
    static QActionPtr cmEditRedo;
    static QActionPtr cmEditCut;
    static QActionPtr cmEditCopy;
    static QActionPtr cmEditPaste;
    static QActionPtr cmEditDelete;
    static QActionPtr cmEditSelectAll;
    static QActionPtr cmEditFind;
    static QActionPtr cmEditReplace;
    static QActionPtr cmEditProperties;

    static QActionPtr cmViewProject;
    static QActionPtr cmViewFill;
    static QActionPtr cmViewNets;
    static QActionPtr cmViewGrid;
    static QActionPtr cmViewLayers;

    static QActionPtr cmMode3dBall;
    static QActionPtr cmMode3dPinWired;
    static QActionPtr cmMode3dPinFlat;
    static QActionPtr cmMode3dBodyCylinder;
    static QActionPtr cmMode3dBodyBrick;
    static QActionPtr cmMode3dBodyContur;
    static QActionPtr cmModeComponent;
    static QActionPtr cmModeNet;
    static QActionPtr cmNetSetup;
    static QActionPtr cmModeBus;
    static QActionPtr cmModeDisconnect;
    static QActionPtr cmModePcbArea;
    static QActionPtr cmModeLink;
    static QActionPtr cmModeNetName;
    static QActionPtr cmModeNetList;
    static QActionPtr cmModePack;
    static QActionPtr cmPads;
    static QActionPtr cmModeLineSize;
    static QActionPtr cmModeRadiusSize;
    static QActionPtr cmModeMovePart;
    static QActionPtr cmModePlace;
    static QActionPtr cmModeEditWire;
    static QActionPtr cmModeWire;
    static QActionPtr cmModePolygon;
    static QActionPtr cmModeDeleteWire;
    static QActionPtr cmModePad;

    static QActionPtr cmOption;
    static QActionPtr cmTools;

    static QActionPtr cmHelpContens;
    static QActionPtr cmHelpIndex;
    static QActionPtr cmHelpAbout;

    //Full mode action table
    static QActionPtr cmModeTable[MD_LAST];

    static void createMenu( SdWMain *frame );
    static void updatePreviousMenu();
    static void addToPreviousMenu( const QString fname );
    static void projectState( bool enable );
    static void createToolBars( SdWMain *frame );
    static void hideEditorContext();
    static void selectMode( int md );
  };

#endif // SDWCOMMAND_H
