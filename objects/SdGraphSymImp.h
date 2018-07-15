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
#include "SdSymImpPin.h"
#include "library/SdStringMap.h"
#include "SdPItemComponent.h"


#define SD_TYPE_SYM_IMP "SymImp"

class SdGraphArea;
class SdPItemSymbol;
class SdPItemSheet;
class SdPItemPart;
class SdGraphPartImp;
class SdPItemPlate;
class SdGraphNetWire;


class SdGraphSymImp : public SdGraph
  {
    SdGraphArea      *mArea;         //PCB where this symbol implement contains in
    int               mSectionIndex; //Section index (from 0)
    int               mLogSection;   //Logical symbol section number (from 1, 0 - not assigned)
    int               mLogNumber;    //Logical part number (from 1, 0 - not assigned)
    SdPoint           mOrigin;       //Position of Implement
    SdPropSymImp      mProp;         //Implement properties
    SdRect            mOverRect;     //Over rect
    QString           mPrefix;       //Part identificator prefix
    SdPropText        mIdentProp;    //Part identificator text properties
    SdPoint           mIdentOrigin;  //Part identificator position in symbol context
    SdPoint           mIdentPos;     //Part identificator position in sheet context
    SdRect            mIdentRect;    //Part identificator over rect

    SdPItemComponent *mComponent;    //Object contains section information, pin assotiation info. May be same as mSymbol.
    SdPItemSymbol    *mSymbol;       //Symbol contains graph information
    SdPItemPart      *mPart;         //Part for partImp construction
    SdGraphPartImp   *mPartImp;      //Part implement in desired plate
                                     //If mPartImp == nullptr then symbol not linked
    SdSymImpPinTable  mPins;         //Pin information table
    SdStringMap       mParam;        //Parameters
    QString           mLinkError;    //Error string, which displays when no link
  public:
    SdGraphSymImp();

    //comp - contains pack info
    //sym  - contains graphics
    //part - part type
    SdGraphSymImp(SdPItemComponent *comp, SdPItemSymbol *sym, SdPItemPart *part , SdPoint pos, SdPropSymImp *prp);



    //Information
    //Return current plate of section
    SdPItemPlate *currentPlate() const;
    QString       getRenumSect( SdPoint &dest, int &sheetNumber );
    //Pin information
    //Pin presention
    bool          isPinPresent( const QString pinName ) const { return mPins.contains(pinName); }
    //Pin connection status
    bool          isPinConnected( const QString pinName ) const;
    //Pin net name
    QString       pinNetName( const QString pinName ) const;
    //Param full list
    SdStringMap   getParamTable() const { return mParam; }
    //Get param
    QString       getParam( const QString key ) { return mParam.value( key ); }
    //Set param
    void          setParam( const QString key, const QString val, SdUndo *undo );
    //Get BOM item line
    QString       getBomItemLine() const;


    //Ident edit
    //Get full visual ident of section aka D4.2
    QString       getIdent() const;
    //Get separated ident information
    QString       getIdentInfo( int &logNumber, int &logSection );
    //Set ident information
    void          setIdentInfo( const QString prefix, int logNumber, int logSection );
    //Move ident regarding symbol implement
    void          moveIdent( SdPoint offset );
    //Get ident properties
    void          getIdentProp( SdProp &prop );
    //Set ident properties
    void          setIdentProp( const SdProp &prop );



    //Notifications about segment operation
    //Notification about wire segment position changed
    void          netWirePlace( SdGraphNetWire *wire, SdUndo *undo );
    //Notification about wire segment deletion
    void          netWireDelete(SdGraphNetWire *wire, SdUndo *undo ); //Извещение об удалении сегмента
    //Accumulate segments connected to component
    void          accumLinked(SdPoint a, SdPoint b, const QString netName, SdSelector *sel );

    //Service
    //Pin status get
    void          pinStatusGet( const QString pinName, SdSymImpPin &pin ) const;
    //Pin status set
    void          pinStatusSet( const QString pinName, const SdSymImpPin &pin );
    //Move section to plate
    void          moveToPlate( SdPItemPlate *plate, SdUndo *undo );
    //Link with partImp
    void          setLinkSection(int section, SdGraphPartImp *partImp );
    //Unconnect pin in point
    void          unconnectPinInPoint(SdPoint p , SdUndo *undo, const QString undoTitle);
    //Unlink symbol from part
    void          unLinkPart( SdUndo *undo );
    //Link auto partImp. partImp and section are selected automatic
    void          linkAutoPart( SdUndo *undo );
    //TODO D017 Accum auto net
    //void          autoNet( DNetListTable &table );         //Накопить цепи в текстовый список цепей

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_SYM_IMP ); }
    virtual quint64 getClass() const override { return dctSymImp; }
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
    virtual void    moveComplete( SdPoint grid, SdUndo *undo ) override;
    virtual void    prepareMove( SdUndo *undo ) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdPropAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    setProp(SdPropSelected &prop) override;
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual bool    getInfo(SdPoint p, QString &info, bool extInfo) override;
    virtual bool    snapPoint(SdSnapInfo *snap) override;


  private:
    SdPItemSheet *getSheet() const;
    //Update pins positions upon position and props of symbol implement
    void          updatePinsPositions();
    //Link auto partImp in given plate. partImp and section are selected automatic
    void          linkAutoPartInPlate( SdPItemPlate *plate, SdUndo *undo );
    //Pin connection-disconnection by name for symbol and part implements
    void          pinConnectionSet(const QString pinName, const QString netName, SdUndo *undo );
    //Update connection status for all pins
    void          updatePinsConnectionStatus( SdUndo *undo );

  };

#endif // SDGRAPHSYMIMP_H
