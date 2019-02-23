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
  { LID0_PCB,                    0x00ffff00, layerTraceBoundary, stmThrough }, //6
  { LID0_WIRE LID1_TOP,          0x00cf0000, layerTraceRoad, stmTop }, //7
  { LID0_WIRE LID1_BOT,          0x000000cf, layerTraceRoad, stmBottom }, //8

  { LID0_POLYGON LID1_TOP,       0x808f0000, layerTracePolygon, stmTop }, //9
  { LID0_POLYGON LID1_BOT,       0x8000008f, layerTracePolygon, stmBottom }, //10

  //{ LID0_PAD,                  0x0000bf00, layerTraceNone, 0x00000000 }, //11
  { LID0_PAD LID1_TOP,           0x00bf0000, layerTracePad, stmTop }, //12
  { LID0_PAD LID1_BOT,           0x000000bf, layerTracePad, stmBottom }, //13

  { LID0_HOLE,                   0x00bf4000, layerTraceHole, stmThrough }, //14

  //{ LID0_CLEAR,                0x803f3f00, layerTraceNone, 0x00000000 }, //15
  { LID0_CLEAR LID1_TOP,         0x803f3f00, layerTraceKeepout, stmTop }, //16
  { LID0_CLEAR LID1_BOT,         0x8040407f, layerTraceKeepout, stmBottom }, //17

  //{ LID0_SOLDER_MASK,          0x803fff00 },
  { LID0_SOLDER_MASK LID1_TOP,   0x803fff00, layerTraceMask, stmTop },
  { LID0_SOLDER_MASK LID1_BOT,   0x8040007f, layerTraceMask, stmBottom },

  { LID0_STENCIL LID1_TOP,       0x003f8000, layerTraceStencil, stmTop },
  { LID0_STENCIL_REPER LID1_TOP, 0x003f4000, layerTraceNone, 0x00000000 },

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
  { LID0_WIRE LID1_INT00, 0x000040cf, layerTraceRoad, stmInt00 },
  { LID0_WIRE LID1_INT01, 0x004040cf, layerTraceRoad, stmInt01 },
  { LID0_WIRE LID1_INT02, 0x004000cf, layerTraceRoad, stmInt02 },
  { LID0_WIRE LID1_INT03, 0x00cf4040, layerTraceRoad, stmInt03 },
  { nullptr, 0, layerTraceNone, 0x00000000 }
};

SdLayerLevel sdLayerLevel0[] = {
  { LID0_COMMON,        QT_TRANSLATE_NOOP( "SdLayer", "Common" ), 0 },
  { LID0_INVISIBLE,     QT_TRANSLATE_NOOP( "SdLayer", "Invisible" ), 0 },

  //Schematic specific
  { LID0_NET,           QT_TRANSLATE_NOOP( "SdLayer", "Schematic net" ), 0 },
  { LID0_NET_NAME,      QT_TRANSLATE_NOOP( "SdLayer", "Schematic net name" ), 0 },
  { LID0_BUS,           QT_TRANSLATE_NOOP( "SdLayer", "Schematic net bus" ), 0 },
  { LID0_AREA,          QT_TRANSLATE_NOOP( "SdLayer", "Schematic pcb area contour" ), 0 },

  //PCB specific
  { LID0_PCB,           QT_TRANSLATE_NOOP( "SdLayer", "PCB contour" ), 0 },
  { LID0_WIRE,          QT_TRANSLATE_NOOP( "SdLayer", "PCB wire" ), 0 },
  { LID0_POLYGON,       QT_TRANSLATE_NOOP( "SdLayer", "PCB polygon" ), 0 },
  { LID0_PAD,           QT_TRANSLATE_NOOP( "SdLayer", "PCB pad" ), 0 },
  { LID0_HOLE,          QT_TRANSLATE_NOOP( "SdLayer", "PCB hole" ), 0 },
  { LID0_CLEAR,         QT_TRANSLATE_NOOP( "SdLayer", "PCB trace clear" ), 0 },
  { LID0_SOLDER_MASK,   QT_TRANSLATE_NOOP( "SdLayer", "PCB solder mask" ), 0 },
  { LID0_STENCIL,       QT_TRANSLATE_NOOP( "SdLayer", "PCB stencil aperture" ), 0 },
  { LID0_STENCIL_REPER, QT_TRANSLATE_NOOP( "SdLayer", "PCB stencil reper" ), 0 },
  { LID0_EXCLUSION,     QT_TRANSLATE_NOOP( "SdLayer", "PCB trace exclusion area" ), 0 },
  { LID0_TRACE,         QT_TRANSLATE_NOOP( "SdLayer", "PCB trace area" ), 0 },
  { LID0_TRACE_DEFAULT, QT_TRANSLATE_NOOP( "SdLayer", "PCB trace default" ), 0 }, //Default trace layer for undefined stratum

  //Both schematic and PCB
  { LID0_COMPONENT,     QT_TRANSLATE_NOOP( "SdLayer", "Component" ), 0 },
  { LID0_PIN,           QT_TRANSLATE_NOOP( "SdLayer", "Pin" ), 0 },
  { LID0_PIN_NAME,      QT_TRANSLATE_NOOP( "SdLayer", "Pin name" ), 0 },
  { LID0_PIN_NUMBER,    QT_TRANSLATE_NOOP( "SdLayer", "Pin number" ), 0 },
  { LID0_IDENT,         QT_TRANSLATE_NOOP( "SdLayer", "Component ident" ), 0 },
  { LID0_PICTURE,       QT_TRANSLATE_NOOP( "SdLayer", "Picture" ), 0 },
  { LID0_REMARK,        QT_TRANSLATE_NOOP( "SdLayer", "Remark" ), 0 },
  { LID0_VALUE,         QT_TRANSLATE_NOOP( "SdLayer", "Value" ), 0 },
  { LID0_CONTOUR,       QT_TRANSLATE_NOOP( "SdLayer", "Contour" ), 0 },
  { nullptr, nullptr, 0 }
};



SdLayerLevel sdLayerLevel1[] = {
  { "",                 QT_TRANSLATE_NOOP( "SdLayer", "---" ), stmThrough },
  { LID1_TOP,           QT_TRANSLATE_NOOP( "SdLayer", "on top" ), stmTop },
  { LID1_BOT,           QT_TRANSLATE_NOOP( "SdLayer", "on bottom" ), stmBottom },
  { LID1_INT00,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 1" ), stmInt00 },
  { LID1_INT01,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 2" ), stmInt01 },
  { LID1_INT02,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 3" ), stmInt02 },
  { LID1_INT03,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 4" ), stmInt03 },
  { LID1_INT04,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 5" ), stmInt04 },
  { LID1_INT05,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 6" ), stmInt05 },
  { LID1_INT06,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 7" ), stmInt06 },
  { LID1_INT07,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 8" ), stmInt07 },
  { LID1_INT08,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 9" ), stmInt08 },
  { LID1_INT09,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 10" ), stmInt09 },
  { LID1_INT10,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 11" ), stmInt10 },
  { LID1_INT11,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 12" ), stmInt11 },
  { LID1_INT12,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 13" ), stmInt12 },
  { LID1_INT13,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 14" ), stmInt13 },
  { LID1_INT14,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 15" ), stmInt14 },
  { LID1_INT15,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 16" ), stmInt15 },
  { LID1_INT16,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 17" ), stmInt16 },
  { LID1_INT17,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 18" ), stmInt17 },
  { LID1_INT18,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 19" ), stmInt18 },
  { LID1_INT19,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 20" ), stmInt19 },
  { LID1_INT20,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 21" ), stmInt20 },
  { LID1_INT21,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 22" ), stmInt21 },
  { LID1_INT22,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 23" ), stmInt22 },
  { LID1_INT23,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 24" ), stmInt23 },
  { LID1_INT24,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 25" ), stmInt24 },
  { LID1_INT25,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 26" ), stmInt25 },
  { LID1_INT26,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 27" ), stmInt26 },
  { LID1_INT27,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 28" ), stmInt27 },
  { nullptr, nullptr, 0 }
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
    mTrace = layerTraceRoad;
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
