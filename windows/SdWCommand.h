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
#include <QAction>
#include <QMenu>

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
    static QMenu *menuInsert;
    static QMenu *menuInstruments;
    static QMenu *menuHelp;


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
    static QActionPtr cmViewZoomIn;
    static QActionPtr cmViewZoomOut;
    static QActionPtr cmViewArea;
    static QActionPtr cmViewMeasurement;

    static QActionPtr cmModeLine;
    static QActionPtr cmModeRect;
    static QActionPtr cmModeFilledRect;
    static QActionPtr cmModeRegion;
    static QActionPtr cmModeFilledRegion;
    static QActionPtr cmModeCircle;
    static QActionPtr cmModeFilledCircle;
    static QActionPtr cmModeArc;
    static QActionPtr cmModeText;
    static QActionPtr cmModeField;

    static QActionPtr cmBall;
    static QActionPtr cmPinWired;
    static QActionPtr cmPinFlat;
    static QActionPtr cmBodyCylinder;
    static QActionPtr cmBodyBrick;
    static QActionPtr cmBodyContur;
    static QActionPtr cmModePin;
    static QActionPtr cmModeReference;
    static QActionPtr cmModeOrigin;
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

    static void createMenu( SdWMain *frame );
    static void updatePreviousMenu();
    static void addToPreviousMenu( const QString fname );
    static void projectState( bool enable );
  };

#endif // SDWCOMMAND_H
