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
  19.07.2017 v0.22 Creation selection mode
                   in SdGraph::select selector may be nullptr, insert check of this
                   remove empty select (base insertion released in SdGraph::select)
  16.08.2017 v0.23 Begin creation print dialog and print system
  22.08.2017 v0.24 Continue with print dialog.
                     Print count.
                     Select any printer.
                     Correct scale calculation.
                     Color appliance.
  22.08.2017 v0.25 Print dialog complete.
                   Append undo deleting.
  04.09.2017 v0.26 Append some icons.
                   Set edit-view mech.
  25.02.2018 v0.27 Append grid dialog
  18.02.2018 v0.28 Append layer dialog
                   - layer name change
                   - color change
                   - state change
                   - save list
  19.05.2018 v0.29 Append pasCad text prop import, text import, circle import
  04.06.2018 v0.30 Append wheel support in graph windows
  04.06.2018 v0.31 Append arc pic pasCad import. Symbols imported.
                   Append system color selction to Options dialog.
  09.06.2018 v0.32 Begin PadDialog creation
                   Append pad description building
  13.06.2018 v0.33 Continue with pad dialog.
                   Begin PadMaster dialog creation.
  13.06.2018 v0.34 Complete with PadMaster dialog and Pads dialog.
                   Append SdIllustrator to use in masters to draw actual object schematic
  14.06.2018 v0.35 Append rules and rules extractor
  15.06.2018 v0.36 Append command for check rules and showRuleErrors with icons
                   Append "check rules" algorithm
                   Append tracing layer count edit line in layers dialog when calling from plate
  15.07.2018 v0.37 Append central object repository with server.
                   Reconstruct communication with central object repository.
  15.07.2018 v0.38 Append compression to object database to reduce traffic
                   Disable "sali" subname when registration
                   Convert all author names to lower to exclude clones with sAli, saLi and so on
  16.07.2018 v0.39 Append last resently editor automatic remove
                   Fix: rename component name not changed in graph editor
  16.07.2018 v0.40 Append in SdIllustrator circle draw and cross draw
                   Begin creation twoPinPart master
  17.07.2018 v0.41 Append part master which creates rectangle part with two pins.
  18.07.2018 v0.42 Append part master which creates double sided pins part.
                   Fix error in illustrator. Image shifted down.
  21.07.2018 v0.43 Append all modes for draw cursor
  01.08.2018 v0.44 Append unical ident (uid) for libary storage objects.
                   Object id at now is simple pointer to resolve crossreference over project.
                   From uid removed time component, so object with same type, name and author is identical.
                   Time of creation used for replace older objects in base, so updating component when
                   upgraded symbol or part no more needed.
  01.08.2018 v0.45 Append part import from pascad
  06.08.2018 v0.46 Append param edit to component editor
                   Append param delete possibility to ProjectItem
                   Fix bugs with object insertion to library
  07.08.2018 v0.47 Append inheritance object which inherits component
  14.08.2018 v0.48 Append sort possibility to SdContainer
                   Release sort for project items
  27.08.2018 v0.49 Complete minimal version of part place mode. Support smart mode, direct selection, incremental selection, browse in sheet.
  04.09.2018 v0.50 Complete minimal version of road enter mode and via enter mode.
                   Support smart mode, handle tracing, tracing guide.
  07.09.2018 v0.51 Append rules editing. Reconstruct rules with two-level hierarchy.
                   Fix errors in sheet bus mode, sheet wire mode, part and symbol pin modes.
  11.09.2018 v0.52 Improvements to RoadEnter mode
                   Extend smart tracing
  19.09.2018 v0.53 Append object clipboard operations
                   Append object duplicate command
  23.09.2018 v0.54 Rebrended GetObject dialog
                   Remove tag field
  12.10.2018 v0.55 Append display pad possibility to part editor
  21.10.2018 v0.56 Append symbol ic master
                   Append smart mode to polygon enter mode
                   Append via enter mode
                   Append common text object and graphics editor
                   Append project preview editor
                   Append intro help editor
                   Append tabled param to component
                   Redesign GetObject dialog
                   Append newer object update possibilities
  08.11.2018 v0.57 Redesign SdSnapInfo
                   Append intro window to allow help display
*/

#ifndef SDCONFIG
#define SDCONFIG

//Do'nt change this name
#define SD_AUTHOR                   "Alexander Sibilev"
#define SD_NAME                     "SaliCAD"

//Version definition
#define SD_VERSION_MAJOR             0
#define SD_VERSION_MINOR             57

//Some defaults
#define SD_DEFAULT_WEB               "www.SaliLAB.com"
#define SD_DEFAULT_IP                "62.109.11.83"
//#define SD_DEFAULT_IP                "127.0.0.0"
#define SD_DEFAULT_PORT              1970


//When defined disable registration with sali name
#define SD_DISABLE_SALI_AUTHOR

//Project naming definition
#define SD_BASE_EXTENSION            ".salicad"
#define SD_DEFAULT_FILE_NAME         "default"

//Layer list naming definition
#define SD_LAYER_LIST_EXTENSION      ".layersSet"

//Gerber file extension
#define SD_GERBER_EXTENSION          ".gbr"

//SaliCAD database version
//Must be compatible with previous versions
#define SD_BASE_VERSION_1            1

//Current SaliCAD database version
#define SD_BASE_VERSION              SD_BASE_VERSION_1

//Previous file count in menu
#define PREVIOUS_FILES_COUNT         5

//Grid history count
//User can enter any grid size, but only GRID_HISTORY_SIZE grid sizes save in system for fast selection
#define GRID_HISTORY_SIZE            8

//Maximum open editors
//When open editor which index more than this maximum
// then last resently editor removed
#define SD_MAX_EDITORS              10

//Maximum font count
//In project fonts referenced by index in system font table
//This count define size of this table. So only FONT_COUNT different
//fonts may be used in project.
#define FONT_COUNT                   5

//Undo history count
#define UNDO_HISTORY_SIZE            100

//Clipboard objects format
#define SD_CLIP_FORMAT_PITEM         "bin/salicad-SdProjectItem"    //Partial salicad SdProjectItem
#define SD_CLIP_FORMAT_SELECTOR      "bin/salicad-SdSelector"       //Project and selection SdSelector

//Size of picture to draw fragment to clipboard
#define CLIP_IMAGE_WIDTH             1920
#define CLIP_IMAGE_HEIGHT            1280

//Max default delivered limit elements
#define SD_DEFAULT_DELIVERED_LIMIT    100

//Max object in find list of GetObject dialog
#define SD_GET_OBJECT_MAX_FIND_LIST   300

//Settings key names
#define SDK_WMAIN_MAX                "WMainMax"
#define SDK_LANGUAGE                 "Language"
#define SDK_MAIN_SPLITTER            "MainSplitter"
#define SDK_PREVIOUS_FILES           "PreviousFiles"
#define SDK_GLOBAL_AUTHOR            "Author global id"
#define SDK_MACHINE_KEY              "Machine global key"
#define SDK_LOCAL_SYNC               "Local sync"
#define SDK_REMOTE_SYNC              "Remote sync"
#define SDK_REMOTE_REMAIN            "Remote remain"
#define SDK_SERVER_IP                "SaliCAD server IP"
#define SDK_ENVIR_VERSION            "Envir version"
#define SDK_ENVIR                    "Envir"
#define SDK_HELP_PATH                "HelpPath"

#define MODE_HELP         //Prefix to mode help files

#define MAX64_MASK 0xffffffffffffffffL
#define MAX32_MASK 0xffffffff

#define defSymScale 20.0   /*Коэффициент масштаба по умолчанию для схемных изображений*/
#define defPrtScale 200.0  /*Коэффициент масштаба по умолчанию для изобр. плат*/
#define minScale    0.0001 /*Ограничения масштабирования*/
#define maxScale    10000.0
#define maxBusNumber 1000  //Max net number in bus when enter bus mode

#define defNetNamePrefix "XN" //Default net name prefix

//Standard param names
#define stdParamBom           QStringLiteral("bom")
#define stdParamArticle       QStringLiteral("article")
#define stdParamTitle         QStringLiteral("title")
#define stdParamValue         QStringLiteral("value")
#define stdParamValueMin      QStringLiteral("valueMin")
#define stdParamValueMax      QStringLiteral("valueMax")
#define stdParamValueRow      QStringLiteral("valueRow")
#define stdParamPrefix        QStringLiteral("prefix")
#define stdParamValueSelector QStringLiteral("valueSelector")
#define stdParamManufacturer  QStringLiteral("manufacturer")
#define stdParamTolerance     QStringLiteral("tolerance")
#define stdParamDatasheet     QStringLiteral("datasheet")
#define stdParamWww           QStringLiteral("www")

#endif // SDCONFIG

