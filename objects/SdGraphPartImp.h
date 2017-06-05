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

#include "SdGraphTraced.h"
#include "SdStratum.h"

#define SD_TYPE_PART_IMP "PartImp"


class SdPItemPlate;
class SdPItemPart;
class SdGraphPartPin;
class SdPItemSymbol;
class SdGraphSymImp;
class SdGraphRoadPin;

//Ножка вхождения корпуса
struct SdPartImpPin {
  SdGraphPartPin *mPin;       //Original pin
  QString         mPinNumber; //Part pin number
  QString         mPinName;   //Part pin name
  QString         mNetName;   //Name of net pin conneted to
  SdPoint         mPosition;  //Pin position in plate context
  bool            mCom;       //Pin to wire flag connection
  SdPItemPart    *mPadStack;  //Pad stack
  SdStratum       mStratum;   //Pin stratum
  //Unsaved
  int             mSubNetIndex; //Index of subnet to detecting subnets

  SdPartImpPin();

  void operator = ( const SdPartImpPin &pin );
  void draw( SdContext *dc );

  QJsonObject toJson() const;
  void        fromJson( SdObjectMap *map, const QJsonObject obj );
  };

typedef QVector<SdPartImpPin> SdPartImpPinTable;



//Information about section in part implementation
struct SdPartImpSection {
  SdPItemSymbol *mSymbol; //Symbol of this section
  SdGraphSymImp *mSymImp; //Symbol implementation of this section

  SdPartImpSection() : mSymbol(nullptr), mSymImp(nullptr) {}

  QJsonObject toJson() const;
  void        fromJson( SdObjectMap *map, const QJsonObject obj );

  bool        isFree( SdPItemSymbol *symbol ) { return mSymbol == symbol && mSymImp == nullptr; }
  };

typedef QVector<SdPartImpSection> SdPartImpSectionTable;




class SdGraphPartImp : public SdGraphTraced
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
    SdPItemSymbol         *mComponent;   //Component
  public:
    SdGraphPartImp();
    SdGraphPartImp(SdPoint org, SdPropPartImp *prp, SdPItemPart *part, SdPItemSymbol *comp );

    //Information
    //Get full visual ident of part aka D4 or R45
    QString         getIdent() const;
    //Get wire name pin with pinIndex connected to
    QString         pinNetName( int pinIndex ) const;
    //Pin position
    SdPoint         pinPosition( int pinIndex ) const;
    //Return if pin with pinIndex connected to any wire or not
    bool            isPinConnected( int pinIndex ) const;
    //Return pin index of pinNumber
    int             getPinIndex( const QString pinNumber ) const;
    //Check if there free section slot. If there - setup section and return true
    bool            isSectionFree(int *section, SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *sym );


    //Service
    //Set pin name for pin index
    void            setPinName( int pinIndex, const QString pinName );
    //Pin connection-disconnection by index
    void            pinConnectionSet(int pinIndex, const QString netName, bool com );
    //link-unlink section
    void            setLinkSection( int section, SdGraphSymImp *symImp );


    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_PART_IMP); }
    virtual quint64 getClass() const override { return dctPartImp; }
    virtual void    attach(SdUndo *undo) override;
    virtual void    detach(SdUndo *undo) override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    move(SdPoint offset) override;
    virtual void rotate(SdPoint center, SdAngle angle) override;
    virtual void mirror(SdPoint a, SdPoint b) override;
    virtual void setProp(SdProp &prop) override;
    virtual void getProp(SdProp &prop) override;
    virtual void selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void select(SdSelector *selector) override;
    virtual bool canHideLayer(SdLayer *layer) override;
    virtual bool isVisible() override;
    virtual SdRect getOverRect() const override;
    virtual void draw(SdContext *dc) override;
    virtual int behindCursor(SdPoint p) override;
    virtual bool getInfo(SdPoint p, QString &info, bool extInfo) override;
    virtual bool snapPoint(SdSnapInfo *snap) override;

  private:
    void updatePinsPositions();
    //Pin connection-disconnection by index only for part (internal and undo)
    void            partPinConnectionSet( int pinIndex, SdGraphRoadPin *roadPin );

    friend class SdUndoRecordImpPin;

    // SdGraphTraced interface
  public:
    virtual bool isPointOnNet(SdPoint p, SdStratum stratum, QString &netName) override;
    virtual void unionSubNet(int srcSubNet, int dstSubNet) override;
  };

#endif // SDGRAPHPARTIMP_H
