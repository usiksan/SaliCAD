/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDGRAPHPARTIMP_H
#define SDGRAPHPARTIMP_H

#include "SdGraph.h"

class SdPItemPlate;
class SdPItemPart;
class SdGraphPartPin;

//Ножка вхождения корпуса
struct SdPartImpPin {
  SdGraphPartPin *mPin;
  QString         mPinNumber; //Part pin number
  QString         mPinName;   //Part pin name
  QString         mWireName;  //Name of net pin conneted to
  SdPoint         mPosition;  //Pin position in plate context
  //DPrtPinProp  prop;      //Свойства вывода: Тип и слой вывода
  bool            mCom;       //Pin to wire flag connection
  SdPItemPart    *mPadStack;  //Pad stack

  SdPartImpPin();

  void setConnection( const QString wireName, bool com );

  void Connect( CPChar wire );  //Подключить вывод к цепи
  void Disconnect();            //Отключить вывод от цепи
  void Draw( DContext &dc, DAngle angle, DMirror mirror, DSide side );    //Рисовать вывод
  void CalcSide( DSide side );
  //void SetPadStack( bool connect, DPadTable &table );
  };

typedef QVector<SdPartImpPin> SdPartImpPinTable;

class SdGraphPartImp : public SdGraph
  {
    SdPartImpPinTable mPins;
  public:
    SdGraphPartImp();

    //Pin connection-disconnection by index
    void          pinConnectionSet( int pinIndex, const QString wireName, bool com );
    QString       pinWireName( int pinIndex ) const;
    bool          isPinConnected( int pinIndex ) const;

    //void pinConnectionSet( const QString pinNumber, const QString wireName, bool com );

    SdPItemPlate *getPlate() const;
  };

#endif // SDGRAPHPARTIMP_H
