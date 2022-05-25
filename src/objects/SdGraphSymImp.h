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
#include "SdGraphParam.h"
#include "SdPoint.h"
#include "SdPropText.h"
#include "SdPropSymImp.h"
#include "SdSymImpPin.h"
#include "library/SdStringMap.h"
#include "SdPItemComponent.h"
#include "SdTextImplement.h"


#define SD_TYPE_SYM_IMP "SymImp"

class SdGraphArea;
class SdPItemSymbol;
class SdPItemSheet;
class SdPItemPart;
class SdGraphPartImp;
class SdPItemPlate;
class SdGraphNetWire;


class SdGraphSymImp : public SdGraphParam
  {
    SdGraphArea      *mArea;         //!< PCB where this symbol implement contains in
    int               mSectionIndex; //!< Section index (from 0)
    int               mLogSection;   //!< Logical symbol section number (from 1, 0 - not assigned)
    int               mLogNumber;    //!< Logical part number (from 1, 0 - not assigned)
    SdPoint           mOrigin;       //!< Position of Implement
    SdPropSymImp      mProp;         //!< Implement properties
    SdRect            mOverRect;     //!< Over rect

    //Symbol identification
    SdTextImplement   mIdent;        //!< Symbol identificator text properties and position

    //Symbol value
    SdTextImplement   mValue;        //!< Symbol value text properties and position

    SdPItemComponent *mComponent;    //!< Object contains section information, pin assotiation info. May be same as mSymbol.
    SdPItemSymbol    *mSymbol;       //!< Symbol contains graph information
    SdPItemPart      *mPart;         //!< Part for partImp construction
    SdGraphPartImp   *mPartImp;      //!< Part implement in desired plate
                                     //!  If mPartImp == nullptr then symbol not linked
    SdSymImpPinTable  mPins;         //!< Pin information table
    QString           mLinkError;    //!< Error string, which displays when no link
  public:
    SdGraphSymImp();

    //comp - contains pack info
    //sym  - contains graphics
    //part - part type
    SdGraphSymImp(SdPItemComponent *comp, SdPItemSymbol *sym, SdPItemPart *part, const SdStringMap &param, SdPoint pos, SdPropSymImp *prp);


    //Information
    //Get implement transform matrix
    QTransform        matrix() const;
    //Return current plate of section
    SdPItemPlate     *currentPlate() const;
    //Retrive origin point and sheet number
    QString           getRenumSect( SdPoint &dest, int &sheetNumber ) const;
    //Return part implement
    SdGraphPartImp   *getPartImp() const { return mPartImp; }

    //Pin information
    //Pin presention
    bool              isPinPresent( const QString pinName ) const { return mPins.contains(pinName); }
    //Pin connection status
    bool              isPinConnected( const QString pinName ) const;
    //Pin net name
    QString           pinNetName( const QString pinName ) const;

    //Params with local param table
    //Setup full param table
    void              paramTableSet( const SdStringMap map, SdUndo *undo, SdGraphPartImp *partImp );

    //Get BOM item line
    QString           getBomItemLine() const;


    //Ident edit
    //Get ident prefix
    QString           identPrefix() const;
    //Get full visual ident of part aka D4 or R45
    QString           ident() const;
    //Get ident text properties
    const SdPropText& identProp() const { return mIdent.mProp; }
    //Get idnet text position
    SdPoint           identPosition() const { return mIdent.mOrigin; }
    //Set ident text properties and position
    void              identSet( const SdPropText &prp, SdPoint pos, SdUndo *undo );
    //Get separated ident information
    QString           identInfoGet( int &logNumber, int &logSection );
    //Set ident information
    void              identInfoSet(int logNumber, int logSection );


    //Value
    //Get full visual value of part aka smt32f417vgt
    QString           value() const;
    //Get value text properties
    const SdPropText& valueProp() const { return mValue.mProp; }
    //Get value text position
    SdPoint           valuePosition() const { return mValue.mOrigin; }
    //Set value text properties and position
    void              valueSet( const SdPropText &prp, SdPoint pos, SdUndo *undo );



    //Notifications about segment operation
    //Notification about wire segment position changed
    void              netWirePlace( SdGraphNetWire *wire, SdUndo *undo );
    //Notification about wire segment deletion [Извещение об удалении сегмента]
    void              netWireDelete(SdGraphNetWire *wire, SdUndo *undo );
    //Accumulate segments connected to component
    void              accumLinked(SdPoint a, SdPoint b, const QString netName, SdSelector *sel );

    //Service
    //Pin status get
    void              pinStatusGet( const QString pinName, SdSymImpPin &pin ) const;
    //Pin status set
    void              pinStatusSet( const QString pinName, const SdSymImpPin &pin );
    //Move section to plate
    void              moveToPlate( SdPItemPlate *plate, SdUndo *undo );
    //Link with partImp
    void              setLinkSection(int section, SdGraphPartImp *partImp );
    //Unconnect pin over rect
    void              unconnectPinOverRect(SdRect over , SdUndo *undo, const QString undoTitle);
    //Unlink symbol from part
    void              unLinkPart( SdUndo *undo );
    //Link auto partImp. partImp and section are selected automatic
    void              linkAutoPart( SdUndo *undo );
    //TODO D017 Accum auto net
    //void          autoNet( DNetListTable &table );         //Накопить цепи в текстовый список цепей

    // SdObject interface
  public:
    virtual QString   getType() const override { return QStringLiteral( SD_TYPE_SYM_IMP ); }
    virtual quint64   getClass() const override { return dctSymImp; }
    virtual void      attach(SdUndo *undo) override;
    virtual void      detach(SdUndo *undo) override;
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void      cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void      json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void      json( const SdJsonReader &js ) override;

    virtual bool      isUsed(SdObject *obj) const override;
    //Upgrade project item on new one
    virtual bool      upgradeProjectItem( SdUndo *undo, QWidget *parent ) override;

    // SdGraph interface
  public:
    virtual void      saveState(SdUndo *undo) override;
    virtual void      moveComplete( SdPoint grid, SdUndo *undo ) override;
    virtual void      prepareMove( SdUndo *undo ) override;
    virtual void      move(SdPoint offset) override;
    virtual void      rotate(SdPoint center, SdPropAngle angle) override;
    virtual void      mirror(SdPoint a, SdPoint b) override;
    virtual void      setProp(SdPropSelected &prop) override;
    virtual void      getProp(SdPropSelected &prop) override;
    virtual void      selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void      selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual bool      isVisible() override;
    virtual SdRect    getOverRect() const override;
    virtual void      draw(SdContext *dc) override;
    virtual int       behindCursor(SdPoint p) override;
    virtual bool      getInfo(SdPoint p, QString &info, bool extInfo) override;
    //Find snap point on object
    virtual void      snapPoint(SdSnapInfo *snap) override;


  private:
    SdPItemSheet     *getSheet() const;
    //Update pins positions upon position and props of symbol implement
    void              updatePinsPositions();
    //Link auto partImp in given plate. partImp and section are selected automatic
    void              linkAutoPartInPlate( SdPItemPlate *plate, SdUndo *undo );
    //Pin connection-disconnection by name for symbol and part implements
    void              pinConnectionSet(const QString pinName, const QString netName, SdUndo *undo );
    //Update connection status for all pins
    void              updatePinsConnectionStatus( SdUndo *undo );

  };

#endif // SDGRAPHSYMIMP_H
