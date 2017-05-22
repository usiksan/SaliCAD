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
class SdPItemSymbol;
class SdGraphSymImp;

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

  void operator = ( SdPartImpPin &pin );
  void setConnection( const QString wireName, bool com );
  void draw( SdContext *dc );

  QJsonObject toJson();
  void        fromJson( SdObjectMap *map, const QJsonObject obj );
//  void Connect( CPChar wire );  //Подключить вывод к цепи
//  void Disconnect();            //Отключить вывод от цепи
//  void Draw( DContext &dc, DAngle angle, DMirror mirror, DSide side );    //Рисовать вывод
//  void CalcSide( DSide side );
  //void SetPadStack( bool connect, DPadTable &table );
  };

typedef QVector<SdPartImpPin> SdPartImpPinTable;



//Information about section in part implementation
struct SdPartImpSection {
  SdPItemSymbol *mSymbol; //Symbol of this section
  SdGraphSymImp *mSymImp; //Symbol implementation of this section

  SdPartImpSection() : mSymbol(nullptr), mSymImp(nullptr) {}

  QJsonObject toJson();
  void        fromJson( SdObjectMap *map, const QJsonObject obj );
  };

typedef QVector<SdPartImpSection> SdPartImpSectionTable;




class SdGraphPartImp : public SdGraph
  {
    int                    mLogNumber;   //Logical part number (from 1)
    SdPoint                mOrigin;      //Position of Implement
    SdPropPartImp          mProp;        //Implement properties
    SdRect                 mOverRect;    //Over rect
    QString                mPrefix;      //Part identificator prefix
    SdPropText             mIdentProp;   //Part identificator text properties
    SdPoint                mIdentOrigin; //Part identificator position in symbol context
    SdPoint                mIdentPos;    //Part identificator position in sheet context
    SdRect                 mIdentRect;   //Part identificator over rect

    SdPartImpPinTable      mPins;
    SdPartImpSectionTable  mSections;
    SdPItemPart           *mPart;        //Part for this implementation
    SdPItemSymbol         *mComp;        //Component
  public:
    SdGraphPartImp();
    SdGraphPartImp( SdPoint org, SdPItemPart *part, SdPItemSymbol *comp );

    //Information
    //Return plate where part resides
    SdPItemPlate *getPlate() const;
    //Get full visual ident of part aka D4 or R45
    QString       getIdent() const;
    //Get wire name pin with pinIndex connected to
    QString       pinWireName( int pinIndex ) const;
    //Return if pin with pinIndex connected to any wire or not
    bool          isPinConnected( int pinIndex ) const;
    //Return pin index of pinNumber
    int           getPinIndex( const QString pinNumber ) const;
    //Check if there free section slot. If there - setup section and return true
    bool          isSectionFree( int *section, SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *symbol );
    //link-unlink section
    void          setLinkSection( int section, SdGraphSymImp *symImp );


    //Service
    //Pin connection-disconnection by index
    void          pinConnectionSet( int pinIndex, const QString wireName, bool com );

  };

#endif // SDGRAPHPARTIMP_H
