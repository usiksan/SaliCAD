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
  layerTraceStensil,  //Layer for stensil apertures
  layerTraceHole,     //Layer for holes
  layerTraceRoad,     //Layer for wires
  layerTracePolygon,  //Layer for polygons
  layerTraceBoundary, //Layer for tracing boundary
  layerTraceKeepout,  //Layer for tracing keepout
  layerTraceLast
  };


//Layer identification
#define LID0_COMMON        "Common"       //Любые объекты
#define LID0_INVISIBLE     "Invisible"    //Невидимые объекты

//Schematic specific
#define LID0_NET           "Net"          //Изображение цепей
#define LID0_NET_NAME      "NetName"      //Изображение имен цепей
#define LID0_BUS           "Bus"          //Изображение шин
#define LID0_AREA          "Area"         //PCB area

//PCB specific
#define LID0_PCB           "PCB"          //PCB contour
#define LID0_WIRE          "Wire"         //PCB wires
#define LID0_POLYGON       "Polygon"      //Слой полигона
#define LID0_PAD           "Pad"          //Площадки
#define LID0_HOLE          "Hole"         //Отверстие
#define LID0_CLEAR         "Clear"        //Зазор
#define LID0_SOLDER_MASK   "Mask"         //Маска
#define LID0_STENSIL       "Stensil"      //Трафарет
#define LID0_STENSIL_REPER "StensilReper" //Репер на трафарете
#define LID0_EXCLUSION     "Exclusion"    //Trace exclusion area
#define LID0_TRACE         "Trace"        //Trace area
#define LID0_TRACE_DEFAULT "TraceDefault" //Default trace layer for undefined stratum

//Both schematic and PCB
#define LID0_COMPONENT     "Component"    //Изображение элемента (символа или корпуса)
#define LID0_PIN           "Pin"          //Изображение вывода
#define LID0_PIN_NAME      "PinName"      //Изображение имени вывода
#define LID0_PIN_NUMBER    "PinNumber"    //Изображение номера вывода
#define LID0_IDENT         "Ident"        //Изображение идентификатора
#define LID0_PICTURE       "Picture"      //Произвольный рисунок
#define LID0_REMARK        "Remark"       //Коментарий
#define LID0_VALUE         "Value"        //Слой значений
#define LID0_CONTOUR       "Contour"      //Контур


#define LID1_TOP           ".Top" //Верхняя сторона
#define LID1_BOT           ".Bot" //Нижняя сторона
#define LID1_INT00         ".Int00" //Внутренняя сторона 0
#define LID1_INT01         ".Int01" //Внутренняя сторона 1
#define LID1_INT02         ".Int02" //Внутренняя сторона 2
#define LID1_INT03         ".Int03" //Внутренняя сторона 3
#define LID1_INT04         ".Int04" //Внутренняя сторона 4
#define LID1_INT05         ".Int05" //Внутренняя сторона 5
#define LID1_INT06         ".Int06" //Внутренняя сторона 6
#define LID1_INT07         ".Int07" //Внутренняя сторона 7
#define LID1_INT08         ".Int08" //Внутренняя сторона 8
#define LID1_INT09         ".Int09" //Внутренняя сторона 9
#define LID1_INT10         ".Int10" //Внутренняя сторона 10
#define LID1_INT11         ".Int11" //Внутренняя сторона 11
#define LID1_INT12         ".Int12" //Внутренняя сторона 12
#define LID1_INT13         ".Int13" //Внутренняя сторона 13
#define LID1_INT14         ".Int14" //Внутренняя сторона 14
#define LID1_INT15         ".Int15" //Внутренняя сторона 15
#define LID1_INT16         ".Int16" //Внутренняя сторона 16
#define LID1_INT17         ".Int17" //Внутренняя сторона 17
#define LID1_INT18         ".Int18" //Внутренняя сторона 18
#define LID1_INT19         ".Int19" //Внутренняя сторона 19
#define LID1_INT20         ".Int20" //Внутренняя сторона 20
#define LID1_INT21         ".Int21" //Внутренняя сторона 21
#define LID1_INT22         ".Int22" //Внутренняя сторона 22
#define LID1_INT23         ".Int23" //Внутренняя сторона 23
#define LID1_INT24         ".Int24" //Внутренняя сторона 24
#define LID1_INT25         ".Int25" //Внутренняя сторона 25
#define LID1_INT26         ".Int26" //Внутренняя сторона 26
#define LID1_INT27         ".Int27" //Внутренняя сторона 27
#define LID1_INT28         ".Int28" //Внутренняя сторона 28
#define LID1_INT29         ".Int29" //Внутренняя сторона 29


struct SdLayerDescr {
  const char   *mId;       //Layer id
  unsigned      mColor;    //Layer color
  SdLayerTrace  mTrace;    //Layer trace type
  int           mStratum;  //Layer stratum
  };

//#define LAYER_DESCR_ACTUAL 10 //Actual layers count
//#define LAYER_DESCR_COUNT  20 //Default layers count
extern SdLayerDescr sdLayerDescrActual[];
extern SdLayerDescr sdLayerDescrAddon[];

//Translation layer id to human visible name
struct SdLayerLevel {
    const char *mLid;       //Level id LIDxxx
    const char *mTranslate; //Human visible name aka "Schematic net"
  };

extern SdLayerLevel sdLayerLevel0[];
extern SdLayerLevel sdLayerLevel1[];


class SdLayer
  {
    QString       mId;       //Layer ident - unical handle
    QString       mName;     //Layer name for visual
    SdLayerState  mState;    //State (visible, editing, invisible)
    SdLayerTrace  mTrace;    //Layer trace type
    int           mStratum;  //Layer stratum [Позиция слоя при трассировке (верх, низ, внутри)]
    unsigned      mColor;    //Layer color [Цвет]
    SdLayer      *mPair;     //Layer pair for flipped component [Парный слой]
    bool          mUsage;    //Usage flag [Флаг использования]
  public:
    SdLayer(QString layerId, QString layerName, unsigned layerColor );

    QString      id() const { return mId; }

    QString      name() const { return mName; }
    void         setName( const QString nm ) { mName = nm; }

    SdLayer     *pair() { return mPair; }

    //Установить новую пару для слоя
    void         setPair( SdLayer *p );

    //Reset pair for layer
    void         resetPair();

    //Return true when layer is visible
    bool         isVisible() const { return mState != layerStateOff; }

    //Возвращает истину когда слой редактируемый
    bool         isEdited() const { return mState == layerStateEdit; }

    //Layer state
    SdLayerState state() const { return mState; }
    void         setState( SdLayerState st ) { mState = st; }

    //Layer trace type
    SdLayerTrace trace() const { return mTrace; }
    void         setTrace( SdLayerTrace tr ) { mTrace = tr; }

    int          stratum() const { return mStratum; }
    void         setStratum( int st ) { mStratum = st; }

    //Layer color
    //Return layer color
    QColor       color() const { return QColor( QRgb(mColor) ); }
    //Set new color for layer
    void         setColor( unsigned c ) { mColor = c; }

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
  };

typedef SdLayer *SdLayerPtr;

typedef QMap<QString,SdLayerPtr> SdLayerPtrTable;

#endif // SDLAYER_H
