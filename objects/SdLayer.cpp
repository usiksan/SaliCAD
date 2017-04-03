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
  if( mId.startsWith(LID0_TRACE) )
    mTrace = layerTraceSignal;
  //По умолчанию пара - этот-же слой
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
    mPair->mPair = 0;
    mPair = 0;
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
