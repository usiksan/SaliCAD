/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Symbol Implementation
*/
#ifndef SDGRAPHSYMIMP_H
#define SDGRAPHSYMIMP_H
#include "SdGraph.h"
#include "SdPoint.h"
#include "SdPropText.h"
#include "SdPropSymImp.h"

#include <QString>

class SdGraphArea;
class SdPItemSymbol;
class SdPart;
class SdComponent;
class SdGraphPartImp;

//Ножка вхождения символа
struct SdSymImpPin {
  SdLayer  *mLayer;  //Слой расположения вывода
  SdGraphSymPin *mSymPin;
  SdPartImpPin *mPartPin;
  DInt32    prtPin; //Индекс вывода корпуса
  DPoint    origin; //Точка привязки вывода
  DName     wire;   //Цепь, с которой связан вывод
  DBool     com;    //Флаг подключенности вывода к цепи

  DSymImpPin();

  void operator = ( DSymImpPin &pin );
  void Draw( DContext &dc );
  void Connect( DPoint a, DPoint b, DName &name, DPrtImpPic *prt );
  void IfConnect( DPrtImpPic *prt );
  void Disconnect( DPoint a, DPoint b, DName &name, DPrtImpPic *prt );
  void IfDisconnect( DPrtImpPic *prt );
  void Ucom( DPrtImpPic *prt );
  void PrepareMove( DSheetPic &sheet, DSelectorPic &selector );
  void ConnectTest( DSheetPic &sheet, DPrtImpPic *prt );
  };


class SdGraphSymImp : public SdGraph
  {
    SdGraphArea    *mArea;        //PCB where this symbol implement contains in
    int             mLogSection;  //Logical symbol section number
    int             mLogNumber;   //Logical part number
    QString         mName;        //Name of component
    SdPoint         mOrigin;      //Position of Implement
    SdPropSymImp    mProp;        //Implement properties
    SdPropText      mIdentProp;   //Part identificator text properties
    QString         mPrefix;      //Part identificator prefix
    SdPoint         mPrefixPos;   //Part identificator prefix position in sheet context

    SdComponent    *mComponent;
    SdPItemSymbol       *mSymbol;
    SdPart         *mPart;
    SdGraphPartImp *mPartImp;
    DSymImpPic      *prevPtr; //Указатель для совместимости с В10
    DPrtImpPic      *prtImp;  //Ссылка на корпус
    DPrtPic         *part;    //Корпус
    DSymbolPic      *symbol;  //Символ
    DSymImpPinTable  pins;    //Информация о номерах выводов
    DParam           param;   //Конкретные параметры элемента
  public:
    SdGraphSymImp();
  };

#endif // SDGRAPHSYMIMP_H
