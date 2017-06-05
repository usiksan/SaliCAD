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
#include <QVector>

#define SD_TYPE_SYM_IMP "SymImp"

class SdGraphArea;
class SdGraphSymPin;
class SdPItemSymbol;
class SdPItemSheet;
class SdPItemPart;
class SdGraphPartImp;
class SdPItemPlate;

//Pin for symbol implementation
struct SdSymImpPin {
  SdGraphSymPin  *mPin;       //Pin
  QString         mPinName;   //Pin name in symbol
  QString         mPinNumber; //Pin number in part
  SdPoint         mPosition;  //Pin position in sheet context
  QString         mWireName;  //Net, which pin connected to
  bool            mCom;       //State of pin to net connectivity
  int             mPrtPin;    //Pin index in pin array of part implement

  SdSymImpPin();

  void operator = (const SdSymImpPin &pin );
  void        draw( SdContext *dc );
  void        setConnection( const QString wireName, bool com );
  void        setConnection( SdGraphPartImp *partImp, const QString wireName, bool com );
  bool        isCanConnect( SdPoint a, SdPoint b ) const;
  bool        isCanDisconnect( SdPoint a, SdPoint b, const QString wireName ) const;
  void        prepareMove( SdPItemSheet *sheet, SdSelector *selector );

  QJsonObject toJson() const;
  void        fromJson( SdObjectMap *map, const QJsonObject obj );

  };


typedef QVector<SdSymImpPin> SdSymImpPinTable;


class SdGraphSymImp : public SdGraph
  {
    SdGraphArea      *mArea;        //PCB where this symbol implement contains in
    int               mSectionIndex;//Section index (from 0)
    int               mLogSection;  //Logical symbol section number (from 1)
    int               mLogNumber;   //Logical part number (from 1)
    SdPoint           mOrigin;      //Position of Implement
    SdPropSymImp      mProp;        //Implement properties
    SdRect            mOverRect;    //Over rect
    QString           mPrefix;      //Part identificator prefix
    SdPropText        mIdentProp;   //Part identificator text properties
    SdPoint           mIdentOrigin; //Part identificator position in symbol context
    SdPoint           mIdentPos;    //Part identificator position in sheet context
    SdRect            mIdentRect;   //Part identificator over rect

    SdPItemSymbol    *mComponent;   //Object contains section information, pin assotiation info. May be same as mSymbol.
    SdPItemSymbol    *mSymbol;      //Symbol contains graph information
    SdPItemPart      *mPart;
    SdGraphPartImp   *mPartImp;
    SdSymImpPinTable  mPins;        //Pin information table
    SdParamTable      mParam;       //Parameters
  public:
    SdGraphSymImp();
    SdGraphSymImp(SdPItemSymbol *comp, SdPItemSymbol *sym, SdPItemPart *part , SdPoint pos, SdPropSymImp *prp);



    //Information
    //Return current plate of section
    SdPItemPlate *currentPlate() const;
    QString       getRenumSect( SdPoint &dest, int &sheetNumber );
    //Get wire name pin with pinIndex connected to
    QString       pinWireName( int pinIndex ) const;
    //Return if pin with pinIndex connected to any wire or not
    bool          isPinConnected( int pinIndex ) const;
    //Param full list
    SdParamTable& getParamTable() { return mParam; }
    //Get param
    QVariant      getParam( const QString key ) { return mParam.value( key ); }
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
    void          netWirePlace( SdPoint a, SdPoint b, const QString name, SdUndo *undo );
    //Notification about wire segment deletion
    void          netWireDelete( SdPoint a, SdPoint b, const QString name, SdUndo *undo ); //Извещение об удалении сегмента
    //Accumulate segments connected to component
    void          accumLinked( SdPoint a, SdPoint b, SdSelector *sel );

    //Service
    //Pin connection-disconnection by index only for symbol implement
    void          symPinConnectionSet( int pinIndex, const QString wireName, bool com );
    //Move section to plate
    void          moveToPlate( SdPItemPlate *plate, SdUndo *undo );
    //Link with partImp
    void          setLinkSection(int section, SdGraphPartImp *partImp );
    //Unconnect pin in point
    void          unconnectPinInPoint(SdPoint p , SdUndo *undo);
    //Unlink symbol from part
    void          unLinkPartImp( SdUndo *undo );
    //Link auto partImp. partImp and section are selected automatic
    void          linkAutoPart( SdUndo *undo );
    //Replace part
    void          replacePart( SdPItemPart *part, SdUndo *undo );
    //TODO Accum auto net
    //void          autoNet( DNetListTable &table );         //Накопить цепи в текстовый список цепей
    //Replace symbol
    void          replace( SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *symbol, SdUndo *undo );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_SYM_IMP ); }
    virtual quint64 getClass() const override { return dctSymImp; }
    virtual void    attach(SdUndo *undo) override;
    virtual void    detach(SdUndo *undo) override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    moveComplete( SdPoint grid, SdUndo *undo ) override;
    virtual void    prepareMove( SdUndo *undo ) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    setProp(SdProp &prop) override;
    virtual void    getProp(SdProp &prop) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual bool    getInfo(SdPoint p, QString &info, bool extInfo) override;
    virtual bool    snapPoint(SdSnapInfo *snap) override;


  private:
    SdPItemSheet *getSheet() const;
    void          updatePinsPositions();
    //Unconnect all pins from wires
    void          ucomAllPins( SdUndo *undo );
    //Create new pins
    void          createPins( SdUndo *undo );
    //Link auto partImp in given plate. partImp and section are selected automatic
    void          linkAutoPartInPlate( SdPItemPlate *plate, SdUndo *undo );
    //Pin connection-disconnection by index for symbol and part implements
    void          pinConnectionSet( int pinIndex, const QString netName, bool com, SdUndo *undo );

  };

#endif // SDGRAPHSYMIMP_H
