﻿/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  This file contains list of all gui commands and toolBars

  Functions of this class used to init menus and creation tool bars.

  All command mapping to main window handlers
*/

#include "SdWCommand.h"
#include "SdWMain.h"
#include "SdPropBar.h"
#include "SdPropBarDefault.h"
#include "SdPropBarLinear.h"
#include "SdPropBarTextual.h"
#include "SdPropBarSymPin.h"
#include "SdPropBarPartPin.h"
#include "SdPropBarSymImp.h"
#include "SdPropBarPartImp.h"
#include "SdPropBarRoad.h"
#include "SdPropBarPolygon.h"
#include "SdPropBarWire.h"
#include "SdPropBarPartPlace.h"
#include "objects/SdEnvir.h"
#include <QMenuBar>
#include <QSettings>
#include <QFileInfo>
#include <QComboBox>
#include <QDebug>




void SdWCommand::createMenu(SdWMain *frame)
  {
  //File menu [Меню Файл]
  menuFile = new QMenu( QObject::tr("File") );




  cmFileNew = menuFile->addAction( QIcon(QString(":/pic/new.png")), QObject::tr("New project"), frame, SLOT(cmFileNew()) );
  cmFileLoadFromLibrary = menuFile->addAction( QIcon(QString(":/pic/download.png")), QObject::tr("Load from library..."), frame, SLOT(cmFileLoad()) );
  cmFileOpen = menuFile->addAction( QIcon(QString(":/pic/fileOpen.png")), QObject::tr("Open project ..."), frame, SLOT(cmFileOpen()) );
  menuFilePrevious = menuFile->addMenu( QObject::tr("Previous files") );
  menuFile->addSeparator();
  cmFileClose = menuFile->addAction( QIcon(QString(":/pic/closeFile.png")), QObject::tr("Close project"), frame, SLOT(cmFileClose()) );
  cmFileCloseAll = menuFile->addAction( QIcon(QString(":/pic/fileCloseAll.png")), QObject::tr("Close all projects"), frame, SLOT(cmFileCloseAll()) );
  menuFile->addSeparator();
  cmFileSave = menuFile->addAction( QIcon(QString(":/pic/save.png")),QObject::tr("Save project"), frame, SLOT(cmFileSave()) );
  cmFileSaveAs = menuFile->addAction( QIcon(QString(":/pic/save_as.png")), QObject::tr("Save project as..."), frame, SLOT(cmFileSaveAs()) );
  cmFileSaveAll = menuFile->addAction( QIcon(QString(":/pic/save_all.png")), QObject::tr("Save all projects"), frame, SLOT(cmFileSaveAll()) );
  cmFileStoreToLibrary = menuFile->addAction( QIcon(QString(":/pic/upload.png")),QObject::tr("Store to library..."), frame, SLOT(cmFileStore()) );
  cmFileCloud = menuFile->addAction( QIcon(QStringLiteral(":/pic/projectCloud.png")), QObject::tr("Project stored in library"), frame, SLOT(cmFileCloud()) );
  cmFileCloud->setCheckable(true);
  menuFile->addSeparator();
  cmFileImportPis = menuFile->addAction( QObject::tr("Import PasCAD"), frame, SLOT(cmFileImportPis()) );
  cmFileImport    = menuFile->addAction( QIcon(QString(":/pic/fileImport.png")), QObject::tr("Import..."), frame, SLOT(cmFileImport()) );
  cmFileExport    = menuFile->addAction( QIcon(QString(":/pic/fileExport.png")), QObject::tr("Export..."), frame, SLOT(cmFileExport()) );
  menuFile->addSeparator();
  cmFilePrint = menuFile->addAction( QIcon(QString(":/pic/filePrint.png")), QObject::tr("Print..."), frame, SLOT(cmFilePrint()) );
  menuFile->addSeparator();
  cmFileExit = menuFile->addAction( QIcon(QString(":/pic/exit.png")), QObject::tr("Exit programm"), frame, SLOT(cmFileExit()) );

  //Last previous files menu [Меню для предыдущих файлов]
  for (int i = 0; i < PREVIOUS_FILES_COUNT; ++i)
    {
    cmFilePrevious[i] = new QAction(frame);
    cmFilePrevious[i]->setVisible(false);
    frame->connect(cmFilePrevious[i], SIGNAL(triggered()), frame, SLOT(cmFilePrevious()) );
    menuFilePrevious->addAction(cmFilePrevious[i]);
    }

  updatePreviousMenu();


  //Object menu [Меню Объект]
  menuObject = new QMenu( QObject::tr("Objects") );
  cmObjectNew         = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectNew.png")), QObject::tr("Create..."), frame, SLOT(cmObjectNew()) );
  cmObjectLoad        = menuObject->addAction( QIcon(QStringLiteral(":/pic/download.png")), QObject::tr("Load from library..."), frame, SLOT(cmObjectLoad()) );
  cmObjectRename      = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectRename.png")), QObject::tr("Rename..."), frame, SLOT(cmObjectRename()) );
  cmObjectParam       = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectParam.png")), QObject::tr("Edit object param..."), frame, SLOT(cmObjectParam()) );
  cmObjectDelete      = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectDelete.png")), QObject::tr("Delete"), frame, SLOT(cmObjectDelete()) );
  cmObjectCopy        = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectCopy.png")), QObject::tr("Copy"), frame, SLOT(cmObjectCopy()) );
  cmObjectPaste       = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectPaste.png")), QObject::tr("Paste"), frame, SLOT(cmObjectPaste()) );
  cmObjectCut         = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectCut.png")), QObject::tr("Cut"), frame, SLOT(cmObjectCut()) );
  cmObjectDuplicate   = menuObject->addAction( QIcon(QStringLiteral(":/pic/duplicate.png")), QObject::tr("Duplicate"), frame, SLOT(cmObjectDuplicate()) );
  cmObjectSort        = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectSort.png")), QObject::tr("Sort"), frame, SLOT(cmObjectSort()) );
  cmObjectEditEnable  = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectEditEnable.png")), QObject::tr("Enable edit"), frame, SLOT(cmObjectEditEnable()) );
  cmObjectEditDisable = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectEditDisable.png")), QObject::tr("Finish edit"), frame, SLOT(cmObjectEditDisable()) );
  cmProjectParam      = menuObject->addAction( QIcon(QStringLiteral(":/pic/editProjectParam.png")), QObject::tr("Edit project param..."), frame, SLOT(cmProjectParam()) );
  cmProjectUpgrade    = menuObject->addAction( QIcon(QStringLiteral(":/pic/editProjectUpgrade.png")), QObject::tr("Upgrade with new versions"), frame, SLOT(cmProjectUpgrade()) );




  //Edit menu [Меню Редактирование]
  menuEdit = new QMenu( QObject::tr("Edit") );
  cmEditUndo      = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editUndo.png")), QObject::tr("Undo"), frame, SLOT(cmEditUndo()) );
  cmEditUndo->setEnabled(false);
  cmEditRedo      = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editRedo.png")), QObject::tr("Redo"), frame, SLOT(cmEditRedo()) );
  cmEditRedo->setEnabled(false);
  menuEdit->addSeparator();
  cmEditCopy      = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCopy.png")), QObject::tr("Copy"), frame, SLOT(cmEditCopy()) );
  cmEditPaste     = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editPaste.png")), QObject::tr("Paste"), frame, SLOT(cmEditPaste()) );
  cmEditCut       = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCut.png")), QObject::tr("Cut"), frame, SLOT(cmEditCut()) );
  cmEditDelete    = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editDel.png")), QObject::tr("Delete"), frame, SLOT(cmEditDelete()) );
  menuEdit->addSeparator();
  cmEditCopySvg   = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCopySvg.png")), QObject::tr("Copy SVG"), frame, SLOT(cmEditCopySvg()) );
  cmEditCopyWeb   = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCopyWeb.png")), QObject::tr("Copy WEB"), frame, SLOT(cmEditCopyWeb()) );
  cmEditSelectSave= menuEdit->addAction( QIcon(QStringLiteral(":/pic/editSelectSave.png")), QObject::tr("Save selection"), frame, SLOT(cmEditSelectSave()) );
  menuEdit->addSeparator();
  cmEditSelectAll = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editSelectAll.png")), QObject::tr("Select All"), frame, SLOT(cmEditSelectAll()) );
  cmEditUnSelect  = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editUnselect.png")), QObject::tr("UnSelect"), frame, SLOT(cmEditUnSelect()) );
  cmEditFind      = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editFind.png")), QObject::tr("Find"), frame, SLOT(cmEditFind()) );
  cmEditReplace   = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editReplace.png")), QObject::tr("Replace"), frame, SLOT(cmEditReplace()) );
  cmEditRotateGroup = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editRotate.png")), QObject::tr("Rotate group 90 degree [F7]"), frame, SLOT(cmEditRotateGroup()) );
  menuEdit->addSeparator();
  cmEditProperties  = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editProp.png")), QObject::tr("Params of selection..."), frame, SLOT(cmEditProperties()) );
  cmEditCalculations = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCalculations.png")), QObject::tr("Calculations..."), frame, SLOT(cmEditCalculations()) );
  cmEditFragments   = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editFragments.png")), QObject::tr("Fragments..."), frame, SLOT(cmEditFragments()) );



  //View menu
  menuView = new QMenu( QObject::tr("View") );
  cmViewProject = menuView->addAction( QIcon(QString(":/pic/openProjects.png")), QObject::tr("Show-hide project"), frame, SLOT(cmViewProject()) );
  menuView->addSeparator();
  cmView3d   = menuView->addAction( QIcon(QStringLiteral(":/pic/view3d.png")), QObject::tr("3d-2d switch"), frame, SLOT(cmView3d()) );
  cmView3d->setEnabled(false);
  cmViewMirror = menuView->addAction( QObject::tr("Mirror"), frame, SLOT(cmViewMirrorHorz()) );
  cmViewMirror->setCheckable(true);
  cmViewMirror->setEnabled(false);
  cmViewNets = menuView->addAction( QIcon(QString(":/pic/viewRatnet.png")), QObject::tr("Nets"), frame, SLOT(cmViewNets()) );
  cmViewGrid = menuView->addAction( QIcon(QString(":/pic/viewGrid.png")), QObject::tr("Grid"), frame, SLOT(cmViewGrid()) );
  cmViewGrid->setToolTip( QObject::tr("Show grid tune dialog") );
  cmViewLayers = menuView->addAction( QIcon(QString(":/pic/layers.png")), QObject::tr("Layers"), frame, SLOT(cmViewLayers()) );
  cmViewLayers->setToolTip( QObject::tr("Show layers editor dialog") );
  menuView->addSeparator();
  cmViewFill = menuView->addAction( QIcon(QString(":/pic/viewFit.png")), QObject::tr("Zoom to fit view"), frame, SLOT(cmViewFill()) );
  cmModeTable[MD_ZOOM_IN]     = menuView->addAction( QIcon(QString(":/pic/viewZoomIn.png")), QObject::tr("Zoom in"), frame, SLOT(cmViewZoomIn()) );
  cmModeTable[MD_ZOOM_OUT]    = menuView->addAction( QIcon(QString(":/pic/viewZoomOut.png")), QObject::tr("Zoom out"), frame, SLOT(cmViewZoomOut()) );
  cmModeTable[MD_ZOOM_WIN]   = menuView->addAction( QIcon(QString(":/pic/viewWin.png")), QObject::tr("Zoom area"), frame, SLOT(cmViewArea()) );
  menuView->addSeparator();
  cmModeTable[MD_MEASUREMENT] = menuView->addAction( QIcon(QString(":/pic/viewRuller.png")), QObject::tr("Measurement"), frame, SLOT(cmViewMeasurement()) );



  //Draw menu
  menuDraw = new QMenu( QObject::tr("Draw") );
  cmModeTable[MD_SELECT]      = menuDraw->addAction( QIcon(QString(":/pic/select.png")), QObject::tr("Select and edit"), frame, SLOT(cmModeSelect()) );
  menuDraw->addSeparator();
  cmModeTable[MD_LINE]        = menuDraw->addAction( QIcon(QString(":/pic/drawLine.png")), QObject::tr("Line"), frame, SLOT(cmModeLine()) );
  cmModeTable[MD_RECT]        = menuDraw->addAction( QIcon(QString(":/pic/drawRect.png")), QObject::tr("Rect"), frame, SLOT(cmModeRect()) );
  cmModeTable[MD_FILL_RECT]   = menuDraw->addAction( QIcon(QString(":/pic/drawFRect.png")), QObject::tr("Filled rect"), frame, SLOT(cmModeFilledRect()) );
  cmModeTable[MD_REGION]      = menuDraw->addAction( QIcon(QString(":/pic/drawRegion.png")), QObject::tr("Region"), frame, SLOT(cmModeRegion()) );
  cmModeTable[MD_FILL_REGION] = menuDraw->addAction( QIcon(QString(":/pic/drawFRegion.png")), QObject::tr("Filled region"), frame, SLOT(cmModeFilledRegion()) );
  cmModeTable[MD_CIRCLE]      = menuDraw->addAction( QIcon(QString(":/pic/drawCircle.png")), QObject::tr("Circle"), frame, SLOT(cmModeCircle()) );
  cmModeTable[MD_FILL_CIRCLE] = menuDraw->addAction( QIcon(QString(":/pic/drawFCircle.png")), QObject::tr("Filled circle"), frame, SLOT(cmModeFilledCircle()) );
  cmModeTable[MD_ARC]         = menuDraw->addAction( QIcon(QString(":/pic/drawArc.png")), QObject::tr("Arc"), frame, SLOT(cmModeArc()) );
  cmModeTable[MD_TEXT]        = menuDraw->addAction( QIcon(QString(":/pic/drawText.png")), QObject::tr("Text"), frame, SLOT(cmModeText()) );
  cmShowField = menuDraw->addAction( QIcon(QStringLiteral(":/pic/showField.png")), QObject::tr("Show fields or values") );
  cmShowField->setCheckable(true);
  cmShowField->setChecked( sdEnvir->mShowFields );
  cmShowField->connect( cmShowField, &QAction::toggled, frame, &SdWMain::cmShowFields );



  //Symbol editor menu
  menuInsertSymbol = new QMenu( QObject::tr("Symbol") );

  cmModeTable[MD_SYM_PIN]         = menuInsertSymbol->addAction( QIcon(QString(":/pic/objPin.png")), QObject::tr("Insert pin"), frame, SLOT(cmModePin()) );
  cmModeTable[MD_SYM_IDENT]       = menuInsertSymbol->addAction( QIcon(QString(":/pic/objIdent.png")), QObject::tr("Edit reference"), frame, SLOT(cmModeReference()) );
  cmModeTable[MD_SYM_ORIGIN]      = menuInsertSymbol->addAction( QIcon(QString(":/pic/objOrigin.png")), QObject::tr("Edit origin"), frame, SLOT(cmModeOrigin()) );
  cmModeTable[MD_SYM_VALUE]       = menuInsertSymbol->addAction( QIcon(QString(":/pic/objValue.png")), QObject::tr("Edit value"), frame, SLOT(cmModeValue()) );
  cmModeTable[MD_SYMBOL_FRAGMENT] = menuInsertSymbol->addAction( QIcon(QString(":/pic/symbolFragment.png")), QObject::tr("Enter symbol fragment"), frame, SLOT(cmModeSymbolFragment()) );
  menuInsertSymbol->addSeparator();
  cmSymbolPartParam               = menuInsertSymbol->addAction( QIcon(QString(":/pic/objSymbolPartParam.png")), QObject::tr("Default part and params"), frame, &SdWMain::cmSymbolPartAndParam );



  //Part editor menu
  menuInsertPart = new QMenu( QObject::tr("Part") );

  cmShowPads                     = menuInsertPart->addAction( QIcon(QString(":/pic/objVia.png")), QObject::tr("Show pads") );
  cmShowPads->setCheckable(true);
  cmShowPads->setChecked( sdEnvir->mShowPads );
  cmShowPads->connect( cmShowPads, &QAction::toggled, frame, &SdWMain::cmShowPads );
  cmModeTable[MD_PART_PIN]       = menuInsertPart->addAction( QIcon(QString(":/pic/objPrtPin.png")), QObject::tr("Insert pin"), frame, SLOT(cmModePin()) );
  cmModeTable[MD_PART_IDENT]     = menuInsertPart->addAction( QIcon(QString(":/pic/objIdent.png")), QObject::tr("Edit reference"), frame, SLOT(cmModeReference()) );
  cmModeTable[MD_PART_ORIGIN]    = menuInsertPart->addAction( QIcon(QString(":/pic/objOrigin.png")), QObject::tr("Edit origin"), frame, SLOT(cmModeOrigin()) );
  cmModeTable[MD_PART_VALUE]     = menuInsertPart->addAction( QIcon(QString(":/pic/objPrtValue.png")), QObject::tr("Edit value"), frame, SLOT(cmModeValue()) );



  //3D part editor menu
  menuInsertPart3d = new QMenu( QObject::tr("3d part") );
  cm3dMaster           = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dmaster.png")), QObject::tr("Master of 3d form"), frame, SLOT(cm3dMaster()) );
  cm3dProgram          = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dProgram.png")), QObject::tr("Edit 3d model script"), frame, SLOT(cm3dProgram()) );
  cm3dImportStl        = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dImportStl.png")), QObject::tr("Import from stl file"), frame, SLOT(cm3dImportStl()) );
  //cm3dImportStep       = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dImportStep.png")), QObject::tr("Import from step file"), frame, SLOT(cm3dImportStep()) );
  cm3dImportVrml       = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dImportVrml.png")), QObject::tr("Import from vrml file"), frame, SLOT(cm3dImportVrml()) );
  menuInsertPart3d->addSeparator();
  cm3dShow2d                     = menuInsertPart3d->addAction( QIcon(QString(":/pic/drawRect.png")), QObject::tr("Show/hide 2d graphics in 3d scene"), frame, SLOT(cm3dShow2d()) );
  cm3dShow2d->setCheckable(true);
  cm3dShowPads                   = menuInsertPart3d->addAction( QIcon(QString(":/pic/objVia.png")), QObject::tr("Show/hide pads"), frame, SLOT(cm3dShowPads()) );
  cm3dShowPads->setCheckable(true);
  menuInsertPart3d->addSeparator();
  cmModeTable[MD_3D_VIEW]        = menuInsertPart3d->addAction( QIcon(QString(":/pic/select.png")), QObject::tr("View 3d scene with no edit"), frame, SLOT(cm3dModeView()) );
  cmModeTable[MD_3D_HORZ_MOVE]   = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dHorzMove.png")), QObject::tr("Horizontal moving 3d object"), frame, SLOT(cm3dModeHorzMove()) );
  cmModeTable[MD_3D_HORZ_ROTATE] = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dHorzRotate.png")), QObject::tr("Horizontal rotation 3d object"), frame, SLOT(cm3dModeHorzRotate()) );
  cmModeTable[MD_3D_VERT_MOVE]   = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dVertMove.png")), QObject::tr("Vertical moving 3d object"), frame, SLOT(cm3dModeVertMove()) );
  cmModeTable[MD_3D_VERT_ROTATE] = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dVertRotate.png")), QObject::tr("Vertical rotation 3d object"), frame, SLOT(cm3dModeVertRotate()) );
  cmModeTable[MD_3D_FACE_COLOR]  = menuInsertPart3d->addAction( QIcon(QString(":/pic/colors.png")), QObject::tr("Change face color"), frame, SLOT(cm3dModeFaceColor()) );


  //3D part view menu
  menuViewPart3d = new QMenu( QObject::tr("3d part view") );
  menuViewPart3d->addAction( cm3dProgram );
  menuViewPart3d->addSeparator();
  menuViewPart3d->addAction( cm3dShow2d );
  menuViewPart3d->addAction( cm3dShowPads );


  menuInsertComp = new QMenu( QObject::tr("Insert comp") );



  //Sheet editor menu
  menuInsertSheet = new QMenu( QObject::tr("Sheet") );
  cmRenumeration              = menuInsertSheet->addAction( QIcon(QStringLiteral(":/pic/renumeration.png")), QObject::tr("Renumeration"), frame, SLOT(cmRenumeration()) );
  menuInsertSheet->addSeparator();
  cmModeTable[MD_FRAGMENT]    = menuInsertSheet->addAction( QIcon(QString(":/pic/iconSheet.png")), QObject::tr("Insert fragment"), frame, SLOT(cmModeFragment()) );
  cmModeTable[MD_COMPONENT]   = menuInsertSheet->addAction( QIcon(QString(":/pic/objComp.png")), QObject::tr("Insert component"), frame, SLOT(cmModeComponent()) );
  cmModeTable[MD_NET]         = menuInsertSheet->addAction( QIcon(QString(":/pic/objWire.png")), QObject::tr("Insert net wire"), frame, SLOT(cmModeNet()) );
  cmModeTable[MD_NET_NAME]    = menuInsertSheet->addAction( QIcon(QString(":/pic/objWireName.png")), QObject::tr("Insert net name"), frame, SLOT(cmModeNetName()) );
  cmModeTable[MD_BUS]         = menuInsertSheet->addAction( QIcon(QString(":/pic/objBus.png")), QObject::tr("Insert bus (many net)"), frame, SLOT(cmModeBus()) );
  cmModeTable[MD_DISCONNECT]  = menuInsertSheet->addAction( QIcon(QString(":/pic/objUnConnect.png")), QObject::tr("Disconnect net"), frame, SLOT(cmModeDisconnect()) );
  cmModeTable[MD_SHEET_IDENT] = menuInsertSheet->addAction( QIcon(QString(":/pic/objIdent.png")), QObject::tr("Move ident of components"), frame, SLOT(cmModeReferenceMove()) );
  cmModeTable[MD_SHEET_VALUE] = menuInsertSheet->addAction( QIcon(QString(":/pic/objValue.png")), QObject::tr("Move value of components"), frame, SLOT(cmModeValueMove()) );
  cmModeTable[MD_SCRIPT]      = menuInsertSheet->addAction( QIcon(QString(":/pic/objScript.png")), QObject::tr("Script insert and edit"), frame, SLOT(cmModeScript()) );
  //cmModeTable[MD_PCB_AREA]   = menuInsertSheet->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
//  cmModeTable[MD_FIELD]      = menuInsertSheet->addAction( QIcon(QString(":/pic/objField.png")), QObject::tr("Field"), frame, SLOT(cmModeF) );



  //PCB editor menu
  menuInsertPcb = new QMenu( QObject::tr("Plate") );
  menuInsertPcb->insertAction( nullptr, cmRenumeration );
  cmPcbStratum  = menuInsertPcb->addAction( QIcon(QString(":/pic/iconViaThrow.png")), QObject::tr("PCB layer count..."), frame, SLOT(cmPcbStratum()) );
  menuInsertPcb->insertSeparator(nullptr);
  cmShowRatNet = menuInsertPcb->addAction( QIcon(QStringLiteral(":/pic/viewRatnet.png")), QObject::tr("Show rat net") );
  cmShowRatNet->setCheckable(true);
  cmShowRatNet->setChecked( sdEnvir->mShowRatNet );
  cmShowRatNet->connect( cmShowRatNet, &QAction::toggled, frame, &SdWMain::cmShowRatNet );
  cmPads = menuInsertPcb->addAction( QIcon(QStringLiteral(":/pic/pads.png")), QObject::tr("Pads association"), frame, SLOT(cmPads()) );
  //cmShowRatNet->co
//  cmNetSetup       = menuInsertPcb->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
//  cmPads           = menuInsertPcb->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
//  menuInsertPcb->addSeparator();
  //  cmModeLink       = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeNetName    = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeNetList    = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  // cmModePack       = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeLineSize   = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeRadiusSize = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  cmModeTable[MD_PART_IMP]   = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrt.png")), QObject::tr("Insert part"), frame, SLOT(cmModeComponent()) );
  cmModeTable[MD_MOVE_PART]  = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrtPlace.png")), QObject::tr("Move part"), frame, SLOT(cmModeMovePart()) );
  cmModeTable[MD_ROAD_ENTER] = menuInsertPcb->addAction( QIcon(QString(":/pic/objPcbWire.png")), QObject::tr("Road enter"), frame, SLOT(cmModeRoadEnter()) );
  cmModeTable[MD_ROAD_MOVE]  = menuInsertPcb->addAction( QIcon(QString(":/pic/objPcbWireMove.png")), QObject::tr("Road move"), frame, SLOT(cmModeRoadMove()) );
  cmModeTable[MD_POLYGON]    = menuInsertPcb->addAction( QIcon(QString(":/pic/objPolygon.png")), QObject::tr("Polygon enter"), frame, SLOT(cmModePolygon()) );
  cmModeTable[MD_VIA_ENTER]  = menuInsertPcb->addAction( QIcon(QString(":/pic/objVia.png")), QObject::tr("Via enter"), frame, SLOT(cmModeViaEnter()) );
  cmModeTable[MD_TRACE_DELETE]   = menuInsertPcb->addAction( QIcon(QString(":/pic/objPcbWireDel.png")), QObject::tr("Delete trace"), frame, SLOT(cmModeTraceDelete()) );
  //  cmModePlace      = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeEditWire   = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeWire       = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeDeleteWire = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModePad        = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  cmModeTable[MD_PLATE_IDENT] = menuInsertPcb->addAction( QIcon(QString(":/pic/objIdent.png")), QObject::tr("Move ident of components"), frame, SLOT(cmModeReferenceMove()) );
  cmModeTable[MD_PLATE_VALUE] = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrtValue.png")), QObject::tr("Move value of components"), frame, SLOT(cmModeValueMove()) );
  cmModeTable[MD_PLATE_ORIGIN]= menuInsertPcb->addAction( QIcon(QString(":/pic/objPlateOrigin.png")), QObject::tr("Edit plate origin"), frame, SLOT(cmModeOrigin()) );
  cmModeTable[MD_PART_HIGHLIGHT] = menuInsertPcb->addAction( QIcon(QString(":/pic/objPartHighlight.png")), QObject::tr("Highlight components"), frame, SLOT(cmModePartHighlight()) );


  //PCB roads rules menu
  menuRules = new QMenu( QObject::tr("Rules") );
  cmRulesEdit = menuRules->addAction( QIcon(QStringLiteral(":/pic/rules.png")), QObject::tr("Edit rules dialog"), frame, SLOT(cmRulesEdit()) );
  cmShowRuleErrors = menuRules->addAction( QIcon(QStringLiteral(":/pic/objShowRuleErrors.png")), QObject::tr("Show rule errors") );
  cmShowRuleErrors->setCheckable(true);
  cmShowRuleErrors->setChecked( sdEnvir->mShowRuleErrors );
  cmShowRuleErrors->connect( cmShowRuleErrors, &QAction::toggled, frame, &SdWMain::cmShowRuleErrors );
  cmRulesCheck = menuRules->addAction( QIcon(QStringLiteral(":/pic/objRulesCheck.png")), QObject::tr("Check all rules"), frame, SLOT(cmRulesCheck()) );
  cmRulesErrorNext = menuRules->addAction( QIcon(QStringLiteral(":/pic/rulesNextError.png")), QObject::tr("Center next rules error"), frame, SLOT(cmRulesErrorNext()) );


  //Instruments menu
  menuInstruments = new QMenu( QObject::tr("Instruments") );
  cmOption = menuInstruments->addAction( QIcon(QString(":/pic/instrumOptions.png")), QObject::tr("Options"), frame, SLOT(cmOption()) );
  menuInstruments->addSeparator();
  cmTools  = menuInstruments->addAction( QIcon(QString(":/pic/instrumTools.png")), QObject::tr("Tools"), frame, SLOT(cmTools()) );
  menuInstruments->addSeparator();
  cmGuiderCapture    = menuInstruments->addAction( QObject::tr("Capture start-stop"), frame, SLOT(cmGuiderCapture()), QKeySequence(Qt::Key_F10) );
  cmGuiderPause      = menuInstruments->addAction( QObject::tr("Capture pause-resume"), frame, SLOT(cmGuiderPause()), QKeySequence(Qt::Key_F11) );


  //Help menu
  menuHelp = new QMenu( QObject::tr("Help") );
  cmHelpContents      = menuHelp->addAction( QIcon(QString(":/pic/help.png")), QObject::tr("Contents"), frame, SLOT(cmHelpContents()) );
  cmHelpIndex        = menuHelp->addAction( QIcon(QString(":/pic/helpContext.png")), QObject::tr("Index"), frame, SLOT(cmHelpIndex()) );
  cmHelpAbout        = menuHelp->addAction( QIcon(QString(":/pic/helpAbout.png")), QObject::tr("About"), frame, SLOT(cmHelpAbout()) );
  cmHelpRegistration = menuHelp->addAction( QIcon(QString(":/pic/helpRegistration.png")), QObject::tr("Registration"), frame, SLOT(cmHelpRegistration()) );
  cmHelpHome         = new QAction( QIcon(QString(":/pic/helpHome.png")), QObject::tr("Home help page") );
  frame->connect( cmHelpHome, &QAction::triggered, frame, &SdWMain::cmHelpContents );
  cmHelpBackward     = new QAction( QIcon(QString(":/pic/helpPrevious.png")), QObject::tr("Backward help page") );
  frame->connect( cmHelpBackward, &QAction::triggered, frame, &SdWMain::cmHelpBackward );
  cmHelpForward      = new QAction( QIcon(QString(":/pic/helpNext.png")), QObject::tr("Forward help page") );
  frame->connect( cmHelpForward, &QAction::triggered, frame, &SdWMain::cmHelpForward );


  //Select popup menu
  menuSelect = new QMenu( QObject::tr("Select") );
  menuSelect->insertAction( nullptr, cmEditUndo );
  menuSelect->insertAction( nullptr, cmEditRedo );
  menuSelect->addSeparator();
  menuSelect->insertAction( nullptr, cmEditCopy );
  menuSelect->insertAction( nullptr, cmEditPaste );
  menuSelect->insertAction( nullptr, cmEditCut );
  menuSelect->insertAction( nullptr, cmEditDelete );
  menuSelect->addSeparator();
  menuSelect->insertAction( nullptr, cmEditSelectAll );
  menuSelect->insertAction( nullptr, cmEditUnSelect );
  menuSelect->insertAction( nullptr, cmEditFind );
  menuSelect->insertAction( nullptr, cmEditReplace );
  menuSelect->insertAction( nullptr, cmEditRotateGroup );
  menuSelect->addSeparator();
  menuSelect->insertAction( nullptr, cmEditProperties );
  menuSelect->insertAction( nullptr, cmEditCalculations );
  menuSelect->insertAction( nullptr, cmEditFragments );



  QMenuBar *bar = frame->menuBar();
  bar->addMenu( menuFile );
  bar->addMenu( menuObject );
  bar->addMenu( menuEdit );
  bar->addMenu( menuView );
  cmMenuViewPart3d   = bar->addMenu( menuViewPart3d );
  bar->addMenu( menuDraw );
  cmMenuInsertSymbol = bar->addMenu( menuInsertSymbol );
  cmMenuInsertPart   = bar->addMenu( menuInsertPart );
  cmMenuInsertPart3d = bar->addMenu( menuInsertPart3d );
  cmMenuInsertComp   = bar->addMenu( menuInsertComp );
  cmMenuInsertSheet  = bar->addMenu( menuInsertSheet );
  cmMenuInsertPcb    = bar->addMenu( menuInsertPcb );
  cmMenuRules        = bar->addMenu( menuRules );
  bar->addMenu( menuInstruments );
  bar->addMenu( menuHelp );

  projectState(false);
  }





void SdWCommand::updatePreviousMenu()
  {
  //Заполнить меню предыдущими файлами
  QSettings settings;
  QStringList files = settings.value(SDK_PREVIOUS_FILES).toStringList();

  int numRecentFiles = qMin(files.size(), static_cast<int>(PREVIOUS_FILES_COUNT) );

  for (int i = 0; i < numRecentFiles; ++i)
    {
    QString text = QString("&%1 %2").arg(i + 1).arg( QFileInfo(files[i]).filePath() );
    cmFilePrevious[i]->setText(text);
    cmFilePrevious[i]->setData(files[i]);
    cmFilePrevious[i]->setVisible(true);
    }
  for (int j = numRecentFiles; j < PREVIOUS_FILES_COUNT; ++j)
    cmFilePrevious[j]->setVisible(false);

  //Сделать доступным menuFilePrevious
  menuFilePrevious->setEnabled( numRecentFiles != 0 );
  }




void SdWCommand::addToPreviousMenu(const QString fname)
  {
  if( !fname.isEmpty() && fname != SD_DEFAULT_FILE_NAME ) {
    //Если файл имеет неверное расширение, то не открываем
    QSettings settings;
    QStringList files = settings.value(SDK_PREVIOUS_FILES).toStringList();
    files.removeAll(fname);
    files.prepend(fname);
    while( files.size() > PREVIOUS_FILES_COUNT )
      files.removeLast();

    settings.setValue(SDK_PREVIOUS_FILES, files);

    //Обновить меню
    updatePreviousMenu();
    }
  }




void SdWCommand::projectState(bool enable)
  {
  if( !enable ) {
    cmFileSave->setEnabled(enable);
    cmFilePrint->setEnabled(enable);
    cmFileImport->setEnabled(enable);
    cmFileExport->setEnabled(enable);

    cmObjectRename->setEnabled(enable);
    cmObjectParam->setEnabled(enable);
    cmObjectDelete->setEnabled(enable);
    cmObjectCopy->setEnabled(enable);
    cmObjectCut->setEnabled(enable);
    cmObjectDuplicate->setEnabled(enable);

    cmObjectPaste->setEnabled(enable);
    }
  cmFileClose->setEnabled(enable);
  cmFileCloseAll->setEnabled(enable);
  cmFileSaveAs->setEnabled(enable);
  cmFileSaveAll->setEnabled(enable);

  cmObjectNew->setEnabled(enable);
  cmObjectLoad->setEnabled(enable);
  cmObjectSort->setEnabled(enable);
  }




void SdWCommand::addEditCommands(QToolBar *bar)
  {
  bar->insertAction( nullptr, cmEditCopy );
  bar->insertAction( nullptr, cmEditCut );
  bar->insertAction( nullptr, cmEditPaste );
  bar->insertAction( nullptr, cmEditDelete );
  bar->insertAction( nullptr, cmEditProperties );
  }




void SdWCommand::addViewCommands(QToolBar *bar)
  {
  bar->addSeparator();
  bar->insertAction( nullptr, cmViewGrid );
//  bar->addSeparator();
  bar->insertAction( nullptr, cmViewFill );
  bar->insertAction( nullptr, cmModeTable[MD_ZOOM_IN] );
  bar->insertAction( nullptr, cmModeTable[MD_ZOOM_OUT] );
  bar->insertAction( nullptr, cmModeTable[MD_ZOOM_WIN] );
  bar->insertAction( nullptr, cmModeTable[MD_MEASUREMENT] );
  }





void SdWCommand::addDrawCommands(QToolBar *bar)
  {
  bar->addSeparator();
  bar->insertAction( nullptr, cmModeTable[MD_SELECT] );
//  bar->addSeparator();
  bar->insertAction( nullptr, cmModeTable[MD_LINE] );
  bar->insertAction( nullptr, cmModeTable[MD_RECT] );
  bar->insertAction( nullptr, cmModeTable[MD_FILL_RECT] );
  bar->insertAction( nullptr, cmModeTable[MD_REGION] );
  bar->insertAction( nullptr, cmModeTable[MD_FILL_REGION] );
  bar->insertAction( nullptr, cmModeTable[MD_CIRCLE] );
  bar->insertAction( nullptr, cmModeTable[MD_FILL_CIRCLE] );
  bar->insertAction( nullptr, cmModeTable[MD_ARC] );
  bar->insertAction( nullptr, cmModeTable[MD_TEXT] );
  bar->insertAction( nullptr, cmShowField );
  bar->addSeparator();
  }




void SdWCommand::setModeBar(int barId)
  {
  if( barId >= 0 && barId < PB_LAST ) {
    for( int i = 0; i < PB_LAST; i++ )
      if( mbarTable[i] )
        mbarTable[i]->setVisible( i == barId );
    }
  }



QToolBar *SdWCommand::getModeBar(int barId)
  {
  if( barId >= 0 && barId < PB_LAST )
    return mbarTable[barId];
  return nullptr;
  }




//Get current visible barId
int SdWCommand::getModeBarId()
  {
  for( int i = 0; i < PB_LAST; i++ )
    if( mbarTable[i] && mbarTable[i]->isVisible() )
      return i;
  return PB_DEFAULT;
  }



void SdWCommand::createToolBars(SdWMain *frame)
  {
  //Main bar
  barMain = new QToolBar( QString("Main") );
  barMain->insertAction( nullptr, cmFileNew );
  barMain->insertAction( nullptr, cmFileOpen );
  barMain->insertAction( nullptr, cmFileSave );
  barMain->insertAction( nullptr, cmFilePrint );
  barMain->insertAction( nullptr, cmFileCloud );
  barMain->addSeparator();
  barMain->insertAction( nullptr, cmObjectNew );
  barMain->insertAction( nullptr, cmObjectEditEnable );
  barMain->insertAction( nullptr, cmObjectEditDisable );
  barMain->insertAction( nullptr, cmEditUndo );
  barMain->insertAction( nullptr, cmEditRedo );

  frame->addToolBar( barMain );
  //barMain->setIconSize( QSize(20,24) );



  //Symbol bar
  barSymbol = new QToolBar( QString("Symbol") );
  addEditCommands( barSymbol );
  addViewCommands( barSymbol );
  addDrawCommands( barSymbol );
  barSymbol->insertAction( nullptr, cmModeTable[MD_SYM_PIN] );
  barSymbol->insertAction( nullptr, cmModeTable[MD_SYM_IDENT] );
  barSymbol->insertAction( nullptr, cmModeTable[MD_SYM_ORIGIN] );
  barSymbol->insertAction( nullptr, cmModeTable[MD_SYM_VALUE] );
  barSymbol->insertAction( nullptr, cmModeTable[MD_SYMBOL_FRAGMENT] );
  barSymbol->addSeparator();
  barSymbol->insertAction( nullptr, cmSymbolPartParam );

  frame->addToolBar( barSymbol );


  //Part bar
  barPart = new QToolBar( QString("Part") );
  addEditCommands( barPart );
  addViewCommands( barPart );
  addDrawCommands( barPart );
  barPart->insertAction( nullptr, cmShowPads );
  barPart->insertAction( nullptr, cmModeTable[MD_PART_PIN] );
  barPart->insertAction( nullptr, cmModeTable[MD_PART_IDENT] );
  barPart->insertAction( nullptr, cmModeTable[MD_PART_ORIGIN] );
  barPart->insertAction( nullptr, cmModeTable[MD_PART_VALUE] );

  frame->addToolBar( barPart );


  //Part 3d bar
  barPart3d = new QToolBar( QString("Part3d") );
  barPart3d->insertAction( nullptr, cm3dShowPads );
  barPart3d->insertAction( nullptr, cm3dShow2d );
  barPart3d->addSeparator();
  barPart3d->insertAction( nullptr, cmModeTable[MD_3D_VIEW] );
  barPart3d->addSeparator();
  barPart3d->insertAction( nullptr, cm3dProgram );
  barPart3d->addSeparator();
  barPart3d->insertAction( nullptr, cmModeTable[MD_3D_HORZ_MOVE] );
  barPart3d->insertAction( nullptr, cmModeTable[MD_3D_HORZ_ROTATE] );
  barPart3d->insertAction( nullptr, cmModeTable[MD_3D_VERT_MOVE] );
  barPart3d->insertAction( nullptr, cmModeTable[MD_3D_VERT_ROTATE] );
  frame->addToolBar( barPart3d );

  //Part 3d bar view
  barPart3dView = new QToolBar( QString("Part3dView") );
  barPart3dView->insertAction( nullptr, cm3dShowPads );
  barPart3dView->insertAction( nullptr, cm3dShow2d );
  barPart3dView->addSeparator();
  barPart3dView->insertAction( nullptr, cm3dProgram );
  frame->addToolBar( barPart3dView );

  //Comp bar
  barComp = new QToolBar( QString("Component") );

  frame->addToolBar( barComp );



  //Sheet bar
  barSheet = new QToolBar( QString("Sheet") );
  addEditCommands( barSheet );
  addViewCommands( barSheet );
  addDrawCommands( barSheet );
  barSheet->insertAction( nullptr, cmModeTable[MD_FRAGMENT] );
  barSheet->insertAction( nullptr, cmModeTable[MD_COMPONENT] );
  barSheet->insertAction( nullptr, cmModeTable[MD_NET] );
  barSheet->insertAction( nullptr, cmModeTable[MD_BUS] );
  barSheet->insertAction( nullptr, cmModeTable[MD_DISCONNECT] );
  barSheet->insertAction( nullptr, cmModeTable[MD_NET_NAME] );
  barSheet->insertAction( nullptr, cmModeTable[MD_SHEET_IDENT] );
  barSheet->insertAction( nullptr, cmModeTable[MD_SHEET_VALUE] );

  frame->addToolBar( barSheet );



  //PCB bar
  barPcb = new QToolBar( QString("PCB") );
  addEditCommands( barPcb );
  addViewCommands( barPcb );
  addDrawCommands( barPcb );
  barPcb->insertAction( nullptr, cmShowRatNet );
  barPcb->insertAction( nullptr, cmShowRuleErrors );
  barPcb->insertAction( nullptr, cmRulesEdit );
  barPcb->insertAction( nullptr, cmRulesCheck );
  barPcb->insertAction( nullptr, cmPads );
  barPcb->insertAction( nullptr, cmModeTable[MD_PART_IMP] );
  barPcb->insertAction( nullptr, cmModeTable[MD_MOVE_PART] );
  barPcb->insertAction( nullptr, cmModeTable[MD_ROAD_ENTER] );
  barPcb->insertAction( nullptr, cmModeTable[MD_ROAD_MOVE] );
  barPcb->insertAction( nullptr, cmModeTable[MD_TRACE_DELETE] );
  barPcb->insertAction( nullptr, cmModeTable[MD_POLYGON] );
  barPcb->insertAction( nullptr, cmModeTable[MD_VIA_ENTER] );
  barPcb->insertAction( nullptr, cmModeTable[MD_PLATE_IDENT] );
  barPcb->insertAction( nullptr, cmModeTable[MD_PLATE_VALUE] );
  barPcb->insertAction( nullptr, cmModeTable[MD_PLATE_ORIGIN] );
  barPcb->insertAction( nullptr, cmModeTable[MD_PART_HIGHLIGHT] );

  frame->addToolBar( barPcb );



  //View bar
  barView = new QToolBar( QString("View") );
  addViewCommands( barView );
  //barView->insertAction( nullptr, cmObjectEditEnable );

  frame->addToolBar( barView );


  //Help bar
  barHelp = new QToolBar( QString("Help") );
  barHelp->insertAction( nullptr, cmHelpHome );
  barHelp->insertAction( nullptr, cmHelpBackward );
  barHelp->insertAction( nullptr, cmHelpForward );
  frame->addToolBar(barHelp);


  frame->addToolBarBreak();

  SdPropBar *mbar;
  mbar = new SdPropBar( QStringLiteral("Default tool bar") );
  frame->addToolBar( mbar );
  mbarTable[PB_DEFAULT] = mbar;

  mbar = new SdPropBarLinear( QStringLiteral("Linear mode") );
  frame->addToolBar( mbar );
  mbar->setVisible(false);
  mbarTable[PB_LINEAR] = mbar;
  mbar->connect( mbar, &SdPropBar::propChanged, frame, &SdWMain::cmPropertiesChange );

  mbar = new SdPropBarTextual( QStringLiteral("Textual mode") );
  frame->addToolBar( mbar );
  mbar->setVisible(false);
  mbarTable[PB_TEXT] = mbar;
  mbar->connect( mbar, &SdPropBar::propChanged, frame, &SdWMain::cmPropertiesChange );

  mbar = new SdPropBarSymPin( QStringLiteral("Sym pin") );
  frame->addToolBar( mbar );
  mbar->setVisible(false);
  mbarTable[PB_SYM_PIN] = mbar;
  mbar->connect( mbar, &SdPropBar::propChanged, frame, &SdWMain::cmPropertiesChange );

  mbar = new SdPropBarPartPin( QStringLiteral("Part pin") );
  frame->addToolBar( mbar );
  mbar->setVisible(false);
  mbarTable[PB_PART_PIN] = mbar;
  mbar->connect( mbar, &SdPropBar::propChanged, frame, &SdWMain::cmPropertiesChange );

  SdPropBarSymImp *isbar = new SdPropBarSymImp( QStringLiteral("Symbol implement") );
  frame->addToolBar( isbar );
  isbar->setVisible(false);
  mbarTable[PB_SYM_IMP] = isbar;
  isbar->connect( isbar, &SdPropBarSymImp::propChanged, frame, &SdWMain::cmPropertiesChange );

  SdPropBarPartImp *ipbar = new SdPropBarPartImp( QStringLiteral("Part implement") );
  frame->addToolBar( ipbar );
  ipbar->setVisible(false);
  mbarTable[PB_PART_IMP] = ipbar;
  ipbar->connect( ipbar, &SdPropBarPartImp::propChanged, frame, &SdWMain::cmPropertiesChange );

  SdPropBarRoad *rbar = new SdPropBarRoad( QStringLiteral("Road") );
  frame->addToolBar( rbar );
  rbar->setVisible(false);
  mbarTable[PB_ROAD] = rbar;
  rbar->connect( rbar, &SdPropBarRoad::propChanged, frame, &SdWMain::cmPropertiesChange );

  SdPropBarPolygon *gbar = new SdPropBarPolygon( QStringLiteral("Polygon") );
  frame->addToolBar( gbar );
  gbar->setVisible(false);
  mbarTable[PB_POLYGON] = gbar;
  gbar->connect( gbar, &SdPropBarRoad::propChanged, frame, &SdWMain::cmPropertiesChange );

  SdPropBarWire *wbar = new SdPropBarWire( QStringLiteral("Wire") );
  frame->addToolBar( wbar );
  wbar->setVisible(false);
  mbarTable[PB_WIRE] = wbar;
  wbar->connect( wbar, &SdPropBarWire::propChanged, frame, &SdWMain::cmPropertiesChange );

  SdPropBarPartPlace *pbar = new SdPropBarPartPlace( QStringLiteral("Part place") );
  frame->addToolBar( pbar );
  pbar->setVisible(false);
  mbarTable[PB_PART_PLACE] = pbar;
  pbar->connect( pbar, &SdPropBarPartPlace::propChanged, frame, &SdWMain::cmPropertiesChange );
  pbar->connect( pbar, &SdPropBarPartPlace::partSelect, frame, &SdWMain::cmModePartSelect );

  SdPropBarDefault *dbar = new SdPropBarDefault( QStringLiteral("No selection") );
  frame->addToolBar( dbar );
  dbar->setVisible(false);
  mbarTable[PB_NO_SELECTION] = dbar;

  SdPropBarRoad *vbar = new SdPropBarRoad( QStringLiteral("Via"), false );
  frame->addToolBar( vbar );
  vbar->setVisible(false);
  mbarTable[PB_VIA] = vbar;
  rbar->connect( vbar, &SdPropBarRoad::propChanged, frame, &SdWMain::cmPropertiesChange );

  for( int i = 0; i < MD_LAST; i++ )
    if( cmModeTable[i] )
      cmModeTable[i]->setCheckable(true);
  }



void SdWCommand::hideEditorContext()
  {
  //Hide all editor-referenced menu  [Погасить все редакторо-зависимые меню insert]
  cmMenuViewPart3d->setVisible(false);
  cmMenuInsertComp->setVisible(false);
  cmMenuInsertPart->setVisible(false);
  cmMenuInsertPart3d->setVisible(false);
  cmMenuInsertPcb->setVisible(false);
  cmMenuInsertSheet->setVisible(false);
  cmMenuInsertSymbol->setVisible(false);
  cmObjectEditEnable->setVisible(false);
  cmObjectEditDisable->setVisible(false);
  cmMenuRules->setVisible(false);

  //Погасить все редакторо-зависимые toolBars
  barComp->hide();
  barPart->hide();
  barPart3d->hide();
  barPart3dView->hide();
  barPcb->hide();
  barSheet->hide();
  barSymbol->hide();
  barView->hide();
  barHelp->hide();
  }




void SdWCommand::selectMode(int md)
  {
  //qDebug() << "selectMode" << md;
  for( int i = 0; i < MD_LAST; i++ )
    if( cmModeTable[i] )
      cmModeTable[i]->setChecked( i == md );
  }









QActionPtr SdWCommand::cmFileNew;
QActionPtr SdWCommand::cmFileOpen;
QActionPtr SdWCommand::cmFileClose;
QActionPtr SdWCommand::cmFileCloseAll;
QActionPtr SdWCommand::cmFileSave;
QActionPtr SdWCommand::cmFileSaveAs;
QActionPtr SdWCommand::cmFileSaveAll;
QActionPtr SdWCommand::cmFilePrint;
QActionPtr SdWCommand::cmFileExit;
QActionPtr SdWCommand::cmFileImportPis;
QActionPtr SdWCommand::cmFileImport;
QActionPtr SdWCommand::cmFileExport;
QActionPtr SdWCommand::cmFileExportGerber;
QActionPtr SdWCommand::cmFileStoreToLibrary;
QActionPtr SdWCommand::cmFileLoadFromLibrary;
QActionPtr SdWCommand::cmFileCloud;


QActionPtr SdWCommand::cmFilePrevious[PREVIOUS_FILES_COUNT];

QActionPtr SdWCommand::cmObjectNew;
QActionPtr SdWCommand::cmObjectLoad;
QActionPtr SdWCommand::cmObjectRename;
QActionPtr SdWCommand::cmObjectDelete;
QActionPtr SdWCommand::cmObjectCopy;
QActionPtr SdWCommand::cmObjectPaste;
QActionPtr SdWCommand::cmObjectCut;
QActionPtr SdWCommand::cmObjectDuplicate;
QActionPtr SdWCommand::cmObjectSort;
QActionPtr SdWCommand::cmObjectParam;
QActionPtr SdWCommand::cmObjectEditEnable;
QActionPtr SdWCommand::cmObjectEditDisable;
QActionPtr SdWCommand::cmProjectParam;
QActionPtr SdWCommand::cmProjectUpgrade;

QActionPtr SdWCommand::cmEditUndo;
QActionPtr SdWCommand::cmEditRedo;
QActionPtr SdWCommand::cmEditCut;
QActionPtr SdWCommand::cmEditCopy;
QActionPtr SdWCommand::cmEditCopySvg;
QActionPtr SdWCommand::cmEditCopyWeb;
QActionPtr SdWCommand::cmEditPaste;
QActionPtr SdWCommand::cmEditDelete;
QActionPtr SdWCommand::cmEditSelectAll;
QActionPtr SdWCommand::cmEditSelectSave;
QActionPtr SdWCommand::cmEditUnSelect;
QActionPtr SdWCommand::cmEditFind;
QActionPtr SdWCommand::cmEditReplace;
QActionPtr SdWCommand::cmEditRotateGroup;
QActionPtr SdWCommand::cmEditProperties;
QActionPtr SdWCommand::cmEditCalculations;
QActionPtr SdWCommand::cmEditFragments;

QActionPtr SdWCommand::cmViewProject;
QActionPtr SdWCommand::cmView3d;
QActionPtr SdWCommand::cmViewMirror;
QActionPtr SdWCommand::cmViewFill;
QActionPtr SdWCommand::cmViewNets;
QActionPtr SdWCommand::cmViewGrid;
QActionPtr SdWCommand::cmViewLayers;

QActionPtr SdWCommand::cm3dMaster;
QActionPtr SdWCommand::cm3dProgram;
QActionPtr SdWCommand::cm3dImportStl;
QActionPtr SdWCommand::cm3dImportStep;
QActionPtr SdWCommand::cm3dImportVrml;
QActionPtr SdWCommand::cm3dShow2d;
QActionPtr SdWCommand::cm3dShowPads;

QActionPtr SdWCommand::cmNetSetup;
QActionPtr SdWCommand::cmModeLink;
QActionPtr SdWCommand::cmModeNetList;
QActionPtr SdWCommand::cmModePack;
QActionPtr SdWCommand::cmPads;

QActionPtr SdWCommand::cmShowRatNet;
QActionPtr SdWCommand::cmShowRuleErrors;
QActionPtr SdWCommand::cmRenumeration;
QActionPtr SdWCommand::cmShowField;
QActionPtr SdWCommand::cmShowPads;
QActionPtr SdWCommand::cmPcbStratum;

QActionPtr SdWCommand::cmRulesEdit;
QActionPtr SdWCommand::cmRulesCheck;
QActionPtr SdWCommand::cmRulesErrorNext;

QActionPtr SdWCommand::cmSymbolPartParam;

QActionPtr SdWCommand::cmSheetExpression;

QActionPtr SdWCommand::cmOption;

QActionPtr SdWCommand::cmTools;

QActionPtr SdWCommand::cmHelpContents;
QActionPtr SdWCommand::cmHelpIndex;
QActionPtr SdWCommand::cmHelpAbout;
QActionPtr SdWCommand::cmHelpRegistration;
QActionPtr SdWCommand::cmHelpHome;
QActionPtr SdWCommand::cmHelpBackward;
QActionPtr SdWCommand::cmHelpForward;

QActionPtr SdWCommand::cmGuiderCapture;
QActionPtr SdWCommand::cmGuiderPause;

QMenu *SdWCommand::menuFile;
QMenu *SdWCommand::menuFilePrevious;
QMenu *SdWCommand::menuObject;
QMenu *SdWCommand::menuEdit;
QMenu *SdWCommand::menuView;
QMenu *SdWCommand::menuViewPart3d;
QMenu *SdWCommand::menuDraw;
QMenu *SdWCommand::menuInsertSymbol;
QMenu *SdWCommand::menuInsertSheet;
QMenu *SdWCommand::menuInsertPart;
QMenu *SdWCommand::menuInsertPart3d;
QMenu *SdWCommand::menuInsertPcb;
QMenu *SdWCommand::menuInsertComp;
QMenu *SdWCommand::menuInstruments;
QMenu *SdWCommand::menuHelp;
QMenu *SdWCommand::menuRules;

QMenu *SdWCommand::menuSelect;

QActionPtr SdWCommand::cmMenuInsertSymbol;
QActionPtr SdWCommand::cmMenuInsertSheet;
QActionPtr SdWCommand::cmMenuInsertPart;
QActionPtr SdWCommand::cmMenuInsertPart3d;
QActionPtr SdWCommand::cmMenuInsertPcb;
QActionPtr SdWCommand::cmMenuInsertComp;
QActionPtr SdWCommand::cmMenuRules;
QActionPtr SdWCommand::cmMenuViewPart3d;

QToolBar *SdWCommand::barMain;
QToolBar *SdWCommand::barSymbol;
QToolBar *SdWCommand::barPart;
QToolBar *SdWCommand::barPart3d;
QToolBar *SdWCommand::barPart3dView;
QToolBar *SdWCommand::barComp;
QToolBar *SdWCommand::barSheet;
QToolBar *SdWCommand::barPcb;
QToolBar *SdWCommand::barView;
QToolBar *SdWCommand::barHelp;


//Full mode action table
QActionPtr SdWCommand::cmModeTable[MD_LAST];

//Full list mode tool bars
QToolBarPtr SdWCommand::mbarTable[PB_LAST];
