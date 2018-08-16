/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implementation
*/
#ifndef SDGRAPHPARTIMP_H
#define SDGRAPHPARTIMP_H

#include "SdGraphTraced.h"
#include "SdStratum.h"
#include "SdPadAssociation.h"
#include "SdBarrier.h"

#include <QMap>

#define SD_TYPE_PART_IMP "PartImp"


class SdPItemPlate;
class SdPItemPart;
class SdGraphPartPin;
class SdPItemSymbol;
class SdPItemComponent;
class SdGraphSymImp;
class SdGraphRoadPin;

//Part implementation pin [Ножка вхождения корпуса]
struct SdPartImpPin {
  SdGraphPartPin *mPin;       //Original pin
  SdGraphSymImp  *mSection;   //Schematic section where pin located
  QString         mPinName;   //Part pin name
  SdPoint         mPosition;  //Pin position in plate context
  SdStratum       mStratum;   //Pin stratum

  SdPartImpPin();

  void        operator = ( const SdPartImpPin &pin );
  void        draw(SdContext *dc , SdPItemPlate *plate, int stratum) const;
  void        drawWithoutPad( SdContext *dc ) const;
  void        drawPad(SdContext *dc , SdPItemPlate *plate, int stratum, const QString highliteNet) const;
  bool        isConnected() const;
  QString     getNetName() const;
  void        accumUsedPin( SdPadMap &map ) const;
  void        accumBarriers(SdPItemPlate *plate, SdBarrierList &dest, int stratum, SdRuleId ruleId, int clearance , int halfWidth, QTransform t) const;

  QJsonObject toJson(const QString pinNumber) const;
  QString     fromJson( SdObjectMap *map, const QJsonObject obj );
  };

typedef QMap<QString,SdPartImpPin> SdPartImpPinTable;





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





//Part implementation
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
    SdPItemComponent      *mComponent;   //Component with pack info
    SdStringMap            mParam;        //Parameters
  public:
    SdGraphPartImp();
    SdGraphPartImp(SdPoint org, SdPropPartImp *prp, SdPItemPart *part, SdPItemComponent *comp, const SdStringMap &param );

    //Information
    //Get full visual ident of part aka D4 or R45
    QString         getIdent() const;
    //Check if there free section slot. If there - setup section and return true
    bool            isSectionFree(int *section, SdPItemPart *part, SdPItemComponent *comp, const SdStringMap &param, SdPItemSymbol *sym );


    //Service
    //Pin link-unlink
    bool            partPinLink(const QString pinNumber, SdGraphSymImp *imp, const QString pinName, bool link );
    //link-unlink section
    void            setLinkSection( int section, SdGraphSymImp *symImp );
    //Check if all section removed, then autodeleted
    void            autoDelete( SdUndo *undo );
    //Save to undo state of all pins
    void            savePins( SdUndo *undo );
    //Accum used pins
    void            accumUsedPins(SdPadMap &map ) const;


    //Renumeration
    QString         getIdentPrefix();
    //Compare partImp's
    bool            compareRenumeration( const SdGraphPartImp *imp ) const;
    //Lower sheet
    bool            getLowerPosAndSheet( SdPoint &dest, int &sheet ) const;
    //Set index
    void            setIdentIndex( int index );

    //Parameters
    //Param full list
    SdStringMap     getParamTable() const { return mParam; }
    //Get param
    QString         getParam( const QString key ) const { return mParam.value( key ); }
    //Set param
    void            setParam( const QString key, const QString val, SdUndo *undo );
    //Get BOM item line
    QString         getBomItemLine() const;

    //Special drawing
    //Draw part without pads
    void            drawWithoutPads( SdContext *cdx );
    //Draw pads only
    void            drawPads( SdContext *cdx, SdStratum stratum, const QString highlightNetName );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_PART_IMP); }
    virtual quint64 getClass() const override { return dctPartImp; }
    virtual void    attach(SdUndo *undo) override;
    virtual void    detach(SdUndo *undo) override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
    virtual bool    isUsed(SdObject *obj) const override;
    virtual void    upgradeProjectItem(SdProjectItem *newItem, SdUndo *undo) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdPropAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    setProp(SdPropSelected &prop) override;
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    setLayerUsage() override;
    virtual bool isVisible() override;
    virtual SdRect getOverRect() const override;
    virtual int behindCursor(SdPoint p) override;
    virtual bool getInfo(SdPoint p, QString &info, bool extInfo) override;
    virtual bool    snapPoint(SdSnapInfo *snap) override;

  private:
    void updatePinsPositions();
    //Pin connection-disconnection by index only for part (internal and undo)
    void            partPinConnectionSet( int pinIndex, SdGraphRoadPin *roadPin );

    friend class SdUndoRecordImpPin;

    // SdGraphTraced interface
  public:
    virtual bool isPointOnNet(SdPoint p, SdStratum stratum, QString *netName, int *destStratum) override;
    virtual void accumNetSegments( SdPlateNetList &netList ) const override;
    virtual void drawStratum(SdContext *dc, int stratum ) override;
    virtual void accumBarriers( SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk ) const override;
  };

#endif // SDGRAPHPARTIMP_H
