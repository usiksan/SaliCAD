/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdLayer.h"
#include "SdEnvir.h"

SdLayerDescr sdLayerDescrActual[] = {
  { LID0_COMMON,                 0x00404040, layerTraceNone, 0x00000000 }, //0
  { LID0_INVISIBLE,              0xff007f7f, layerTraceNone, 0x00000000 }, //1
  //Schematic specific
  { LID0_NET,                    0x00ffff7f, layerTraceNone, 0x00000000 }, //2
  { LID0_NET_NAME,               0x007f7f00, layerTraceNone, 0x00000000 }, //3
  { LID0_BUS,                    0xff0000ff, layerTraceNone, 0x00000000 }, //4
  { LID0_AREA,                   0x007f7fff, layerTraceNone, 0x00000000 }, //5
  //PCB specific
  { LID0_PCB,                    0x00ffff00, layerTraceBoundary, stmThrow }, //6
  { LID0_WIRE LID1_TOP,          0x00cf0000, layerTraceWire, stmTop }, //7
  { LID0_WIRE LID1_BOT,          0x000000cf, layerTraceWire, stmBottom }, //8

  { LID0_POLYGON LID1_TOP,       0x808f0000, layerTracePolygon, stmTop }, //9
  { LID0_POLYGON LID1_BOT,       0x8000008f, layerTracePolygon, stmBottom }, //10

  //{ LID0_PAD,                  0x0000bf00, layerTraceNone, 0x00000000 }, //11
  { LID0_PAD LID1_TOP,           0x00bf0000, layerTracePad, stmTop }, //12
  { LID0_PAD LID1_BOT,           0x000000bf, layerTracePad, stmBottom }, //13

  { LID0_HOLE,                   0x00bf4000, layerTraceHole, stmThrow }, //14

  //{ LID0_CLEAR,                0x803f3f00, layerTraceNone, 0x00000000 }, //15
  { LID0_CLEAR LID1_TOP,         0x803f3f00, layerTraceKeepout, stmTop }, //16
  { LID0_CLEAR LID1_BOT,         0x8040407f, layerTraceKeepout, stmBottom }, //17

  //{ LID0_SOLDER_MASK,          0x803fff00 },
  { LID0_SOLDER_MASK LID1_TOP,   0x803fff00, layerTraceMask, stmTop },
  { LID0_SOLDER_MASK LID1_BOT,   0x8040007f, layerTraceMask, stmBottom },

  { LID0_STENSIL LID1_TOP,       0x003f8000, layerTraceStensil, stmTop },
  { LID0_STENSIL_REPER LID1_TOP, 0x003f4000, layerTraceNone, 0x00000000 },

  { LID0_EXCLUSION,              0x00002080, layerTraceNone, 0x00000000 },
  { LID0_TRACE,                  0x00200080, layerTraceNone, 0x00000000 },
  { LID0_TRACE_DEFAULT,          0x00404020, layerTraceNone, 0x00000000 }, //Default trace layer for undefined stratum

  //Both schematic and PCB
  { LID0_COMPONENT,            0x00ff0000, layerTraceNone, 0x00000000 },
  { LID0_COMPONENT LID1_BOT,   0x000000ff, layerTraceNone, 0x00000000 },
  { LID0_PIN,                  0x0000ff00, layerTraceNone, 0x00000000 },
  { LID0_PIN LID1_BOT,         0x00008040, layerTraceNone, 0x00000000 },
  { LID0_PIN_NAME,             0x000000ff, layerTraceNone, 0x00000000 },
  { LID0_PIN_NAME LID1_BOT,    0x00002080, layerTraceNone, 0x00000000 },
  { LID0_PIN_NUMBER,           0x000020ff, layerTraceNone, 0x00000000 },
  { LID0_PIN_NUMBER LID1_BOT,  0x0000a040, layerTraceNone, 0x00000000 },
  { LID0_IDENT,                0x00ff7f00, layerTraceNone, 0x00000000 },
  { LID0_IDENT LID1_BOT,       0x00207f00, layerTraceNone, 0x00000000 },

  { LID0_PICTURE,              0x0000ff00, layerTraceNone, 0x00000000 },
  { LID0_REMARK,               0x00ff2000, layerTraceNone, 0x00000000 },
  { LID0_VALUE,                0x00404000, layerTraceNone, 0x00000000 },
  { nullptr, 0, layerTraceNone, 0x00000000 }
};


SdLayerDescr sdLayerDescrAddon[] = {
  { LID0_WIRE LID1_INT00, 0x000040cf, layerTraceWire, stmInt00 },
  { LID0_WIRE LID1_INT01, 0x004040cf, layerTraceWire, stmInt01 },
  { LID0_WIRE LID1_INT02, 0x004000cf, layerTraceWire, stmInt02 },
  { LID0_WIRE LID1_INT03, 0x00cf4040, layerTraceWire, stmInt03 },
  { nullptr, 0, layerTraceNone, 0x00000000 }
};

SdLayerLevel sdLayerLevel0[] = {
  { LID0_COMMON,        QT_TR_NOOP( "Common" ) },
  { LID0_INVISIBLE,     QT_TR_NOOP( "Invisible" ) },

  //Schematic specific
  { LID0_NET,           QT_TR_NOOP( "Schematic net" ) },
  { LID0_NET_NAME,      QT_TR_NOOP( "Schematic net name" ) },
  { LID0_BUS,           QT_TR_NOOP( "Schematic net bus" ) },
  { LID0_AREA,          QT_TR_NOOP( "Schematic pcb area contour" ) },

  //PCB specific
  { LID0_PCB,           QT_TR_NOOP( "PCB contour" ) },
  { LID0_WIRE,          QT_TR_NOOP( "PCB wire" ) },
  { LID0_POLYGON,       QT_TR_NOOP( "PCB polygon" ) },
  { LID0_PAD,           QT_TR_NOOP( "PCB pad" ) },
  { LID0_HOLE,          QT_TR_NOOP( "PCB hole" ) },
  { LID0_CLEAR,         QT_TR_NOOP( "PCB trace clear" ) },
  { LID0_SOLDER_MASK,   QT_TR_NOOP( "PCB solder mask" ) },
  { LID0_STENSIL,       QT_TR_NOOP( "PCB stensil aperture" ) },
  { LID0_STENSIL_REPER, QT_TR_NOOP( "PCB stensil reper" ) },
  { LID0_EXCLUSION,     QT_TR_NOOP( "PCB trace exclusion area" ) },
  { LID0_TRACE,         QT_TR_NOOP( "PCB trace area" ) },
  { LID0_TRACE_DEFAULT, QT_TR_NOOP( "PCB trace default" ) }, //Default trace layer for undefined stratum

  //Both schematic and PCB
  { LID0_COMPONENT,     QT_TR_NOOP( "Component" ) },
  { LID0_PIN,           QT_TR_NOOP( "Pin" ) },
  { LID0_PIN_NAME,      QT_TR_NOOP( "Pin name" ) },
  { LID0_PIN_NUMBER,    QT_TR_NOOP( "Pin number" ) },
  { LID0_IDENT,         QT_TR_NOOP( "Component ident" ) },
  { LID0_PICTURE,       QT_TR_NOOP( "Picture" ) },
  { LID0_REMARK,        QT_TR_NOOP( "Remark" ) },
  { LID0_VALUE,         QT_TR_NOOP( "Value" ) },
  { LID0_CONTOUR,       QT_TR_NOOP( "Contour" ) },
  { nullptr, nullptr }
};



SdLayerLevel sdLayerLevel1[] = {
  { "",                 QT_TR_NOOP( "---" ) },
  { LID1_TOP,           QT_TR_NOOP( "on top" ) },
  { LID1_BOT,           QT_TR_NOOP( "on bottom" ) },
  { LID1_INT00,         QT_TR_NOOP( "on internal 1" ) },
  { LID1_INT01,         QT_TR_NOOP( "on internal 2" ) },
  { LID1_INT02,         QT_TR_NOOP( "on internal 3" ) },
  { LID1_INT03,         QT_TR_NOOP( "on internal 4" ) },
  { LID1_INT04,         QT_TR_NOOP( "on internal 5" ) },
  { LID1_INT05,         QT_TR_NOOP( "on internal 6" ) },
  { LID1_INT06,         QT_TR_NOOP( "on internal 7" ) },
  { LID1_INT07,         QT_TR_NOOP( "on internal 8" ) },
  { LID1_INT08,         QT_TR_NOOP( "on internal 9" ) },
  { LID1_INT09,         QT_TR_NOOP( "on internal 10" ) },
  { LID1_INT10,         QT_TR_NOOP( "on internal 11" ) },
  { LID1_INT11,         QT_TR_NOOP( "on internal 12" ) },
  { LID1_INT12,         QT_TR_NOOP( "on internal 13" ) },
  { LID1_INT13,         QT_TR_NOOP( "on internal 14" ) },
  { LID1_INT14,         QT_TR_NOOP( "on internal 15" ) },
  { LID1_INT15,         QT_TR_NOOP( "on internal 16" ) },
  { LID1_INT16,         QT_TR_NOOP( "on internal 17" ) },
  { LID1_INT17,         QT_TR_NOOP( "on internal 18" ) },
  { LID1_INT18,         QT_TR_NOOP( "on internal 19" ) },
  { LID1_INT19,         QT_TR_NOOP( "on internal 20" ) },
  { LID1_INT20,         QT_TR_NOOP( "on internal 21" ) },
  { LID1_INT21,         QT_TR_NOOP( "on internal 22" ) },
  { LID1_INT22,         QT_TR_NOOP( "on internal 23" ) },
  { LID1_INT23,         QT_TR_NOOP( "on internal 24" ) },
  { LID1_INT24,         QT_TR_NOOP( "on internal 25" ) },
  { LID1_INT25,         QT_TR_NOOP( "on internal 26" ) },
  { LID1_INT26,         QT_TR_NOOP( "on internal 27" ) },
  { LID1_INT27,         QT_TR_NOOP( "on internal 28" ) },
  { LID1_INT28,         QT_TR_NOOP( "on internal 29" ) },
  { LID1_INT29,         QT_TR_NOOP( "on internal 30" ) },
  { nullptr, nullptr }
};


SdLayer::SdLayer(QString layerId, QString layerName, unsigned layerColor) :
  mId(layerId),                //Идентификатор слоя
  mName(layerName),            //Имя слоя
  mState(layerStateEdit), //Состояние
  mTrace(layerTraceNone), //Разрешение трассировки на данном слое
  mStratum(0),      //Позиция слоя при трассировке (верх, низ, внутри)
  mColor(layerColor),          //Цвет
  mPair(nullptr),               //Парный слой
  mUsage(false)          //Флаг использования
  {
  //Невидимый слой выключить
  if( mId.startsWith(LID0_INVISIBLE) )
    mState = layerStateOff;
  //Трассировочный слой
  if( mId.startsWith(LID0_WIRE) )
    mTrace = layerTraceWire;
  //По умолчанию пара - этот-же слой
  //By default, pair - is same layer
  mPair = this;
  }




//Установить новую пару для слоя
void SdLayer::setPair(SdLayer *p)
  {
  //Reset existing pair
  resetPair();
  //Set new one
  mPair = p;
  p->mPair = this;
  }




//Reset pair for layer
void SdLayer::resetPair()
  {
  //Check if pair exist
  if( mPair ) {
    //If it's - then reset existing pair
    mPair->mPair = mPair;
    mPair = this;
    }
  }




//Сохранить слой
void SdLayer::write(QDataStream &os)
  {
  os << mId
     << mName
     << static_cast<int>(mState)
     << static_cast<int>(mTrace)
     << mStratum
     << mColor
     << mPair->mId;
  }




//Загрузить слой
void SdLayer::read(QDataStream &is)
  {
  int state, trace;
  QString pair;
  is >> mId
     >> mName
     >> state
     >> trace
     >> mStratum
     >> mColor
     >> pair;
  mState = static_cast<SdLayerState>(state);
  mTrace = static_cast<SdLayerTrace>(trace);
  mPair = sdEnvir->getLayer( pair );
  }
