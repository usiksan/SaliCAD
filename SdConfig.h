/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SaliCAD configuration and history

History
  06.10.2016 v0.1  created on PasCAD base
  09.10.2016 v0.2  Append 80% from SaliCAD prior base
  10.10.2016 v0.3  Creating inserting, removing and undo
  18.12.2016 v0.4  Begin creating graph editors
  07.01.2017 v0.5  Begin creation modes
                   Envir created
                   Status bar created
  08.01.2017 v0.6  Move pic, icons and images from pascad9 to this
                   Begin tool bar creation
  04.04.2017 v0.7  Layers model created
                   Converter model created
                   Envir model updated
  09.04.2017 v0.8  Created reading windows cursors
                   Created modes structure
                   Menu mode actions convert to flat table to auto switch checking action
  10.04.2017 v0.9  Menu modes switching is ok
  11.04.2017 v0.10 Releasing view group modes
  12.04.2017 v0.11 Prop bars for mode properties
                   Reconstruct SdContext
                   Mode rect
  13.04.2017 v0.12 Object graph rect
                   Undo system structure
                   Mode insertion func
                   First insertion of graph object by mode
  15.04.2017 v0.13 Undo system basics, some undo operations.
                   Refactory object and container to obtain undo and redo.
                   Impruve main window, tree and editor for deleting object, close file.
                   Append filled rect. Mode and object.
                   Correct grid alignment.
  15.04.2017 v0.14 Append line. Mode and object.
  17.04.2017 v0.15 Add text drawing.
                   Begin text object and textual mode.
  17.04.2017 v0.16 Fixed minimizing main window in windows os. Cause was in QLabel. When it
                   insert in QStatusBar - it minimizing main window.
  19.04.2017 v0.17 Append text. Mode and object.
                   Correct text properties bar.
  10.05.2017 v0.18 Append part pin mode.
                   Append part pin bar (partial).
                   Separate modes for symbol and part.
  10.05.2017 v0.19 Continue to create Component editor. Release symbol append and part append.
                   Append symbol pin name unical and part pin number unical
                   Begin creation sheet and plate editors
  19.05.2017 v0.20 Continue creation GraphArea
  22.06.2017 v0.21 Append arc, arc mode
*/

#ifndef SDCONFIG
#define SDCONFIG

//Do'nt change this name
#define SD_AUTHOR                   "Alexander Sibilev"
#define SD_NAME                     "SaliCAD"

//Version definition
#define SD_VERSION_MAJOR             0
#define SD_VERSION_MINOR             21

//Some defaults
#define SD_DEFAULT_WEB               "www.SaliLAB.com"

//Project naming definition
#define SD_BASE_EXTENSION            ".salicad"
#define SD_DEFAULT_FILE_NAME         "default"

//SaliCAD database version
//Must be compatible with previous versions
#define SD_BASE_VERSION_1            1

//Current SaliCAD database version
#define SD_BASE_VERSION              SD_BASE_VERSION_1

//Previous file count in menu
#define PREVIOUS_FILES_COUNT         5

//Grid history count
//User can enter any grid size, but only GRID_HISTORY_SIZE grid sizes save in system for fast selection
#define GRID_HISTORY_SIZE            5

//Maximum font count
//In project fonts referenced by index in system font table
//This count define size of this table. So only FONT_COUNT different
//fonts may be used in project.
#define FONT_COUNT                   5

//Undo history count
#define UNDO_HISTORY_SIZE            100

//Clipboard objects format
#define SD_CLIP_FORMAT_OBJECT        "text/salicad-object"
#define SD_CLIP_FORMAT_FRAGMENT      "text/salicad-fragment"

#if 0
#define SD_BASE_PATH                 QCoreApplication::applicationDirPath()
#else
#define SD_BASE_PATH                 QString("/home/slilab/work/SaliCAD")
#endif

#define SD_DATABASE_FILE             QString("database.sqlite")

//Settings key names
#define SDK_WMAIN_MAX                "WMainMax"
#define SDK_LANGUAGE                 "Language"
#define SDK_MAIN_SPLITTER            "MainSplitter"
#define SDK_PREVIOUS_FILES           "PreviousFiles"
#define SDK_GLOBAL_ID_MACHINE        "Machine global id"
#define SDK_GLOBAL_ID_COUNT          "Global id count"
#define SDK_ENVIR_VERSION            "Envir version"
#define SDK_ENVIR                    "Envir"

#define MODE_HELP         //Prefix to mode help files

#define MAX64_MASK 0xffffffffffffffffL
#define MAX32_MASK 0xffffffff

#define defSymScale 20.0   /*Коэффициент масштаба по умолчанию для схемных изображений*/
#define defPrtScale 200.0  /*Коэффициент масштаба по умолчанию для изобр. плат*/
#define minScale    0.0001 /*Ограничения масштабирования*/
#define maxScale    10000.0
#define maxBusNumber 1000  //Max net number in bus when enter bus mode

#define defNetNamePrefix "XN" //Default net name prefix

#endif // SDCONFIG

