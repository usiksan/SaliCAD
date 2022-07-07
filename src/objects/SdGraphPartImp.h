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
#include "SdTextImplement.h"
#include "SdPlateNetList.h"

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
  void        accumWindows(SdPItemPlate *plate, SdPolyWindowList &dest, int stratum, int gap, const QString netName, const QTransform &t ) const;

  //!
  //! \brief accumHoles Accum holes description into faceList
  //! \param plate      Plate throught which pads supported
  //! \param model      Model which accumulate coord vertexes
  //! \param faceList   Face list for holding holes (single face for single hole)
  //! \param stratum    Stratum for layers
  //! \param map        Map for holes conversion
  //!
  void        accumHoles( SdPItemPlate *plate, Sd3dModel &model, Sd3drFaceList &faceList, SdStratum stratum, const QMatrix4x4 &map ) const;

  //!
  //! \brief json Overloaded function to write object content into json writer
  //! \param js   Json writer
  //!
  void        json( SdJsonWriter &js ) const;

  //!
  //! \brief json Overloaded function to read object content from json reader
  //! \param js   Json reader
  //!
  void        json( const SdJsonReader &js );

  QString     fromJson( SdObjectMap *map, const QJsonObject obj );
  };

typedef QMap<QString,SdPartImpPin> SdPartImpPinTable;





//Information about section in part implementation
struct SdPartImpSection {
  SdPItemSymbol *mSymbol; //Symbol of this section
  SdGraphSymImp *mSymImp; //Symbol implementation of this section

  SdPartImpSection() : mSymbol(nullptr), mSymImp(nullptr) {}


  //!
  //! \brief json Overloaded function to write object content into json writer
  //! \param js   Json writer
  //!
  void json( SdJsonWriter &js ) const;

  //!
  //! \brief json Overloaded function to read object content from json reader
  //! \param js   Json reader
  //!
  void json( const SdJsonReader &js );

  bool isFree( SdPItemSymbol *symbol ) { return mSymbol == symbol && mSymImp == nullptr; }
  };

typedef QList<SdPartImpSection> SdPartImpSectionTable;





//Part implementation
class SdGraphPartImp : public SdGraphTraced
  {
    int                    mLogNumber;   //Logical part number (from 1)
    SdPoint                mOrigin;      //Position of Implement
    SdPropPartImp          mProp;        //Implement properties
    SdRect                 mOverRect;    //Over rect

    //Part identification
    SdTextImplement        mIdent;       //Part identificator text properties and position

    //Part value
    SdTextImplement        mValue;       //Part value text properties and position

    SdPartImpPinTable      mPins;
    SdPartImpSectionTable  mSections;
    SdPItemPart           *mPart;        //Part for this implementation
    SdPItemComponent      *mComponent;   //Component with pack info
    SdStringMap            mParamTable;  //Parameters
  public:
    SdGraphPartImp();
    SdGraphPartImp(SdPoint org, SdPropPartImp *prp, SdPItemPart *part, SdPItemComponent *comp, const SdStringMap &param );

    //Information
    //Get implement transform matrix
    QTransform        matrix() const;

    //Angle of component rotation
    SdPropAngle       angle() const { return mProp.mAngle.getValue(); }


    //Identificator
    //Get full visual ident of part aka D4 or R45
    QString           ident() const;
    //Get ident text properties
    const SdPropText& identProp() const { return mIdent.mProp; }
    //Get idnet text position
    SdPoint           identPosition() const { return mIdent.mOrigin; }
    //Set ident text properties and position
    void              identSet( const SdPropText &prp, SdPoint pos, SdUndo *undo );

    //Value
    //Get full visual value of part aka smt32f417vgt
    QString           value() const;
    //Get value text properties
    const SdPropText& valueProp() const { return mValue.mProp; }
    //Get value text position
    SdPoint           valuePosition() const { return mValue.mOrigin; }
    //Set value text properties and position
    void              valueSet( const SdPropText &prp, SdPoint pos, SdUndo *undo );

    //Check if there free section slot. If there - setup section and return true
    bool              isSectionFree(int *section, SdPItemPart *part, SdPItemComponent *comp, const SdStringMap &param, SdPItemSymbol *sym );
    //Get origin of component
    SdPoint           getOrigin() const { return mOrigin; }



    //Service
    //Pin link-unlink
    bool              partPinLink(const QString pinNumber, SdGraphSymImp *imp, const QString pinName, bool link );
    //link-unlink section
    void              setLinkSection( int section, SdGraphSymImp *symImp );
    //Check if all section removed, then autodeleted
    void              autoDelete( SdUndo *undo );
    //Save to undo state of all pins
    void              savePins( SdUndo *undo );
    //Accum used pins
    void              accumUsedPins(SdPadMap &map ) const;
    //Pin iterator
    void              forEachPin( std::function<void(const SdPartImpPin &pin)> fun1 );


    //Renumeration
    QString           getIdentPrefix() const;
    //Compare partImp's
    bool              compareRenumeration( const SdGraphPartImp *imp ) const;
    //Lower sheet
    bool              getLowerPosAndSheet( SdPoint &dest, int &sheet ) const;
    //Set index
    void              setIdentIndex( int index );

    //Parameters
    //Params with local param table
    //Test if param present in local table
    bool              paramContains( const QString key ) const { return mParamTable.contains(key); }

    //Get param value from local table
    QString           paramGet( const QString key ) const { return mParamTable.value(key); }

    //Full local param table
    SdStringMap       paramTable() const { return mParamTable; }

    //Setup full param table
    void              paramTableSet( const SdStringMap map, SdUndo *undo, SdGraphSymImp *symImp );

    //Get BOM item line
    QString           getBomItemLine() const;

    //Get param table as object
    QJsonObject       paramTableObject() const;

    //Special drawing
    //Draw part without pads
    void              drawWithoutPads( SdContext *cdx );
    //Draw pads only
    void              drawPads( SdContext *cdx, SdStratum stratum, const QString highlightNetName );
    //Draw rat net
    void              drawRatNet( SdContext *cdx, SdPlateNetList &netList );

    // SdObject interface
  public:
    virtual QString      getType() const override { return QStringLiteral(SD_TYPE_PART_IMP); }
    virtual quint64      getClass() const override { return dctPartImp; }
    virtual void         attach(SdUndo *undo) override;
    virtual void         detach(SdUndo *undo) override;
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void         cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void         json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void         json( const SdJsonReader &js ) override;

    virtual bool         isUsed(SdObject *obj) const override;
    virtual bool         upgradeProjectItem(SdUndo *undo, QWidget *parent) override;

    // SdGraph interface
  public:
    virtual void         saveState(SdUndo *undo) override;
    virtual void         move(SdPoint offset) override;
    virtual void         rotate(SdPoint center, SdPropAngle angle) override;
    virtual void         mirror(SdPoint a, SdPoint b) override;
    virtual void         setProp(SdPropSelected &prop) override;
    virtual void         getProp(SdPropSelected &prop) override;
    virtual void         selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void         selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void         setLayerUsage() override;
    virtual bool         isVisible() override;
    virtual SdRect       getOverRect() const override;
    virtual int          behindCursor(SdPoint p) override;
    virtual bool         getInfo(SdPoint p, QString &info, bool extInfo) override;
    //Find snap point on object
    virtual void         snapPoint(SdSnapInfo *snap) override;

    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //!
    virtual void         draw3d( QOpenGLFunctions_2_0 *f ) const override;

    //!
    //! \brief accumHoles Accum holes description into faceList
    //! \param model      Model which accumulate coord vertexes
    //! \param faceList   Face list for holding holes (single face for single hole)
    //! \param stratum    Stratum for layers
    //! \param map        Map for holes conversion
    //!
    virtual void         accumHoles( Sd3dModel &model, Sd3drFaceList &faceList, SdStratum stratum, const QMatrix4x4 &map ) const override;

  private:
    void                 updatePinsPositions();
    //Pin connection-disconnection by index only for part (internal and undo)
    void                 partPinConnectionSet( int pinIndex, SdGraphRoadPin *roadPin );

    friend class SdUndoRecordImpPin;

    // SdGraphTraced interface
  public:
    virtual bool         isPointOnNet(SdPoint p, SdStratum stratum, QString *netName, int *destStratum) override;
    virtual void         accumNetSegments( SdPlateNetContainer *netContainer ) override;
    virtual void         drawStratum(SdContext *dc, int stratum ) override;
    virtual void         accumBarriers( SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk ) const override;
    virtual void         accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName ) const override;
    //Check if any pin of part linked to point
    virtual bool         isLinked( SdPoint a, SdStratum stratum, QString netName ) const override;
    //Stratum of object
    virtual SdStratum    stratum() const override { return mProp.mSide; }
  };

#endif // SDGRAPHPARTIMP_H
