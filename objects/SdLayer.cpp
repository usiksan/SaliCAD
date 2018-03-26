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
  { LID0_COMMON,               0x00404040 }, //0
  { LID0_INVISIBLE,            0xff007f7f }, //1
  //Schematic specific
  { LID0_NET,                  0x00ffff7f }, //2
  { LID0_NET_NAME,             0x007f7f00 }, //3
  { LID0_BUS,                  0xff0000ff }, //4
  { LID0_AREA,                 0x007f7fff }, //5
  //PCB specific
  { LID0_PCB,                  0x00ffff00 }, //6
  { LID0_WIRE LID1_TOP,        0x00cf0000 }, //7
  { LID0_WIRE LID1_BOT,        0x000000cf }, //8

  { LID0_POLYGON LID1_TOP,     0x808f0000 }, //9
  { LID0_POLYGON LID1_BOT,     0x8000008f }, //10

  { LID0_PAD,                  0x0000bf00 }, //11
  { LID0_PAD LID1_TOP,         0x00bf0000 }, //12
  { LID0_PAD LID1_BOT,         0x000000bf }, //13

  { LID0_HOLE,                 0x00bf4000 }, //14

  { LID0_CLEAR,                0x803f3f00 }, //15
  { LID0_CLEAR LID1_TOP,       0x803f3f00 }, //16
  { LID0_CLEAR LID1_BOT,       0x8040407f }, //17

  { LID0_SOLDER_MASK,          0x803fff00 },
  { LID0_SOLDER_MASK LID1_TOP, 0x803fff00 },
  { LID0_SOLDER_MASK LID1_BOT, 0x8040007f },

  { LID0_STENSIL,              0x003f8000 },
  { LID0_STENSIL_REPER,        0x003f4000 },

  { LID0_EXCLUSION,            0x00002080 },
  { LID0_TRACE,                0x00200080 },

  //Both schematic and PCB
  { LID0_COMPONENT,            0x00ff0000 },
  { LID0_COMPONENT LID1_BOT,   0x000000ff },
  { LID0_PIN,                  0x0000ff00 },
  { LID0_PIN LID1_BOT,         0x00008040 },
  { LID0_PIN_NAME,             0x000000ff },
  { LID0_PIN_NAME LID1_BOT,    0x00002080 },
  { LID0_PIN_NUMBER,           0x000020ff },
  { LID0_PIN_NUMBER LID1_BOT,  0x0000a040 },
  { LID0_IDENT,                0x00ff7f00 },
  { LID0_IDENT LID1_BOT,       0x00207f00 },

  { LID0_PICTURE,              0x0000ff00 },
  { LID0_REMARK,               0x00ff2000 },
  { LID0_VALUE,                0x00404000 },
  { nullptr, 0 }
};


SdLayerDescr sdLayerDescrAddon[] = {
  { LID0_WIRE LID1_INT00, 0x000040cf },
  { LID0_WIRE LID1_INT01, 0x004040cf },
  { LID0_WIRE LID1_INT02, 0x004000cf },
  { LID0_WIRE LID1_INT03, 0x00cf4040 },
  { nullptr, 0 }
};



SdLayer::SdLayer(QString layerId, QString layerName, unsigned layerColor) :
  mId(layerId),                //Идентификатор слоя
  mName(layerName),            //Имя слоя
  mState(layerStateEdit), //Состояние
  mTrace(layerTraceNone), //Разрешение трассировки на данном слое
  mTracePosition(0),      //Позиция слоя при трассировке (верх, низ, внутри)
  mColor(layerColor),          //Цвет
  mPair(0),               //Парный слой
  mUsage(false)          //Флаг использования
  {
  //Невидимый слой выключить
  if( mId.startsWith(LID0_INVISIBLE) )
    mState = layerStateOff;
  //Трассировочный слой
  if( mId.startsWith(LID0_WIRE) )
    mTrace = layerTraceSignal;
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
     << mTracePosition
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
     >> mTracePosition
     >> mColor
     >> pair;
  mState = static_cast<SdLayerState>(state);
  mTrace = static_cast<SdLayerTrace>(trace);
  mPair = sdEnvir->getLayer( pair );
  }
