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
  //Меню Файл
  menuFile = new QMenu( frame->tr("File") );




  cmFileNew = menuFile->addAction( QIcon(QString(":/pic/new.png")), frame->tr("New project"), frame, SLOT(cmFileNew()) );
  cmFileLoadFromLibrary = menuFile->addAction( QIcon(QString(":/pic/download.png")), frame->tr("Load from library..."), frame, SLOT(cmFileLoad()) );
  cmFileOpen = menuFile->addAction( QIcon(QString(":/pic/fileOpen.png")), frame->tr("Open project ..."), frame, SLOT(cmFileOpen()) );
  menuFilePrevious = menuFile->addMenu( frame->tr("Previous files") );
  menuFile->addSeparator();
  cmFileClose = menuFile->addAction( QIcon(QString(":/pic/closeFile.png")), frame->tr("Close project"), frame, SLOT(cmFileClose()) );
  cmFileCloseAll = menuFile->addAction( QIcon(QString(":/pic/fileCloseAll.png")), frame->tr("Close all projects"), frame, SLOT(cmFileCloseAll()) );
  menuFile->addSeparator();
  cmFileSave = menuFile->addAction( QIcon(QString(":/pic/save.png")),frame->tr("Save project"), frame, SLOT(cmFileSave()) );
  cmFileSaveAs = menuFile->addAction( QIcon(QString(":/pic/save_as.png")), frame->tr("Save project as..."), frame, SLOT(cmFileSaveAs()) );
  cmFileSaveAll = menuFile->addAction( QIcon(QString(":/pic/save_all.png")), frame->tr("Save all projects"), frame, SLOT(cmFileSaveAll()) );
  cmFileStoreToLibrary = menuFile->addAction( QIcon(QString(":/pic/upload.png")),frame->tr("Store to library..."), frame, SLOT(cmFileStore()) );
  menuFile->addSeparator();
  cmFileImportPis = menuFile->addAction( frame->tr("Import PasCAD"), frame, SLOT(cmFileImportPis()) );
  cmFileImport    = menuFile->addAction( QIcon(QString(":/pic/fileImport.png")), frame->tr("Import..."), frame, SLOT(cmFileImport()) );
  cmFileExport    = menuFile->addAction( QIcon(QString(":/pic/fileExport.png")), frame->tr("Export..."), frame, SLOT(cmFileExport()) );
  menuFile->addSeparator();
  cmFilePrint = menuFile->addAction( QIcon(QString(":/pic/filePrint.png")), frame->tr("Print..."), frame, SLOT(cmFilePrint()) );
  menuFile->addSeparator();
  cmFileExit = menuFile->addAction( QIcon(QString(":/pic/exit.png")), frame->tr("Exit programm"), frame, SLOT(cmFileExit()) );

  //Меню для предыдущих файлов
  for (int i = 0; i < PREVIOUS_FILES_COUNT; ++i)
    {
    cmFilePrevious[i] = new QAction(frame);
    cmFilePrevious[i]->setVisible(false);
    frame->connect(cmFilePrevious[i], SIGNAL(triggered()), frame, SLOT(cmFilePrevious()) );
    menuFilePrevious->addAction(cmFilePrevious[i]);
    }

  updatePreviousMenu();


  //Меню Объект
  menuObject = new QMenu( frame->tr("Objects") );
  cmObjectNew         = menuObject->addAction( QIcon(QString(":/pic/objectNew.png")), frame->tr("Create..."), frame, SLOT(cmObjectNew()) );
  cmObjectLoad        = menuObject->addAction( QIcon(QString(":/pic/download.png")), frame->tr("Load from library..."), frame, SLOT(cmObjectLoad()) );
  cmObjectRename      = menuObject->addAction( QIcon(QString(":/pic/objectRename.png")), frame->tr("Rename..."), frame, SLOT(cmObjectRename()) );
  cmObjectParam       = menuObject->addAction( QIcon(QString(":/pic/objectParam.png")), frame->tr("Edit object param..."), frame, SLOT(cmObjectParam()) );
  cmObjectDelete      = menuObject->addAction( QIcon(QString(":/pic/objectDelete.png")), frame->tr("Delete"), frame, SLOT(cmObjectDelete()) );
  cmObjectCopy        = menuObject->addAction( QIcon(QString(":/pic/objectCopy.png")), frame->tr("Copy"), frame, SLOT(cmObjectCopy()) );
  cmObjectPaste       = menuObject->addAction( QIcon(QString(":/pic/objectPaste.png")), frame->tr("Paste"), frame, SLOT(cmObjectPaste()) );
  cmObjectCut         = menuObject->addAction( QIcon(QString(":/pic/objectCut.png")), frame->tr("Cut"), frame, SLOT(cmObjectCut()) );
  cmObjectDuplicate   = menuObject->addAction( QIcon(QString(":/pic/duplicate.png")), frame->tr("Duplicate"), frame, SLOT(cmObjectDuplicate()) );
  cmObjectSort        = menuObject->addAction( QIcon(QString(":/pic/objectSort.png")), frame->tr("Sort"), frame, SLOT(cmObjectSort()) );
  cmObjectEditEnable  = menuObject->addAction( QIcon(QString(":/pic/objectEditEnable.png")), frame->tr("Enable edit"), frame, SLOT(cmObjectEditEnable()) );
  cmObjectEditDisable = menuObject->addAction( QIcon(QString(":/pic/objectEditDisable.png")), frame->tr("Finish edit"), frame, SLOT(cmObjectEditDisable()) );
  cmProjectParam      = menuObject->addAction( QIcon(QString(":/pic/editProjectParam.png")), frame->tr("Edit project param..."), frame, SLOT(cmProjectParam()) );
  cmProjectUpgrade    = menuObject->addAction( QIcon(QString(":/pic/editProjectUpgrade.png")), frame->tr("Upgrade with new versions"), frame, SLOT(cmProjectUpgrade()) );




  //Меню Редактирование
  menuEdit = new QMenu( frame->tr("Edit") );
  cmEditUndo = menuEdit->addAction( QIcon(QString(":/pic/editUndo.png")), frame->tr("Undo"), frame, SLOT(cmEditUndo()) );
  cmEditUndo->setEnabled(false);
  cmEditRedo = menuEdit->addAction( QIcon(QString(":/pic/editRedo.png")), frame->tr("Redo"), frame, SLOT(cmEditRedo()) );
  cmEditRedo->setEnabled(false);
  menuEdit->addSeparator();
  cmEditCopy = menuEdit->addAction( QIcon(QString(":/pic/editCopy.png")), frame->tr("Copy"), frame, SLOT(cmEditCopy()) );
  cmEditPaste = menuEdit->addAction( QIcon(QString(":/pic/editPaste.png")), frame->tr("Paste"), frame, SLOT(cmEditPaste()) );
  cmEditCut = menuEdit->addAction( QIcon(QString(":/pic/editCut.png")), frame->tr("Cut"), frame, SLOT(cmEditCut()) );
  cmEditDelete = menuEdit->addAction( QIcon(QString(":/pic/editDel.png")), frame->tr("Delete"), frame, SLOT(cmEditDelete()) );
  menuEdit->addSeparator();
  cmEditSelectAll = menuEdit->addAction( QIcon(QString(":/pic/editSelectAll.png")), frame->tr("Select All"), frame, SLOT(cmEditSelectAll()) );
  cmEditUnSelect  = menuEdit->addAction( QIcon(QString(":/pic/editSelectAll.png")), frame->tr("UnSelect"), frame, SLOT(cmEditUnSelect()) );
  cmEditFind = menuEdit->addAction( QIcon(QString(":/pic/editFind.png")), frame->tr("Find"), frame, SLOT(cmEditFind()) );
  cmEditReplace = menuEdit->addAction( frame->tr("Replace"), frame, SLOT(cmEditReplace()) );
  menuEdit->addSeparator();
  cmEditProperties = menuEdit->addAction( QIcon(QString(":/pic/editProp.png")), frame->tr("Params..."), frame, SLOT(cmEditProperties()) );




  menuView = new QMenu( frame->tr("View") );
  cmViewProject = menuView->addAction( QIcon(QString(":/pic/openProjects.png")), frame->tr("Show-hide project"), frame, SLOT(cmViewProject()) );
  menuView->addSeparator();
  cmViewNets = menuView->addAction( QIcon(QString(":/pic/viewRatnet.png")), frame->tr("Nets"), frame, SLOT(cmViewNets()) );
  cmViewGrid = menuView->addAction( QIcon(QString(":/pic/viewGrid.png")), frame->tr("Grid"), frame, SLOT(cmViewGrid()) );
  cmViewLayers = menuView->addAction( QIcon(QString(":/pic/layers.png")), frame->tr("Layers"), frame, SLOT(cmViewLayers()) );
  menuView->addSeparator();
  cmViewFill = menuView->addAction( QIcon(QString(":/pic/viewFit.png")), frame->tr("Zoom to fit view"), frame, SLOT(cmViewFill()) );
  cmModeTable[MD_ZOOM_IN]     = menuView->addAction( QIcon(QString(":/pic/viewZoomIn.png")), frame->tr("Zoom in"), frame, SLOT(cmViewZoomIn()) );
  cmModeTable[MD_ZOOM_OUT]    = menuView->addAction( QIcon(QString(":/pic/viewZoomOut.png")), frame->tr("Zoom out"), frame, SLOT(cmViewZoomOut()) );
  cmModeTable[MD_ZOOM_WIN]   = menuView->addAction( QIcon(QString(":/pic/viewWin.png")), frame->tr("Zoom area"), frame, SLOT(cmViewArea()) );
  menuView->addSeparator();
  cmModeTable[MD_MEASUREMENT] = menuView->addAction( QIcon(QString(":/pic/viewRuller.png")), frame->tr("Measurement"), frame, SLOT(cmViewMeasurement()) );




  menuDraw = new QMenu( frame->tr("Draw") );
  cmModeTable[MD_SELECT]      = menuDraw->addAction( QIcon(QString(":/pic/select.png")), frame->tr("Select and edit"), frame, SLOT(cmModeSelect()) );
  menuDraw->addSeparator();
  cmModeTable[MD_LINE]        = menuDraw->addAction( QIcon(QString(":/pic/drawLine.png")), frame->tr("Line"), frame, SLOT(cmModeLine()) );
  cmModeTable[MD_RECT]        = menuDraw->addAction( QIcon(QString(":/pic/drawRect.png")), frame->tr("Rect"), frame, SLOT(cmModeRect()) );
  cmModeTable[MD_FILL_RECT]   = menuDraw->addAction( QIcon(QString(":/pic/drawFRect.png")), frame->tr("Filled rect"), frame, SLOT(cmModeFilledRect()) );
  cmModeTable[MD_REGION]      = menuDraw->addAction( QIcon(QString(":/pic/drawRegion.png")), frame->tr("Region"), frame, SLOT(cmModeRegion()) );
  cmModeTable[MD_FILL_REGION] = menuDraw->addAction( QIcon(QString(":/pic/drawFRegion.png")), frame->tr("Filled region"), frame, SLOT(cmModeFilledRegion()) );
  cmModeTable[MD_CIRCLE]      = menuDraw->addAction( QIcon(QString(":/pic/drawCircle.png")), frame->tr("Circle"), frame, SLOT(cmModeCircle()) );
  cmModeTable[MD_FILL_CIRCLE] = menuDraw->addAction( QIcon(QString(":/pic/drawFCircle.png")), frame->tr("Filled circle"), frame, SLOT(cmModeFilledCircle()) );
  cmModeTable[MD_ARC]         = menuDraw->addAction( QIcon(QString(":/pic/drawArc.png")), frame->tr("Arc"), frame, SLOT(cmModeArc()) );
  cmModeTable[MD_TEXT]        = menuDraw->addAction( QIcon(QString(":/pic/drawText.png")), frame->tr("Text"), frame, SLOT(cmModeText()) );
  cmModeTable[MD_FIELD]       = menuDraw->addAction( QIcon(QString(":/pic/drawField.png")), frame->tr("Field"), frame, SLOT(cmModeField()) );
  cmShowField = menuDraw->addAction( QIcon(QStringLiteral(":/pic/showField.png")), frame->tr("Show fields or values") );
  cmShowField->setCheckable(true);
  cmShowField->setChecked( sdEnvir->mShowFields );
  cmShowField->connect( cmShowField, &QAction::toggled, frame, &SdWMain::cmShowFields );




  menuInsertSymbol = new QMenu( frame->tr("Symbol") );

  cmModeTable[MD_SYM_PIN]       = menuInsertSymbol->addAction( QIcon(QString(":/pic/objPin.png")), frame->tr("Insert pin"), frame, SLOT(cmModePin()) );
  cmModeTable[MD_SYM_IDENT]     = menuInsertSymbol->addAction( QIcon(QString(":/pic/objIdent.png")), frame->tr("Edit reference"), frame, SLOT(cmModeReference()) );
  cmModeTable[MD_SYM_ORIGIN]    = menuInsertSymbol->addAction( QIcon(QString(":/pic/objOrigin.png")), frame->tr("Edit origin"), frame, SLOT(cmModeOrigin()) );
  cmModeTable[MD_SYM_VALUE]     = menuInsertSymbol->addAction( QIcon(QString(":/pic/objValue.png")), frame->tr("Edit value"), frame, SLOT(cmModeValue()) );




  menuInsertPart = new QMenu( frame->tr("Part") );

  cmShowPads                     = menuInsertPart->addAction( QIcon(QString(":/pic/objVia.png")), frame->tr("Show pads") );
  cmShowPads->setCheckable(true);
  cmShowPads->setChecked( sdEnvir->mShowPads );
  cmShowPads->connect( cmShowPads, &QAction::toggled, frame, &SdWMain::cmShowPads );
  cmModeTable[MD_PART_PIN]       = menuInsertPart->addAction( QIcon(QString(":/pic/objPrtPin.png")), frame->tr("Insert pin"), frame, SLOT(cmModePin()) );
  cmModeTable[MD_PART_IDENT]     = menuInsertPart->addAction( QIcon(QString(":/pic/objIdent.png")), frame->tr("Edit reference"), frame, SLOT(cmModeReference()) );
  cmModeTable[MD_PART_ORIGIN]    = menuInsertPart->addAction( QIcon(QString(":/pic/objOrigin.png")), frame->tr("Edit origin"), frame, SLOT(cmModeOrigin()) );
  cmModeTable[MD_PART_VALUE]     = menuInsertPart->addAction( QIcon(QString(":/pic/objPrtValue.png")), frame->tr("Edit value"), frame, SLOT(cmModeValue()) );




  menuInsertPart3d = new QMenu( frame->tr("Insert") );
  //  cmBall           = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmPinWired       = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmPinFlat        = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmBodyCylinder   = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmBodyBrick      = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmBodyContur     = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );




  menuInsertComp = new QMenu( frame->tr("Insert comp") );




  menuInsertSheet = new QMenu( frame->tr("Sheet") );
  cmRenumeration              = menuInsertSheet->addAction( QIcon(QStringLiteral(":/pic/renumeration.png")), frame->tr("Renumeration"), frame, SLOT(cmRenumeration()) );
  menuInsertSheet->addSeparator();
  cmModeTable[MD_FRAGMENT]    = menuInsertSheet->addAction( QIcon(QString(":/pic/iconSheet.png")), frame->tr("Insert fragment"), frame, SLOT(cmModeFragment()) );
  cmModeTable[MD_COMPONENT]   = menuInsertSheet->addAction( QIcon(QString(":/pic/objComp.png")), frame->tr("Insert component"), frame, SLOT(cmModeComponent()) );
  cmModeTable[MD_NET]         = menuInsertSheet->addAction( QIcon(QString(":/pic/objWire.png")), frame->tr("Insert net wire"), frame, SLOT(cmModeNet()) );
  cmModeTable[MD_NET_NAME]    = menuInsertSheet->addAction( QIcon(QString(":/pic/objWireName.png")), frame->tr("Insert net name"), frame, SLOT(cmModeNetName()) );
  cmModeTable[MD_BUS]         = menuInsertSheet->addAction( QIcon(QString(":/pic/objBus.png")), frame->tr("Insert bus (many net)"), frame, SLOT(cmModeBus()) );
  cmModeTable[MD_DISCONNECT]  = menuInsertSheet->addAction( QIcon(QString(":/pic/objUnConnect.png")), frame->tr("Disconnect net"), frame, SLOT(cmModeDisconnect()) );
  cmModeTable[MD_SHEET_IDENT] = menuInsertSheet->addAction( QIcon(QString(":/pic/objIdent.png")), frame->tr("Move ident of components"), frame, SLOT(cmModeReferenceMove()) );
  cmModeTable[MD_SHEET_VALUE] = menuInsertSheet->addAction( QIcon(QString(":/pic/objValue.png")), frame->tr("Move value of components"), frame, SLOT(cmModeValueMove()) );
  //cmModeTable[MD_PCB_AREA]   = menuInsertSheet->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
//  cmModeTable[MD_FIELD]      = menuInsertSheet->addAction( QIcon(QString(":/pic/objField.png")), frame->tr("Field"), frame, SLOT(cmModeF) );




  menuInsertPcb = new QMenu( frame->tr("Plate") );
  menuInsertPcb->insertAction( nullptr, cmRenumeration );
  menuInsertPcb->insertSeparator(nullptr);
  cmShowRatNet = menuInsertPcb->addAction( QIcon(QStringLiteral(":/pic/viewRatnet.png")), frame->tr("Show rat net") );
  cmShowRatNet->setCheckable(true);
  cmShowRatNet->setChecked( sdEnvir->mShowRatNet );
  cmShowRatNet->connect( cmShowRatNet, &QAction::toggled, frame, &SdWMain::cmShowRatNet );
  cmPads = menuInsertPcb->addAction( QIcon(QStringLiteral(":/pic/pads.png")), frame->tr("Pads association"), frame, SLOT(cmPads()) );
  //cmShowRatNet->co
//  cmNetSetup       = menuInsertPcb->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
//  cmPads           = menuInsertPcb->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
//  menuInsertPcb->addSeparator();
  //  cmModeLink       = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeNetName    = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeNetList    = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  // cmModePack       = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeLineSize   = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeRadiusSize = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  cmModeTable[MD_PART_IMP]   = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrt.png")), frame->tr("Insert part"), frame, SLOT(cmModeComponent()) );
  cmModeTable[MD_MOVE_PART]  = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrtPlace.png")), frame->tr("Move part"), frame, SLOT(cmModeMovePart()) );
  cmModeTable[MD_ROAD_ENTER] = menuInsertPcb->addAction( QIcon(QString(":/pic/objPcbWire.png")), frame->tr("Road enter"), frame, SLOT(cmModeRoadEnter()) );
  cmModeTable[MD_POLYGON]    = menuInsertPcb->addAction( QIcon(QString(":/pic/objPolygon.png")), frame->tr("Polygon enter"), frame, SLOT(cmModePolygon()) );
  cmModeTable[MD_VIA_ENTER]  = menuInsertPcb->addAction( QIcon(QString(":/pic/objVia.png")), frame->tr("Via enter"), frame, SLOT(cmModeViaEnter()) );
  //  cmModePlace      = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeEditWire   = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeWire       = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeDeleteWire = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModePad        = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  cmModeTable[MD_PLATE_IDENT] = menuInsertPcb->addAction( QIcon(QString(":/pic/objIdent.png")), frame->tr("Move ident of components"), frame, SLOT(cmModeReferenceMove()) );
  cmModeTable[MD_PLATE_VALUE] = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrtValue.png")), frame->tr("Move value of components"), frame, SLOT(cmModeValueMove()) );



  menuRules = new QMenu( frame->tr("Rules") );
  cmRulesEdit = menuRules->addAction( QIcon(QStringLiteral(":/pic/rules.png")), frame->tr("Edit rules dialog"), frame, SLOT(cmRulesEdit()) );
  cmShowRuleErrors = menuRules->addAction( QIcon(QStringLiteral(":/pic/objShowRuleErrors.png")), frame->tr("Show rule errors") );
  cmShowRuleErrors->setCheckable(true);
  cmShowRuleErrors->setChecked( sdEnvir->mShowRuleErrors );
  cmShowRuleErrors->connect( cmShowRuleErrors, &QAction::toggled, frame, &SdWMain::cmShowRuleErrors );
  cmRulesCheck = menuRules->addAction( QIcon(QStringLiteral(":/pic/objRulesCheck.png")), frame->tr("Check all rules"), frame, SLOT(cmRulesCheck()) );
  cmRulesErrorNext = menuRules->addAction( QIcon(QStringLiteral(":/pic/rulesNextError.png")), frame->tr("Center next rules error"), frame, SLOT(cmRulesErrorNext()) );



  menuInstruments = new QMenu( frame->tr("Instruments") );
  cmOption = menuInstruments->addAction( QIcon(QString(":/pic/instrumOptions.png")), frame->tr("Options"), frame, SLOT(cmOption()) );
  menuInstruments->addSeparator();
  cmTools  = menuInstruments->addAction( QIcon(QString(":/pic/instrumTools.png")), frame->tr("Tools"), frame, SLOT(cmTools()) );
  menuInstruments->addSeparator();
  cmGuiderCapture    = menuInstruments->addAction( frame->tr("Capture start-stop"), frame, SLOT(cmGuiderCapture()), QKeySequence(Qt::Key_F10) );
  cmGuiderPause      = menuInstruments->addAction( frame->tr("Capture pause-resume"), frame, SLOT(cmGuiderPause()), QKeySequence(Qt::Key_F11) );



  menuHelp = new QMenu( frame->tr("Help") );
  cmHelpContens      = menuHelp->addAction( QIcon(QString(":/pic/help.png")), frame->tr("Contens"), frame, SLOT(cmHelpContens()) );
  cmHelpIndex        = menuHelp->addAction( QIcon(QString(":/pic/helpContext.png")), frame->tr("Index"), frame, SLOT(cmHelpIndex()) );
  cmHelpAbout        = menuHelp->addAction( QIcon(QString(":/pic/helpAbout.png")), frame->tr("About"), frame, SLOT(cmHelpAbout()) );
  cmHelpRegistration = menuHelp->addAction( QIcon(QString(":/pic/helpRegistration.png")), frame->tr("Registration"), frame, SLOT(cmHelpRegistration()) );
  cmHelpHome         = new QAction( QIcon(QString(":/pic/helpHome.png")), frame->tr("Home help page") );
  frame->connect( cmHelpHome, &QAction::triggered, frame, &SdWMain::cmHelpContens );
  cmHelpBackward     = new QAction( QIcon(QString(":/pic/helpPrevious.png")), frame->tr("Backward help page") );
  frame->connect( cmHelpBackward, &QAction::triggered, frame, &SdWMain::cmHelpBackward );
  cmHelpForward      = new QAction( QIcon(QString(":/pic/helpNext.png")), frame->tr("Forward help page") );
  frame->connect( cmHelpForward, &QAction::triggered, frame, &SdWMain::cmHelpForward );


  menuSelect = new QMenu( frame->tr("Select") );
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
  menuSelect->addSeparator();
  menuSelect->insertAction( nullptr, cmEditProperties );



  QMenuBar *bar = frame->menuBar();
  bar->addMenu( menuFile );
  bar->addMenu( menuObject );
  bar->addMenu( menuEdit );
  bar->addMenu( menuView );
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
  barPcb->insertAction( nullptr, cmModeTable[MD_POLYGON] );
  barPcb->insertAction( nullptr, cmModeTable[MD_VIA_ENTER] );
  barPcb->insertAction( nullptr, cmModeTable[MD_PLATE_IDENT] );
  barPcb->insertAction( nullptr, cmModeTable[MD_PLATE_VALUE] );


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
  //Погасить все редакторо-зависимые меню insert
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
QActionPtr SdWCommand::cmEditPaste;
QActionPtr SdWCommand::cmEditDelete;
QActionPtr SdWCommand::cmEditSelectAll;
QActionPtr SdWCommand::cmEditUnSelect;
QActionPtr SdWCommand::cmEditFind;
QActionPtr SdWCommand::cmEditReplace;
QActionPtr SdWCommand::cmEditProperties;

QActionPtr SdWCommand::cmViewProject;
QActionPtr SdWCommand::cmViewFill;
QActionPtr SdWCommand::cmViewNets;
QActionPtr SdWCommand::cmViewGrid;
QActionPtr SdWCommand::cmViewLayers;

QActionPtr SdWCommand::cmMode3dBall;
QActionPtr SdWCommand::cmMode3dPinWired;
QActionPtr SdWCommand::cmMode3dPinFlat;
QActionPtr SdWCommand::cmMode3dBodyCylinder;
QActionPtr SdWCommand::cmMode3dBodyBrick;
QActionPtr SdWCommand::cmMode3dBodyContur;
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

QActionPtr SdWCommand::cmRulesEdit;
QActionPtr SdWCommand::cmRulesCheck;
QActionPtr SdWCommand::cmRulesErrorNext;


QActionPtr SdWCommand::cmOption;

QActionPtr SdWCommand::cmTools;

QActionPtr SdWCommand::cmHelpContens;
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

QToolBar *SdWCommand::barMain;
QToolBar *SdWCommand::barSymbol;
QToolBar *SdWCommand::barPart;
QToolBar *SdWCommand::barComp;
QToolBar *SdWCommand::barSheet;
QToolBar *SdWCommand::barPcb;
QToolBar *SdWCommand::barView;
QToolBar *SdWCommand::barHelp;


//Full mode action table
QActionPtr SdWCommand::cmModeTable[MD_LAST];

//Full list mode tool bars
QToolBarPtr SdWCommand::mbarTable[PB_LAST];