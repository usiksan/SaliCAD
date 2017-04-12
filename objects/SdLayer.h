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
#define LID0_SCHEMATIC     "Sch" //Схема
#define LID0_PCB_TOP       "Pct" //Плата сверху
#define LID0_PCB_BOT       "Pcb" //Плата снизу
#define LID0_INVISIBLE     "Inv" //Невидимые объекты

#define LID0_TRACE         "Trc" //Слой трассировки
#define LID0_POLYGON       "Pgn" //Слой полигона
#define LID0_PADS          "Pad" //Площадки
#define LID0_HOLE          "Hol" //Отверстие
#define LID0_CLEAR         "Clr" //Зазор
#define LID0_MASK          "Msk" //Маска
#define LID0_STENSIL       "Sts" //Трафарет
#define LID0_STENSIL_REPER "Srp" //Репер на трафарете

#define LID1_TOP           "Top" //Верхняя сторона
#define LID1_BOT           "Bot" //Нижняя сторона
#define LID1_INT00         "I00" //Внутренняя сторона 0
#define LID1_INT01         "I01" //Внутренняя сторона 1
#define LID1_INT02         "I02" //Внутренняя сторона 2
#define LID1_INT03         "I03" //Внутренняя сторона 3

#define LID1_BORDER        "Brd" //Границы

#define LID1_ELEM          "Elm" //Изображение элемента (символа или корпуса)
#define LID1_PIN           "Pin" //Изображение вывода
#define LID1_PIN_NAME      "Nam" //Изображение имени вывода
#define LID1_PIN_NUMBER    "Num" //Изображение номера вывода
#define LID1_IDENT         "Idn" //Изображение идентификатора
#define LID1_NET           "Net" //Изображение цепей
#define LID1_NET_NAME      "Ntn" //Изображение имен цепей
#define LID1_BUS           "Bus" //Изображение шин
#define LID1_PICTURE       "Pic" //Произвольный рисунок
#define LID1_REMARK        "Rem" //Коментарий
#define LID1_VALUES        "Val" //Слой значений
#define LID1_COUNTUR       "Ctr" //Контур


class SdLayer
  {
    QString      mId;             //Идентификатор слоя
    QString      mName;           //Имя слоя
    SdLayerState mState;          //Состояние
    SdLayerTrace mTrace;          //Разрешение трассировки на данном слое
    int          mTracePosition;  //Позиция слоя при трассировке (верх, низ, внутри)
    unsigned     mColor;          //Цвет
    SdLayer     *mPair;           //Парный слой
    bool         mUsage;          //Флаг использования
  public:
    SdLayer(QString layerId, QString layerName, unsigned layerColor );

    QString  id() const { return mId; }

    QString  name() const { return mName; }

    SdLayer *pair() { return mPair; }

    //Установить новую пару для слоя
    void     setPair( SdLayer *p );

    //Reset pair for layer
    void     resetPair();

    //Return true when layer is visible
    bool     isVisible() const { return mState != layerStateOff; }

    //Возвращает истину когда слой редактируемый
    bool     isEdited() const { return mState == layerStateEdit; }

    //Return layer color
    QColor   color() const { return QColor( QRgb(mColor) ); }

    //Сбросить флаг использования
    void     resetUsage() { mUsage = false; }

    //Установить флаг использования
    void     setUsage() { mUsage = true; }

    //Сохранить слой
    void     write( QDataStream &os );

    //Загрузить слой
    void     read( QDataStream &is );
  };

typedef SdLayer *SdLayerPtr;

#endif // SDLAYER_H
