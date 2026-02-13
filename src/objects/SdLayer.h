/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Layer


*/
#ifndef SDLAYER_H
#define SDLAYER_H

#include "SdStratum.h"
#include "SdClass.h"

#include <QString>
#include <QColor>
#include <QDataStream>
#include <QMap>

//Layer visibility and editing status
enum SdLayerState {
  layerStateOff,  //Layer is off
  layerStateOn,   //Layer is visible
  layerStateEdit  //Layer is visible and editable
  };



//Layer trace status
enum SdLayerTrace {
  layerTraceNone,     //Layer not tracing [Слой не трассировочный]
  layerTracePad,      //Layer for pads
  layerTraceMask,     //Layer for mask
  layerTraceStencil,  //Layer for stencil apertures
  layerTraceHole,     //Layer for holes
  layerTraceRoad,     //Layer for wires
  layerTracePolygon,  //Layer for polygons
  layerTraceBoundary, //Layer for tracing boundary
  layerTraceKeepout,  //Layer for tracing keepout
  layerTraceLast
  };


// Layer identification
// Both schematic and PCB (without LID1_...)
#define LID0_COMMON        "Common"       // Any objects
#define LID0_INVISIBLE     "Invisible"    // Invisible objects
#define LID0_ERRORS        "Errors"       // For errors display

// For Schematic (without LID1_...)
#define LID0_NET           "Net"          // Circuit graphics
#define LID0_NET_NAME      "NetName"      // Net name graphics
#define LID0_BUS           "Bus"          // Bus graphics
#define LID0_AREA          "Area"         // PCB area in schematic editor
#define LID0_FORM          "Form"         // Drawing form, frames and title blocks
#define LID0_NAVI          "Navi"         // Navigation grid with alphanumeric addressing

// For PCB (without LID1_...)
#define LID0_PCB           "PCB"          // PCB contour
#define LID0_HOLE          "Hole"         // Hole
// For PCB (LID1_TOP,LID1_BOT)
#define LID0_STENCIL       "Stencil"      // Stencil
#define LID0_STENCIL_REPER "StencilReper" // Stencil fiducial
#define LID0_SILK          "Silk"         // Silk layer
#define LID0_ADHESIV       "Adhesiv"      // Adhesiv layer
#define LID0_DIM           "Dim"          // Dimension lines and annotations
#define LID0_GUIDE         "Guide"        // Construction guides and alignment lines
#define LID0_COURTYARD     "Courtyard"    // Component courtyard
#define LID0_SOLDER_MASK   "Mask"         // Mask
// Both schematic and PCB (without LID1_... for schematic; LID1_TOP, LID1_BOT for PCB)
#define LID0_COMPONENT     "Component"    // Component graphics (symbol or footprint)
#define LID0_PIN           "Pin"          // Pin graphics
#define LID0_PIN_NAME      "PinName"      // Pin name graphics
#define LID0_PIN_NUMBER    "PinNumber"    // Pin number graphics
#define LID0_IDENT         "Ident"        // Identifier graphics
#define LID0_VALUE         "Value"        // Value layer
// Both schematic and PCB (without LID1_... for schematic; without LID1_... and with LID1_xxx for PCB)
#define LID0_PICTURE       "Picture"      // Arbitrary drawing
#define LID0_REMARK        "Remark"       // Comment
// For PCB (without LID1_... and with LID1_xxx)
#define LID0_CLEAR         "Clear"        // Clearance
#define LID0_EXCLUSION     "Exclusion"    // Trace exclusion area
// For pcb (with LID1_xxx for PCB)
#define LID0_WIRE          "Wire"         // PCB wires
#define LID0_POLYGON       "Polygon"      // Polygon layer
#define LID0_PAD           "Pad"          // Pads
#define LID0_TRACE         "Trace"        // Trace area

//#define LID0_CONTOUR       "Contour"      // Contour

#define LID1_TOP           ".Top"         // Top side
#define LID1_BOT           ".Bot"         // Bottom side
#define LID1_INT02         ".Int02"       // Inner side 2
#define LID1_INT03         ".Int03"       // Inner side 3
#define LID1_INT04         ".Int04"       // Inner side 4
#define LID1_INT05         ".Int05"       // Inner side 5
#define LID1_INT06         ".Int06"       // Inner side 6
#define LID1_INT07         ".Int07"       // Inner side 7
#define LID1_INT08         ".Int08"       // Inner side 8
#define LID1_INT09         ".Int09"       // Inner side 9
#define LID1_INT10         ".Int10"       // Inner side 10
#define LID1_INT11         ".Int11"       // Inner side 11
#define LID1_INT12         ".Int12"       // Inner side 12
#define LID1_INT13         ".Int13"       // Inner side 13
#define LID1_INT14         ".Int14"       // Inner side 14
#define LID1_INT15         ".Int15"       // Inner side 15
#define LID1_INT16         ".Int16"       // Inner side 16
#define LID1_INT17         ".Int17"       // Inner side 17
#define LID1_INT18         ".Int18"       // Inner side 18
#define LID1_INT19         ".Int19"       // Inner side 19
#define LID1_INT20         ".Int20"       // Inner side 20
#define LID1_INT21         ".Int21"       // Inner side 21
#define LID1_INT22         ".Int22"       // Inner side 22
#define LID1_INT23         ".Int23"       // Inner side 23
#define LID1_INT24         ".Int24"       // Inner side 24
#define LID1_INT25         ".Int25"       // Inner side 25
#define LID1_INT26         ".Int26"       // Inner side 26
#define LID1_INT27         ".Int27"       // Inner side 27
#define LID1_INT28         ".Int28"       // Inner side 28
#define LID1_INT29         ".Int29"       // Inner side 29

#define LID1_CUSTOM1       ".Custom1"     // User custom layer 1
#define LID1_CUSTOM2       ".Custom2"     // User custom layer 2
#define LID1_CUSTOM3       ".Custom3"     // User custom layer 3
#define LID1_CUSTOM4       ".Custom4"     // User custom layer 4
#define LID1_CUSTOM5       ".Custom5"     // User custom layer 5
#define LID1_CUSTOM6       ".Custom6"     // User custom layer 6
#define LID1_CUSTOM7       ".Custom7"     // User custom layer 7
#define LID1_CUSTOM8       ".Custom8"     // User custom layer 8
#define LID1_CUSTOM9       ".Custom9"     // User custom layer 9

struct SdLayerDescr {
  unsigned      mClass;    //!< Classes for defining layer membership
  const char   *mId;       //!< Layer id
  unsigned      mColor;    //!< Layer color
  SdLayerTrace  mTrace;    //!< Layer trace type
  int           mStratum;  //!< Layer stratum
  };

extern SdLayerDescr sdLayerDescrDefault[];

//Translation layer id to human visible name
struct SdLayerLevel {
    const char   *mLid;       //!< Level id LIDxxx
    const char   *mTranslate; //!< Human visible name aka "Schematic net"
    SdLayerTrace  mTrace;     //!< Layer trace type
    unsigned      mStratum;   //!< Stratum for sublevel or class

    unsigned classId() const { return mStratum; }
  };

extern SdLayerLevel sdLayerLevel0[];
extern SdLayerLevel sdLayerLevel1[];


class SdLayer
  {
    QString       mId;       //!< Layer ident - unical handle
    QString       mName;     //!< Layer name for visual
    QString       mEnglish;  //!< Layer name english
    SdLayerState  mState;    //!< State (visible, editing, invisible)
    SdLayerTrace  mTrace;    //!< Layer trace type
    SdClass       mClass;    //!< Classes for defining layer membership
    int           mStratum;  //!< Layer stratum [Позиция слоя при трассировке (верх, низ, внутри)]
    unsigned      mColor;    //!< Layer color [Цвет]
    int           mIndex;    //!< Ordinal index for sorting
    SdLayer      *mPair;     //!< Layer pair for flipped component [Парный слой]
    bool          mUsage;    //!< Usage flag [Флаг использования]
  public:
    SdLayer(QString layerId, QString layerName, QString layerEnglishName, SdLayerTrace layerTrace, int layerClass, int layerStratum, unsigned layerColor, int index  );

    QString      id() const { return mId; }

    QString      name() const { return mName; }
    void         nameSet( const QString nm ) { mName = nm; }

    SdLayer     *pair() { return mPair; }

    int          index() const { return mIndex; }

    //Установить новую пару для слоя
    void         pairSet( SdLayer *p );

    //Reset pair for layer
    void         pairReset();

    //Return true when layer is visible
    bool         isVisible() const { return mState != layerStateOff; }

    //Возвращает истину когда слой редактируемый
    bool         isEdited() const { return mState == layerStateEdit; }

    //Layer state
    SdLayerState state() const { return mState; }
    void         stateSet( SdLayerState st ) { mState = st; }

    //Layer trace type
    SdLayerTrace trace() const { return mTrace; }

    SdClass      classGet() const { return mClass; }

    int          stratum() const { return mStratum; }

    //Layer color
    //Return layer color
    QColor       color() const { return QColor( QRgb(mColor) ); }
    //Set new color for layer
    void         colorSet( unsigned c ) { mColor = c; }

    //Usage flag. Indicate layer is used anywhere
    //Сбросить флаг использования
    //Reset flag
    void         resetUsage() { mUsage = false; }
    //Установить флаг использования
    //Set flag
    void         setUsage() { mUsage = true; if( pair() ) pair()->mUsage = true; }
    //Get flag
    bool         isUsage() const { return mUsage; }

    //Сохранить слой
    void         write( QDataStream &os );

    //Загрузить слой
    void         read( QDataStream &is );


    static QPair<QString,QString> layerIdToName( const QString layerId, int &layerIndex, SdLayerDescr *descr = nullptr );
    static QPair<QString,QString> layerIdToName(const QString &lid0, const QString &lid1, int &layerIndex, SdLayerDescr *descr = nullptr );
  };

typedef SdLayer *SdLayerPtr;

typedef QMap<QString,SdLayerPtr> SdLayerPtrMap;

#endif // SDLAYER_H
