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
*/

#ifndef SDCONFIG
#define SDCONFIG

//Do'nt change this name
#define SD_AUTHOR                   "Alexander Sibilev"

//Version definition
#define SD_VERSION_MAJOR             0
#define SD_VERSION_MINOR             1

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
#define SALICAD_CLIP_FORMAT_OBJECT   "text/salicad-object"
#define SALICAD_CLIP_FORMAT_FRAGMENT "text/salicad-fragment"



//Settings key names
#define SDK_WMAIN_MAX                "WMainMax"
#define SDK_LANGUAGE                 "Language"
#define SDK_MAIN_SPLITTER            "MainSplitter"
#define SDK_PREVIOUS_FILES           "PreviousFiles"

#endif // SDCONFIG

