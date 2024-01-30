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
  15.11.2018 v0.58 Redesign categories. Append categories editor and save it to file.
  23.12.2018 v0.59 Append road move mode.
                   Changes to road enter mode.
                   Fix check tracing rules
  03.02.2019 v0.60 Append BOM report generator
  13.02.2019 v0.61 Append to road mode possibilities to connect to middle of road
                   and automatic loop detection and removing
  21.02.2019 v0.62 Append upgrade check
  10.03.2019 v0.62 Fix: section allocate critical error
  14.04.2019 v0.63 Remove inheritance object
                   Fix: Set focus to plate editor window when component selected by id
                        When delete schematic sheet and plate we remove all contents for link removing
                   Append named net list to bus mode dialog
  17.04.2019 v0.64 When upgrading component don't changed ident (and perhaps - value)
                   Prevent open same file more then one time
  02.05.2019 v0.65 Append category param, category fixed tree, allow multiple categories
  09.05.2019 v1.0  Release
  23.08.2019 v1.1  In symbol and part editors text must be display only as field and never context
  24.08.2019 v1.2  Append category selection to component editor
             v1.3  Fix other side display error
  06.09.2019 v1.4  Append command to mask component side at moving
  11.09.2019 v1.5  Change drawing filled circle and rectangle from with prop to with layer
                   this exclude wrong setPen call which hamper properly gerber generation
  30.09.2019 v1.6  When append default component copy to it category from symbol
  01.10.2019 v1.7  Append automatic form selection when sheet creation
             v1.8  Append slot form holes in a pads
                   Append single row part master
  17.10.2019 v1.9  Append undo group rotation
  14.11.2019 v1.10 In part master for double side vertical not created first pin marker
  10.02.2020 v1.11 In gerber export appended enabling finish only if gerber generated
  18.02.2020 v1.12 Begin to insert 3d support
  27.02.2020 v1.13 Change remote library sync alg. At now we receiv all available objects immediately,
                   so not need to download object by user query
  18.09.2020 v1.14 In bus mode append automatic wire name orientation
  11.10.2020 v1.15 Fix error: crash when sheet master cancel
  12.12.2020 v1.16 Continue with STEP import
  05.01.2021 v1.17 Begin to change global repository. At now I use MySQL on server and http php queries, because
                   some proxies block queries with socket link.
  17.01.2021 v1.18 Complete with change global repository to use MySQL on server and http php queries
  20.01.2021 v1.19 Append expression list as associated list to the schematic sheet
  21.01.2021 v1.20 Append automatic fragment finding for insertion when component inserting
  25.01.2021 v1.21 Convert all 3d models to FaceSet object which consist of face set
                   Different 3d format are reads by individual readers
  30.01.2021 v1.22 Replace face color in Sd3dFace with face material
                   Reorganize directories: outstand SdScaner... to scaner directory
  02.04.2021 v1.23 Append tool bar for 3d modes in part 3d view editor
                   Remove obsolete commands for 3d insertion. Future main stream for 3d is 3d master using
                   which builds parametriesed 3d models for components
                   Error: crash when no 3d model for view
  05.04.2021 v1.24 Refactor 3d modes
  06.04.2021 v1.25 Add deactivate 3d mode by right button
                   Append checking 3d mode button when select and deselect
                   Append remove previous 3d objects when insert new model with master
             v1.26 Append setting project dirty when insert 3d object
                   Adjust scale of 3d object moving
  07.04.2021 v1.27 Append colors for pads, holes and pcb to central envir
  09.04.2021 v1.28 Append status help and help invocation for 3d modes
                   Append horizontal rotation mode
             v1.29 Append vertical moving and rotation modes
  02.12.2021 v1.30 Append part highlight mode
  17.01.2022 v1.31 Refactor 3d master programm into common script language
  18.01.2022 v1.32 Begin conversion of schematic expressions into script based calculations
  12.05.2022 v1.33 Append some remarks to script for schematic
                   Append some help for script for schematic
                   Fix: draw schematic expressions from up to down
  20.05.2022 v1.34 Append showing reference when copy-paste-move schematic script
  26.05.2022 v1.35 Refactor read-write json into SvJsonIO infrastructure completed
  27.05.2022 v1.36 Append mode for symbol fragment
  02.06.2022 v1.37 Append face rotation algorithm, so we can do curved pins

  20.07.2022 v2.0  Global refactoring library storage system
  28.07.2022 v2.1  Create project to library storage system
  12.10.2022 v2.2  Append web page generation and selection save as picture
  19.10.2022 v2.3  Append via rules, append full list of internal layers, append menu item for pcb layers count
  21.10.2022 v2.4  Append drag origin point of graph editor with Shift+Middle button
                   Append trace delete mode
  07.08.2023 v2.5  Fix: crash for 3d view with no pcb contour
  29.08.2023 v2.6  Append assembler export file extension
                   Append part name to assembler export file
  29.01.2024 v2.7  Append stm32 alternative pin names import
*/

#ifndef SDCONFIG
#define SDCONFIG

//Do'nt change this name
#define SD_AUTHOR                    "Alexander Sibilev"
#define SD_NAME                      "SaliCAD"

//Version definition
#define SD_VERSION_MAJOR             2
#define SD_VERSION_MINOR             7

//Some defaults
#define SD_DEFAULT_WEB               "www.SaliLAB.com"
#define SD_DEFAULT_REPO              "cad.salilab.com/repo/"
#define SD_UPGRADE_WEB               "http:/cad.salilab.com/downloads/"


//When defined disable registration with sali name
#define SD_DISABLE_SALI_AUTHOR

//Project naming definition
#define SD_BASE_EXTENSION            ".salicad"
#define SD_DEFAULT_FILE_NAME         "default"

//Binary project naming definition
#define SD_BINARY_EXTENSION          ".salicadx"

//Layer list naming definition
#define SD_LAYER_LIST_EXTENSION      ".layersSet"

//Pads association naming definition
#define SD_PAD_ASSOCIATION_EXTENSION ".salipads"

//Export to assembler file extension
#define SD_ASSEMBLER_EXTENSION       ".saliasm"

//Category files
#define SD_CATEGORY_EXTENSION        ".category"
#define SD_CATEGORY_FILE             "-salicad.category"
#define SD_CATEGORY_PREFIX           "{uu"

//Gerber file extension
#define SD_GERBER_EXTENSION          ".gbr"

//SaliCAD database version
//Must be compatible with previous versions
#define SD_BASE_VERSION_1            1
#define SD_BASE_VERSION_2            2

//Current SaliCAD database version
#define SD_BASE_VERSION              SD_BASE_VERSION_2

//Salicad json file type description
#define SD_BASE_TYPE                 "SaliCAD file"

//Previous file count in menu
#define PREVIOUS_FILES_COUNT         10

//Grid history count
//User can enter any grid size, but only GRID_HISTORY_SIZE grid sizes save in system for fast selection
#define GRID_HISTORY_SIZE            12

//Maximum open editors
//When open editor which index more than this maximum
// then last resently editor removed
#define SD_MAX_EDITORS               10

//Maximum font count
//In project fonts referenced by index in system font table
//This count define size of this table. So only FONT_COUNT different
//fonts may be used in project.
#define FONT_COUNT                   5

//Undo history count
#define UNDO_HISTORY_SIZE            1000

//Clipboard objects format
#define SD_CLIP_FORMAT_PITEM         "bin/salicad-SdProjectItem"    //Partial salicad SdProjectItem
#define SD_CLIP_FORMAT_SELECTOR      "bin/salicad-SdSelector"       //Project and selection SdSelector

//Size of picture to draw fragment to clipboard
#define CLIP_IMAGE_WIDTH             1920
#define CLIP_IMAGE_HEIGHT            1280

//Max default delivered limit elements
#define SD_DEFAULT_DELIVERED_LIMIT   100

//Max object in find list of GetObject dialog
#define SD_GET_OBJECT_MAX_FIND_LIST  300

//Settings key names
#define SDK_WMAIN_MAX                "WMainMax"
#define SDK_LANGUAGE                 "Language"
#define SDK_MAIN_SPLITTER            "MainSplitter"
#define SDK_PREVIOUS_FILES           "PreviousFiles"
#define SDK_GLOBAL_AUTHOR            "Author global id"
#define SDK_GLOBAL_PASSWORD          "Author global password"
#define SDK_REMOTE_SYNC              "Remote sync"              //!< Remote index of last downloaded object
#define SDK_REMOTE_REMAIN            "Remote remain"
#define SDK_SERVER_REPO              "SaliCAD repository"
#define SDK_UPLOAD_AUTO              "Auto upload"
#define SDK_ENVIR_VERSION            "Envir version"
#define SDK_ENVIR                    "Envir"
#define SDK_HELP_PATH                "HelpPath"
#define SDK_GET_OBJECT_DLG_CFG       "DlgGetObjectCfg"

#define MODE_HELP                    //Prefix to mode help files

#define MAX64_MASK                   0xffffffffffffffffL
#define MAX32_MASK                   0xffffffff

#define defSymScale                  20.0    //Default scale for schematic and symbol
#define defPrtScale                  200.0   //Default scale for PCB and part
#define minScale                     0.0001  //Minimum scale
#define maxScale                     10000.0 //Maximum scale
#define maxBusNumber                 1000    //Max net number in bus when enter bus mode
#define pcb3dZLevel                  -0.05   //Vertical offset of pcb
#define pcb3dThickness               1.2

#define defNetNamePrefix             "XN"    //Default net name prefix

//Standard param names
#define stdParamBom                  QStringLiteral("bom")
#define stdParamArticle              QStringLiteral("article")
#define stdParamTitle                QStringLiteral("title")
#define stdParamValue                QStringLiteral("value")
#define stdParamValueMin             QStringLiteral("valueMin")
#define stdParamValueMax             QStringLiteral("valueMax")
#define stdParamValueRow             QStringLiteral("valueRow")
#define stdParamPrefix               QStringLiteral("prefix")
#define stdParamValueSelector        QStringLiteral("valueSelector")
#define stdParamManufacturer         QStringLiteral("manufacturer")
#define stdParamTolerance            QStringLiteral("tolerance")
#define stdParamDatasheet            QStringLiteral("datasheet")
#define stdParamWww                  QStringLiteral("www")
#define stdParamLogNumber            QStringLiteral("logNumber")
#define stdParamItemId               QStringLiteral("itemId")
#define stdParamItemIdList           QStringLiteral("itemIdList")
#define stdParamCompCount            QStringLiteral("compCount")
#define stdParamTotalPageCount       QStringLiteral("totalPageCount")
#define stdParamPageIndex            QStringLiteral("pageIndex")
#define stdParamDeviceMark           QStringLiteral("device mark")
#define stdParamCategory             QStringLiteral("category")
//Capacitor specific
#define stdParamDielectric           QStringLiteral("dielectric")
#define stdParamVoltage              QStringLiteral("voltage")
//Diod specific
#define stdParamForvardVoltage       QStringLiteral("forward voltage")
#define stdParamCurent               QStringLiteral("current")
#define stdParamReverceVoltage       QStringLiteral("reverce voltage")
//LED specific
#define stdParamAngle                QStringLiteral("angle")
#define stdParamBrightness           QStringLiteral("brightness")
#define stdParamColor                QStringLiteral("color")
#define stdParamWavelength           QStringLiteral("wavelength")
//Inductor specific
#define stdParamSatCurrent           QStringLiteral("saturation current")
#define stdParamResistance           QStringLiteral("resistance")
#define stdParamSRF                  QStringLiteral("SRF")
//MOSFET specific
#define stdParamId                   QStringLiteral("Id")
#define stdParamPd                   QStringLiteral("Pd")
#define stdParamRdsOn                QStringLiteral("Rds on")
#define stdParamVdss                 QStringLiteral("Vdss")
#define stdParamVgs                  QStringLiteral("Vgs")
//Transistor specific
#define stdParamIc                   QStringLiteral("Ic")
#define stdParamVcbo                 QStringLiteral("Vcbo")
#define stdParamVceo                 QStringLiteral("Vceo")
#define stdParamHFE                  QStringLiteral("hFE")
//3d model programm
#define stdParam3dModelProgramm      QStringLiteral("3dModelProgramm")
#define stdParam3dModelDescription   QStringLiteral("3dModelDescription")

#endif // SDCONFIG

