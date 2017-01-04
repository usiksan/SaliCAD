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



void SdWCommand::createMenu(SdWMain *frame)
  {
  //Меню Файл
  menuFile = new QMenu( frame->tr("File") );




  cmFileNew = menuFile->addAction( frame->tr("New project"), frame, SLOT(cmFileNew()) );
  cmFileOpen = menuFile->addAction( frame->tr("Open project ..."), frame, SLOT(cmFileOpen()) );
  menuFilePrevious = menuFile->addMenu( frame->tr("Previous files") );
  menuFile->addSeparator();
  cmFileClose = menuFile->addAction( frame->tr("Close project"), frame, SLOT(cmFileClose()) );
  cmFileCloseAll = menuFile->addAction( frame->tr("Close all projects"), frame, SLOT(cmFileCloseAll()) );
  menuFile->addSeparator();
  cmFileSave = menuFile->addAction( frame->tr("Save project"), frame, SLOT(cmFileSave()) );
  cmFileSaveAs = menuFile->addAction( frame->tr("Save project as..."), frame, SLOT(cmFileSaveAs()) );
  cmFileSaveAll = menuFile->addAction( frame->tr("Save all projects"), frame, SLOT(cmFileSaveAll()) );
  menuFile->addSeparator();
  cmFileImport = menuFile->addAction( frame->tr("Import..."), frame, SLOT(cmFileImport()) );
  cmFileExport = menuFile->addAction( frame->tr("Export..."), frame, SLOT(cmFileExport()) );
  menuFile->addSeparator();
  cmFilePrint = menuFile->addAction( frame->tr("Print..."), frame, SLOT(cmFilePrint()) );
  menuFile->addSeparator();
  cmFileExit = menuFile->addAction( frame->tr("Exit programm"), frame, SLOT(cmFileExit()) );

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
  cmObjectNew = menuObject->addAction( frame->tr("Create"), frame, SLOT(cmObjectNew()) );
  cmObjectRename = menuObject->addAction( frame->tr("Rename"), frame, SLOT(cmObjectRename()) );
  cmObjectDelete = menuObject->addAction( frame->tr("Delete"), frame, SLOT(cmObjectDelete()) );
  cmObjectCopy = menuObject->addAction( frame->tr("Copy"), frame, SLOT(cmObjectCopy()) );
  cmObjectPaste = menuObject->addAction( frame->tr("Paste"), frame, SLOT(cmObjectPaste()) );
  cmObjectCut = menuObject->addAction( frame->tr("Cut"), frame, SLOT(cmObjectCut()) );
  cmObjectSort = menuObject->addAction( frame->tr("Sort"), frame, SLOT(cmObjectSort()) );




  //Меню Редактирование
  menuEdit = new QMenu( frame->tr("Edit") );
  cmEditUndo = menuEdit->addAction( frame->tr("Undo"), frame, SLOT(cmEditUndo()) );
  cmEditRedo = menuEdit->addAction( frame->tr("Redo"), frame, SLOT(cmEditRedo()) );
  menuEdit->addSeparator();
  cmEditCopy = menuEdit->addAction( frame->tr("Copy"), frame, SLOT(cmEditCopy()) );
  cmEditPaste = menuEdit->addAction( frame->tr("Paste"), frame, SLOT(cmEditPaste()) );
  cmEditCut = menuEdit->addAction( frame->tr("Cut"), frame, SLOT(cmEditCut()) );
  cmEditDelete = menuEdit->addAction( frame->tr("Delete"), frame, SLOT(cmEditDelete()) );
  menuEdit->addSeparator();
  cmEditSelectAll = menuEdit->addAction( frame->tr("Select All"), frame, SLOT(cmEditSelectAll()) );
  cmEditFind = menuEdit->addAction( frame->tr("Find"), frame, SLOT(cmEditFind()) );
  cmEditReplace = menuEdit->addAction( frame->tr("Replace"), frame, SLOT(cmEditReplace()) );
  menuEdit->addSeparator();
  cmEditProperties = menuEdit->addAction( frame->tr("Properties..."), frame, SLOT(cmEditProperties()) );




  menuView = new QMenu( frame->tr("View") );
  cmViewProject = menuView->addAction( frame->tr("Show-hide project"), frame, SLOT(cmViewProject()) );
  menuView->addSeparator();
  cmViewNets = menuView->addAction( frame->tr("Nets"), frame, SLOT(cmViewNets()) );
  cmViewGrid = menuView->addAction( frame->tr("Grid"), frame, SLOT(cmViewGrid()) );
  cmViewLayers = menuView->addAction( frame->tr("Layers"), frame, SLOT(cmViewLayers()) );
  menuView->addSeparator();
  cmViewFill = menuView->addAction( frame->tr("Zoom to fit view"), frame, SLOT(cmViewFill()) );
  cmViewZoomIn = menuView->addAction( frame->tr("Zoom in"), frame, SLOT(cmViewZoomIn()) );
  cmViewZoomOut = menuView->addAction( frame->tr("Zoom out"), frame, SLOT(cmViewZoomOut()) );
  cmViewArea = menuView->addAction( frame->tr("Zoom area"), frame, SLOT(cmViewArea()) );
  menuView->addSeparator();
  cmViewMeasurement = menuView->addAction( frame->tr("Measurement"), frame, SLOT(cmViewMeasurement()) );





  menuDraw = new QMenu( frame->tr("Draw") );




  menuInsert = new QMenu( frame->tr("Insert") );




  menuInstruments = new QMenu( frame->tr("Instruments") );
  cmOption = menuInstruments->addAction( frame->tr("Options"), frame, SLOT(cmOption()) );
  menuInstruments->addSeparator();
  cmTools = menuInstruments->addAction( frame->tr("Tools"), frame, SLOT(cmTools()) );



  menuHelp = new QMenu( frame->tr("Help") );
  cmHelpContens = menuHelp->addAction( frame->tr("Contens"), frame, SLOT(cmHelpContens()) );
  cmHelpIndex   = menuHelp->addAction( frame->tr("Index"), frame, SLOT(cmHelpIndex()) );
  cmHelpAbout   = menuHelp->addAction( frame->tr("About"), frame, SLOT(cmHelpAbout()) );


  QMenuBar *bar = frame->menuBar();
  bar->addMenu( menuFile );
  bar->addMenu( menuObject );
  bar->addMenu( menuEdit );
  bar->addMenu( menuView );
  bar->addMenu( menuDraw );
  bar->addMenu( menuInsert );
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

QActionPtr SdWCommand::cmBall;
QActionPtr SdWCommand::cmPinWired;
QActionPtr SdWCommand::cmPinFlat;
QActionPtr SdWCommand::cmBodyCylinder;
QActionPtr SdWCommand::cmBodyBrick;
QActionPtr SdWCommand::cmBodyContur;
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
QMenu *SdWCommand::menuInsert;
QMenu *SdWCommand::menuInstruments;
QMenu *SdWCommand::menuHelp;
