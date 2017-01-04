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
*/

#ifndef SDCONFIG
#define SDCONFIG

//Do'nt change this name
#define SD_AUTHOR                   "Alexander Sibilev"
#define SD_NAME                     "SaliCAD"

//Version definition
#define SD_VERSION_MAJOR             0
#define SD_VERSION_MINOR             4

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

//Clipboard objects format
#define SD_CLIP_FORMAT_OBJECT        "text/salicad-object"
#define SD_CLIP_FORMAT_FRAGMENT      "text/salicad-fragment"

#if 0
#define SD_BASE_PATH                 QCoreApplication::applicationDirPath()
#else
#define SD_BASE_PATH                 QString("/home/slilab/work/SaliCAD")
#endif


//Settings key names
#define SDK_WMAIN_MAX                "WMainMax"
#define SDK_LANGUAGE                 "Language"
#define SDK_MAIN_SPLITTER            "MainSplitter"
#define SDK_PREVIOUS_FILES           "PreviousFiles"
#define SDK_GLOBAL_ID_MACHINE        "Machine global id"
#define SDK_GLOBAL_ID_COUNT          "Global id count"
#define SDK_COLOR                    "Color %d"

#endif // SDCONFIG

