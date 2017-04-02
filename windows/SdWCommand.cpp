/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdWCommand.h"
#include "SdWMain.h"
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
  cmFileOpen = menuFile->addAction( QIcon(QString(":/pic/fileOpen.png")), frame->tr("Open project ..."), frame, SLOT(cmFileOpen()) );
  menuFilePrevious = menuFile->addMenu( frame->tr("Previous files") );
  menuFile->addSeparator();
  cmFileClose = menuFile->addAction( QIcon(QString(":/pic/closeFile.png")), frame->tr("Close project"), frame, SLOT(cmFileClose()) );
  cmFileCloseAll = menuFile->addAction( frame->tr("Close all projects"), frame, SLOT(cmFileCloseAll()) );
  menuFile->addSeparator();
  cmFileSave = menuFile->addAction( QIcon(QString(":/pic/save.png")),frame->tr("Save project"), frame, SLOT(cmFileSave()) );
  cmFileSaveAs = menuFile->addAction( QIcon(QString(":/pic/save_as.png")), frame->tr("Save project as..."), frame, SLOT(cmFileSaveAs()) );
  cmFileSaveAll = menuFile->addAction( QIcon(QString(":/pic/save_all.png")), frame->tr("Save all projects"), frame, SLOT(cmFileSaveAll()) );
  menuFile->addSeparator();
  cmFileImport = menuFile->addAction( QIcon(QString(":/pic/fileImport.png")), frame->tr("Import..."), frame, SLOT(cmFileImport()) );
  cmFileExport = menuFile->addAction( frame->tr("Export..."), frame, SLOT(cmFileExport()) );
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
  cmObjectNew = menuObject->addAction( QIcon(QString(":/pic/objectNew.png")), frame->tr("Create"), frame, SLOT(cmObjectNew()) );
  cmObjectRename = menuObject->addAction( QIcon(QString(":/pic/objectRename.png")), frame->tr("Rename"), frame, SLOT(cmObjectRename()) );
  cmObjectDelete = menuObject->addAction( frame->tr("Delete"), frame, SLOT(cmObjectDelete()) );
  cmObjectCopy = menuObject->addAction( frame->tr("Copy"), frame, SLOT(cmObjectCopy()) );
  cmObjectPaste = menuObject->addAction( frame->tr("Paste"), frame, SLOT(cmObjectPaste()) );
  cmObjectCut = menuObject->addAction( frame->tr("Cut"), frame, SLOT(cmObjectCut()) );
  cmObjectSort = menuObject->addAction( frame->tr("Sort"), frame, SLOT(cmObjectSort()) );




  //Меню Редактирование
  menuEdit = new QMenu( frame->tr("Edit") );
  cmEditUndo = menuEdit->addAction( QIcon(QString(":/pic/editUndo.png")), frame->tr("Undo"), frame, SLOT(cmEditUndo()) );
  cmEditRedo = menuEdit->addAction( QIcon(QString(":/pic/editRedo.png")), frame->tr("Redo"), frame, SLOT(cmEditRedo()) );
  menuEdit->addSeparator();
  cmEditCopy = menuEdit->addAction( QIcon(QString(":/pic/editCopy.png")), frame->tr("Copy"), frame, SLOT(cmEditCopy()) );
  cmEditPaste = menuEdit->addAction( QIcon(QString(":/pic/editPaste.png")), frame->tr("Paste"), frame, SLOT(cmEditPaste()) );
  cmEditCut = menuEdit->addAction( QIcon(QString(":/pic/editCut.png")), frame->tr("Cut"), frame, SLOT(cmEditCut()) );
  cmEditDelete = menuEdit->addAction( QIcon(QString(":/pic/editDel.png")), frame->tr("Delete"), frame, SLOT(cmEditDelete()) );
  menuEdit->addSeparator();
  cmEditSelectAll = menuEdit->addAction( QIcon(QString(":/pic/editSelectAll.png")), frame->tr("Select All"), frame, SLOT(cmEditSelectAll()) );
  cmEditFind = menuEdit->addAction( frame->tr("Find"), frame, SLOT(cmEditFind()) );
  cmEditReplace = menuEdit->addAction( frame->tr("Replace"), frame, SLOT(cmEditReplace()) );
  menuEdit->addSeparator();
  cmEditProperties = menuEdit->addAction( QIcon(QString(":/pic/editProp.png")), frame->tr("Properties..."), frame, SLOT(cmEditProperties()) );




  menuView = new QMenu( frame->tr("View") );
  cmViewProject = menuView->addAction( QIcon(QString(":/pic/openProjects.png")), frame->tr("Show-hide project"), frame, SLOT(cmViewProject()) );
  menuView->addSeparator();
  cmViewNets = menuView->addAction( QIcon(QString(":/pic/viewRatnet.png")), frame->tr("Nets"), frame, SLOT(cmViewNets()) );
  cmViewGrid = menuView->addAction( QIcon(QString(":/pic/viewGrid.png")), frame->tr("Grid"), frame, SLOT(cmViewGrid()) );
  cmViewLayers = menuView->addAction( QIcon(QString(":/pic/layers.png")), frame->tr("Layers"), frame, SLOT(cmViewLayers()) );
  menuView->addSeparator();
  cmViewFill = menuView->addAction( QIcon(QString(":/pic/viewFit.png")), frame->tr("Zoom to fit view"), frame, SLOT(cmViewFill()) );
  cmViewZoomIn = menuView->addAction( QIcon(QString(":/pic/viewZoomIn.png")), frame->tr("Zoom in"), frame, SLOT(cmViewZoomIn()) );
  cmViewZoomOut = menuView->addAction( QIcon(QString(":/pic/viewZoomOut.png")), frame->tr("Zoom out"), frame, SLOT(cmViewZoomOut()) );
  cmViewArea = menuView->addAction( QIcon(QString(":/pic/viewWin.png")), frame->tr("Zoom area"), frame, SLOT(cmViewArea()) );
  menuView->addSeparator();
  cmViewMeasurement = menuView->addAction( QIcon(QString(":/pic/viewRuller.png")), frame->tr("Measurement"), frame, SLOT(cmViewMeasurement()) );





  menuDraw = new QMenu( frame->tr("Draw") );
  cmModeSelect        = menuDraw->addAction( QIcon(QString(":/pic/select.png")), frame->tr("Select and edit"), frame, SLOT(cmModeSelect()) );
  menuDraw->addSeparator();
  cmModeLine          = menuDraw->addAction( QIcon(QString(":/pic/drawLine.png")), frame->tr("Line"), frame, SLOT(cmModeLine()) );
  cmModeRect          = menuDraw->addAction( QIcon(QString(":/pic/drawRect.png")), frame->tr("Rect"), frame, SLOT(cmModeRect()) );
  cmModeFilledRect    = menuDraw->addAction( QIcon(QString(":/pic/drawFRect.png")), frame->tr("Filled rect"), frame, SLOT(cmModeFilledRect()) );
  cmModeRegion        = menuDraw->addAction( QIcon(QString(":/pic/drawRegion.png")), frame->tr("Region"), frame, SLOT(cmModeRegion()) );
  cmModeFilledRegion  = menuDraw->addAction( QIcon(QString(":/pic/drawFRegion.png")), frame->tr("Filled region"), frame, SLOT(cmModeFilledRegion()) );
  cmModeCircle        = menuDraw->addAction( QIcon(QString(":/pic/drawCircle.png")), frame->tr("Circle"), frame, SLOT(cmModeCircle()) );
  cmModeFilledCircle  = menuDraw->addAction( QIcon(QString(":/pic/drawFCircle.png")), frame->tr("Filled circle"), frame, SLOT(cmModeFilledCircle()) );
  cmModeArc           = menuDraw->addAction( QIcon(QString(":/pic/drawArc.png")), frame->tr("Arc"), frame, SLOT(cmModeArc()) );
  cmModeText          = menuDraw->addAction( QIcon(QString(":/pic/drawText.png")), frame->tr("Text"), frame, SLOT(cmModeText()) );
  cmModeField         = menuDraw->addAction( QIcon(QString(":/pic/drawField.png")), frame->tr("Field"), frame, SLOT(cmModeField()) );




  menuInsertSymbol = new QMenu( frame->tr("Insert") );

  cmModePin        = menuInsertSymbol->addAction( QIcon(QString(":/pic/objPin.png")), frame->tr("Pin"), frame, SLOT(cmModePin()) );
  cmModeReference  = menuInsertSymbol->addAction( QIcon(QString(":/pic/objIdent.png")), frame->tr("Reference"), frame, SLOT(cmModeReference()) );
  cmModeOrigin     = menuInsertSymbol->addAction( QIcon(QString(":/pic/objOrigin.png")), frame->tr("Origin"), frame, SLOT(cmModeOrigin()) );




  menuInsertPart = new QMenu( frame->tr("Insert") );




  menuInsertPart3d = new QMenu( frame->tr("Insert") );
  //  cmBall           = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmPinWired       = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmPinFlat        = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmBodyCylinder   = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmBodyBrick      = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmBodyContur     = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );




  menuInsertComp = new QMenu( frame->tr("Insert comp") );




  menuInsertSheet = new QMenu( frame->tr("Insert") );
  //  cmModeComponent  = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeNet        = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeBus        = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeDisconnect = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModePcbArea    = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );




  menuInsertPcb = new QMenu( frame->tr("Insert") );
//  cmNetSetup       = menuInsertPcb->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
//  cmPads           = menuInsertPcb->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
//  menuInsertPcb->addSeparator();
  //  cmModeLink       = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeNetName    = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeNetList    = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModePack       = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeLineSize   = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeRadiusSize = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeMovePart   = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModePlace      = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeEditWire   = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeWire       = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModePolygon    = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModeDeleteWire = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );
  //  cmModePad        = menuInsert->addAction( QIcon(QString(":/pic/.png")), frame->tr(""), frame, SLO );




  menuInstruments = new QMenu( frame->tr("Instruments") );
  cmOption = menuInstruments->addAction( frame->tr("Options"), frame, SLOT(cmOption()) );
  menuInstruments->addSeparator();
  cmTools = menuInstruments->addAction( frame->tr("Tools"), frame, SLOT(cmTools()) );



  menuHelp = new QMenu( frame->tr("Help") );
  cmHelpContens = menuHelp->addAction( QIcon(QString(":/pic/help.png")), frame->tr("Contens"), frame, SLOT(cmHelpContens()) );
  cmHelpIndex   = menuHelp->addAction( QIcon(QString(":/pic/helpContext.png")), frame->tr("Index"), frame, SLOT(cmHelpIndex()) );
  cmHelpAbout   = menuHelp->addAction( QIcon(QString(":/pic/about.png")), frame->tr("About"), frame, SLOT(cmHelpAbout()) );


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
  bar->addMenu( menuInstruments );
  bar->addMenu( menuHelp );

  projectState(false);
  }





void SdWCommand::updatePreviousMenu()
  {
  //Заполнить меню предыдущими файлами
  QSettings settings;
  QStringList files = settings.value(SDK_PREVIOUS_FILES).toStringList();

  int numRecentFiles = qMin(files.size(), (int)PREVIOUS_FILES_COUNT );

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
    cmObjectDelete->setEnabled(enable);
    cmObjectCopy->setEnabled(enable);
    cmObjectCut->setEnabled(enable);

    cmObjectPaste->setEnabled(enable);
    }
  cmFileClose->setEnabled(enable);
  cmFileCloseAll->setEnabled(enable);
  cmFileSaveAs->setEnabled(enable);
  cmFileSaveAll->setEnabled(enable);

  cmObjectNew->setEnabled(enable);
  cmObjectSort->setEnabled(enable);
  }




void SdWCommand::addEditCommands(QToolBar *bar)
  {
  bar->insertAction( 0, cmEditCopy );
  bar->insertAction( 0, cmEditCut );
  bar->insertAction( 0, cmEditPaste );
  bar->insertAction( 0, cmEditDelete );
  bar->insertAction( 0, cmEditProperties );
//  bar->addSeparator();
  bar->insertAction( 0, cmEditUndo );
  bar->insertAction( 0, cmEditRedo );
  }

void SdWCommand::addViewCommands(QToolBar *bar)
  {
  bar->addSeparator();
  bar->insertAction( 0, cmViewLayers );
  bar->insertAction( 0, cmViewGrid );
//  bar->addSeparator();
  bar->insertAction( 0, cmViewFill );
  bar->insertAction( 0, cmViewZoomIn );
  bar->insertAction( 0, cmViewZoomOut );
  bar->insertAction( 0, cmViewArea );
  bar->insertAction( 0, cmViewMeasurement );
  }

void SdWCommand::addDrawCommands(QToolBar *bar)
  {
  bar->addSeparator();
  bar->insertAction( 0, cmModeSelect );
//  bar->addSeparator();
  bar->insertAction( 0, cmModeLine );
  bar->insertAction( 0, cmModeRect );
  bar->insertAction( 0, cmModeFilledRect );
  bar->insertAction( 0, cmModeRegion );
  bar->insertAction( 0, cmModeFilledRegion );
  bar->insertAction( 0, cmModeCircle );
  //bar->insertAction( 0, cmModeFilledCircle );
  bar->insertAction( 0, cmModeArc );
  bar->insertAction( 0, cmModeText );
  //bar->insertAction( 0, cmModeField );
  bar->addSeparator();
  }


void SdWCommand::createToolBars(SdWMain *frame)
  {
  //Main bar
  barMain = new QToolBar( QString("Main") );
  barMain->insertAction( 0, cmFileNew );
  barMain->insertAction( 0, cmFileOpen );
  barMain->insertAction( 0, cmFileSave );
  barMain->insertAction( 0, cmFilePrint );
//  barMain->addSeparator();
  barMain->insertAction( 0, cmFileExit );
//  barMain->addSeparator();
  barMain->insertAction( 0, cmObjectNew );

  frame->addToolBar( barMain );
  //barMain->setIconSize( QSize(20,24) );

  //Symbol bar
  barSymbol = new QToolBar( QString("Symbol") );
  addEditCommands( barSymbol );
  addViewCommands( barSymbol );
  addDrawCommands( barSymbol );
  barSymbol->insertAction( 0, cmModePin );
  barSymbol->insertAction( 0, cmModeReference );
  barSymbol->insertAction( 0, cmModeOrigin );

  frame->addToolBar( barSymbol );


  //Part bar
  barPart = new QToolBar( QString("Part") );
  addEditCommands( barPart );
  addViewCommands( barPart );
  addDrawCommands( barPart );

  frame->addToolBar( barPart );


  //Comp bar
  barComp = new QToolBar( QString("Component") );

  frame->addToolBar( barComp );



  //Sheet bar
  barSheet = new QToolBar( QString("Sheet") );
  addEditCommands( barSheet );
  addViewCommands( barSheet );
  addDrawCommands( barSheet );

  frame->addToolBar( barSheet );



  //PCB bar
  barPcb = new QToolBar( QString("PCB") );
  addEditCommands( barPcb );
  addViewCommands( barPcb );
  addDrawCommands( barPcb );

  frame->addToolBar( barPcb );

  frame->addToolBarBreak();


  mbarLinear = new QToolBar( QString("Linear mode") );
  QComboBox *combo = new QComboBox();
  mbarLinear->addWidget( combo );

  frame->addToolBar( mbarLinear );
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

  //Погасить все редакторо-зависимые toolBars
  barComp->hide();
  barPart->hide();
  barPcb->hide();
  barSheet->hide();
  barSymbol->hide();
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
QActionPtr SdWCommand::cmFileImport;
QActionPtr SdWCommand::cmFileExport;

QActionPtr SdWCommand::cmFilePrevious[PREVIOUS_FILES_COUNT];

QActionPtr SdWCommand::cmObjectNew;
QActionPtr SdWCommand::cmObjectRename;
QActionPtr SdWCommand::cmObjectDelete;
QActionPtr SdWCommand::cmObjectCopy;
QActionPtr SdWCommand::cmObjectPaste;
QActionPtr SdWCommand::cmObjectCut;
QActionPtr SdWCommand::cmObjectSort;

QActionPtr SdWCommand::cmEditUndo;
QActionPtr SdWCommand::cmEditRedo;
QActionPtr SdWCommand::cmEditCut;
QActionPtr SdWCommand::cmEditCopy;
QActionPtr SdWCommand::cmEditPaste;
QActionPtr SdWCommand::cmEditDelete;
QActionPtr SdWCommand::cmEditSelectAll;
QActionPtr SdWCommand::cmEditFind;
QActionPtr SdWCommand::cmEditReplace;
QActionPtr SdWCommand::cmEditProperties;

QActionPtr SdWCommand::cmViewProject;
QActionPtr SdWCommand::cmViewFill;
QActionPtr SdWCommand::cmViewNets;
QActionPtr SdWCommand::cmViewGrid;
QActionPtr SdWCommand::cmViewLayers;
QActionPtr SdWCommand::cmViewZoomIn;
QActionPtr SdWCommand::cmViewZoomOut;
QActionPtr SdWCommand::cmViewArea;
QActionPtr SdWCommand::cmViewMeasurement;

QActionPtr SdWCommand::cmModeLine;
QActionPtr SdWCommand::cmModeRect;
QActionPtr SdWCommand::cmModeFilledRect;
QActionPtr SdWCommand::cmModeRegion;
QActionPtr SdWCommand::cmModeFilledRegion;
QActionPtr SdWCommand::cmModeCircle;
QActionPtr SdWCommand::cmModeFilledCircle;
QActionPtr SdWCommand::cmModeArc;
QActionPtr SdWCommand::cmModeText;
QActionPtr SdWCommand::cmModeField;

QActionPtr SdWCommand::cmModeSelect;
QActionPtr SdWCommand::cmMode3dBall;
QActionPtr SdWCommand::cmMode3dPinWired;
QActionPtr SdWCommand::cmMode3dPinFlat;
QActionPtr SdWCommand::cmMode3dBodyCylinder;
QActionPtr SdWCommand::cmMode3dBodyBrick;
QActionPtr SdWCommand::cmMode3dBodyContur;
QActionPtr SdWCommand::cmModePin;
QActionPtr SdWCommand::cmModeReference;
QActionPtr SdWCommand::cmModeOrigin;
QActionPtr SdWCommand::cmModeComponent;
QActionPtr SdWCommand::cmModeNet;
QActionPtr SdWCommand::cmNetSetup;
QActionPtr SdWCommand::cmModeBus;
QActionPtr SdWCommand::cmModeDisconnect;
QActionPtr SdWCommand::cmModePcbArea;
QActionPtr SdWCommand::cmModeLink;
QActionPtr SdWCommand::cmModeNetName;
QActionPtr SdWCommand::cmModeNetList;
QActionPtr SdWCommand::cmModePack;
QActionPtr SdWCommand::cmPads;
QActionPtr SdWCommand::cmModeLineSize;
QActionPtr SdWCommand::cmModeRadiusSize;
QActionPtr SdWCommand::cmModeMovePart;
QActionPtr SdWCommand::cmModePlace;
QActionPtr SdWCommand::cmModeEditWire;
QActionPtr SdWCommand::cmModeWire;
QActionPtr SdWCommand::cmModePolygon;
QActionPtr SdWCommand::cmModeDeleteWire;
QActionPtr SdWCommand::cmModePad;

QActionPtr SdWCommand::cmOption;

QActionPtr SdWCommand::cmTools;

QActionPtr SdWCommand::cmHelpContens;
QActionPtr SdWCommand::cmHelpIndex;
QActionPtr SdWCommand::cmHelpAbout;

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

QActionPtr SdWCommand::cmMenuInsertSymbol;
QActionPtr SdWCommand::cmMenuInsertSheet;
QActionPtr SdWCommand::cmMenuInsertPart;
QActionPtr SdWCommand::cmMenuInsertPart3d;
QActionPtr SdWCommand::cmMenuInsertPcb;
QActionPtr SdWCommand::cmMenuInsertComp;

QToolBar *SdWCommand::barMain;
QToolBar *SdWCommand::barSymbol;
QToolBar *SdWCommand::barPart;
QToolBar *SdWCommand::barComp;
QToolBar *SdWCommand::barSheet;
QToolBar *SdWCommand::barPcb;

QToolBar *SdWCommand::mbarLinear;
