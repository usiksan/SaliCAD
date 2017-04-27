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
#include "SdParamTable.h"

#include <QString>
#include <QList>

class SdGraphArea;
class SdPItemSymbol;
class SdPItemSheet;
class SdPart;
class SdComponent;
class SdGraphPartImp;

//Ножка вхождения символа
struct SdSymImpPin {
  SdLayer  *mLayer;     //Pin layer
  QString   mPinName;   //Pin name in symbol
  QString   mPinNumber; //Pin number in part
  SdPoint   mOrigin;    //Pin point
  QString   mWireName;  //Net, which pin connected to
  bool      mCom;       //State of pin to net connectivity

  SdSymImpPin();

  void operator = ( SdSymImpPin &pin );
  void draw( SdContext *dc );
  void connect( SdPoint a, SdPoint b, const QString &name, SdGraphPartImp *prt );
  void ifConnect( SdGraphPartImp *prt );
  void disconnect( SdPoint a, SdPoint b, const QString &name, SdGraphPartImp *prt );
  void ifDisconnect( SdGraphPartImp *prt );
  void ucom( SdGraphPartImp *prt );
  void prepareMove( SdPItemSheet *sheet, SdSelector *selector );
  void connectTest( SdPItemSheet *sheet, SdGraphPartImp *prt );
  };


typedef QList<SdSymImpPin> SdSymImpPinTable;


class SdGraphSymImp : public SdGraph
  {
    SdGraphArea      *mArea;        //PCB where this symbol implement contains in
    int               mLogSection;  //Logical symbol section number
    int               mLogNumber;   //Logical part number
    QString           mName;        //Name of component
    SdPoint           mOrigin;      //Position of Implement
    SdPropSymImp      mProp;        //Implement properties
    SdPropText        mIdentProp;   //Part identificator text properties
    QString           mPrefix;      //Part identificator prefix
    SdPoint           mPrefixPos;   //Part identificator prefix position in sheet context

    SdComponent      *mComponent;
    SdPItemSymbol    *mSymbol;
    SdPart           *mPart;
    SdGraphPartImp   *mPartImp;
    SdSymImpPinTable  mPins;        //Pin information table
    SdParamTable      mParam;       //Parameters
  public:
    SdGraphSymImp();
  };

#endif // SDGRAPHSYMIMP_H
