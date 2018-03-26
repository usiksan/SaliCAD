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

#include <QString>
#include <QColor>
#include <QDataStream>

//Layer visibility and editing status
enum SdLayerState {
  layerStateOff,  //Layer is off
  layerStateOn,   //Layer is visible
  layerStateEdit  //Layer is visible and editable
  };

//Layer trace status
enum SdLayerTrace {
  layerTraceNone,   //Слой не трассировочный
  layerTraceSignal, //Трассировка сигнальных цепей
  layerTracePower,  //Трассировка цепей питания
  layerTraceJumper, //Слой перемычек
  layerTraceBar,    //Слой барьера
  layerTraceAllBar, //Слой сквозного барьера
  layerTracePad,    //Слой площадок
  layerTraceAllPad, //Сквозные площадки
  layerTraceLast
  };


//Обозначения в идентификаторе слоя
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
#define LID0_SOLDER_MASK          "Mask"         //Маска
#define LID0_STENSIL       "Stensil"      //Трафарет
#define LID0_STENSIL_REPER "StensilReper" //Репер на трафарете
#define LID0_EXCLUSION     "Exclusion"    //Trace exclusion area
#define LID0_TRACE         "Trace"        //Trace area

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


struct SdLayerDescr {
  const char *mId;
  unsigned    mColor;
  };

//#define LAYER_DESCR_ACTUAL 10 //Actual layers count
//#define LAYER_DESCR_COUNT  20 //Default layers count
extern SdLayerDescr sdLayerDescrActual[];
extern SdLayerDescr sdLayerDescrAddon[];


class SdLayer
  {
    QString      mId;             //Layer ident
    QString      mName;           //Layer name for visual
    SdLayerState mState;          //State (visible, editing, invisible)
    SdLayerTrace mTrace;          //Разрешение трассировки на данном слое
    int          mTracePosition;  //Позиция слоя при трассировке (верх, низ, внутри)
    unsigned     mColor;          //Цвет
    SdLayer     *mPair;           //Парный слой
    bool         mUsage;          //Флаг использования
  public:
    SdLayer(QString layerId, QString layerName, unsigned layerColor );

    QString  id() const { return mId; }

    QString  name() const { return mName; }
    void     setName( const QString nm ) { mName = nm; }

    SdLayer *pair() { return mPair; }

    //Установить новую пару для слоя
    void     setPair( SdLayer *p );

    //Reset pair for layer
    void     resetPair();

    //Return true when layer is visible
    bool     isVisible() const { return mState != layerStateOff; }

    //Возвращает истину когда слой редактируемый
    bool     isEdited() const { return mState == layerStateEdit; }

    //Layer state
    SdLayerState state() const { return mState; }
    void         setState( SdLayerState st ) { mState = st; }

    //Layer color
    //Return layer color
    QColor   color() const { return QColor( QRgb(mColor) ); }
    //Set new color for layer
    void     setColor( unsigned c ) { mColor = c; }

    //Usage flag. Indicate layer is used anywhere
    //Сбросить флаг использования
    //Reset flag
    void     resetUsage() { mUsage = false; }
    //Установить флаг использования
    //Set flag
    void     setUsage() { mUsage = true; }
    //Get flag
    bool     isUsage() const { return mUsage; }

    //Сохранить слой
    void     write( QDataStream &os );

    //Загрузить слой
    void     read( QDataStream &is );
  };

typedef SdLayer *SdLayerPtr;

#endif // SDLAYER_H
