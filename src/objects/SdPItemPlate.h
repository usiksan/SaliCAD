/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical PCB layout with component placement
*/
#ifndef SDPITEMPLATE_H
#define SDPITEMPLATE_H

#include "SdProjectItem.h"
#include "SdRect.h"
#include "SdStratum.h"
#include "SdGraphTraced.h"
#include "SdPlateNet.h"
#include "SdPadAssociation.h"
#include "SdRuleBlock.h"
#include <QMap>
#include <QVector>

class SdGraphPartImp;
class SdGraphSymImp;
class SdPItemPart;
class SdPItemSymbol;
class SdPItemComponent;

#define SD_TYPE_PLATE "Plate"



class SdPItemPlate : public SdProjectItem
  {
    SdRect            mPartRow;           //Row for allocation autoinserted parts
    SdPadAssociation  mPadAssociation;    //Pad to pin association table
    SdRuleBlock       mRulesPcb;          //PCB rules
    SdRuleBlockMap    mRulesMap;          //Net rule map
    qint32            mStratumCount;      //Tracing stratum (layers) count

    //Not saved
    SdRatNet          mRatNet;            //Rat net is unconnected pairs
    bool              mRatNetDirty;

    SdRectList        mRuleErrors;        //Indicator rule errors

    Sd3drModel         m3dModel;
  public:
    //Saved properties for modes
    SdPoint           mTraceGrid;         //Current trace grid
    bool              mTraceCursorGrid;   //Enable cursor grid align when trace
    SdPoint           mPlaceGrid;         //Current place grid
    bool              mPlaceCursorGrid;   //Enable cursor grid align when place



    //Constructor. Build empty plate ready for loading or inserting child
    SdPItemPlate();

    //Stratum count
    int                    stratumCount() const { return mStratumCount; }
    void                   setStratumCount( int sc );
    int                    stratumMask() const;

    SdGraphPartImp        *allocPartImp(int *section, SdPItemPart *part, SdPItemComponent *comp, SdPItemSymbol *sym, const SdStringMap &param, SdUndo *undo);

    //Pin-to-pad association
    //Full association table
    SdPadMap               getPadMap() const { return mPadAssociation.getMap(); }
    //Pad association table name
    QString                getPadAssociationName() const { return mPadAssociation.getName(); }
    //Return pad
    SdPad                  getPad( const QString pinType ) const;
    //Draw pad
    void                   drawPad( SdContext *dc, SdPoint p, const QString pinType, int stratum ) const;
    //Setup new map and name
    void                   setPadAssociation(const QString nm, const SdPadMap &map, SdUndo *undo );
    //Return over pad polygon
    QPolygonF              getPadPolygon(SdPoint p, const QString pinType, int addon) const;
    //Return over pad circle radius
    int                    getPadOverRadius( const QString pinType ) const;
    //Append window for pin pad
    void                   appendPadWindow( SdPolyWindowList &dest, SdPoint p, const QString pinType, int gap, const QTransform &t ) const;

    //!
    //! \brief appendPadHoles Accum holes description into faceList
    //! \param p              Position of pad
    //! \param pinType        Pad description
    //! \param model          Model which accumulate coord vertexes
    //! \param faceList       Face list for holding holes (single face for single hole)
    //! \param stratum        Stratum for layers
    //! \param map            Map for holes conversion
    //!
    void                   appendPadHoles( SdPoint p, const QString pinType, Sd3drModel &model, Sd3drFaceList &faceList, SdStratum stratum, const QMatrix4x4 &map ) const;



    //Accumulate net segments to given net container
    void                   accumNetSegments( SdPlateNetContainer *container );



    //Set flag to update rat net
    void                   setDirtyRatNet();

    //Draw rat net
    void                   drawRatNet( SdContext *dc );

    //Build rat net
    void                   buildRatNet();




    //Draw pcb for trace
    void                   drawTrace( SdContext *ctx, SdStratum curStratum, QString currentNetName );

    //Unconnected nets count
    int                    unconnectedNetCount() const { return mRatNet.count(); }


    //Tracing rules
    //Pcb rules
    const SdRuleBlock     &rulePcbGet() const { return mRulesPcb; }

    //Net rules map
    const SdRuleBlockMap  &ruleMapGet() const { return mRulesMap; }

    //Set new rules
    void                   ruleSet(const SdRuleBlock &pcb, const SdRuleBlockMap &map , SdUndo *undo);

    //Get one rule for given net
    int                    ruleForNet(const QString netName, SdRuleId ruleId) const;

    //Build rules block for given net
    void                   ruleBlockForNet(const QString netName, SdRuleBlock &blockDest );




    //Accum bariers
    void                   accumBarriers( quint64 mask, SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &rule );

    //Check rules
    void                   checkRules(std::function<bool()> fun1);

    //Rule errors count
    int                    ruleErrorsCount() const { return mRuleErrors.count(); }

    //Rule error area
    SdRect                 ruleError( int i ) const { if( i < 0 || i >= mRuleErrors.count() ) return SdRect(); return mRuleErrors.at(i); }

    //Draw rule error indicators
    void                   drawRuleErrors( SdContext *dc );

    //Renumeration implements
    void                   renumeration();


    void                   rebuild3dModel();

    // SdObject interface
  public:
    virtual QString        getType() const override;
    virtual SdClass        getClass() const override;
    virtual void           detach(SdUndo *undo) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void           json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void           json( const SdJsonReader &js ) override;

    // SdProjectItem interface
  public:
    virtual QString        getIconName() const override;
    virtual bool           is3dAllowed() const override { return true; }
    virtual void           draw3d( QOpenGLFunctions_2_0 *f ) override;
    virtual quint64        getAcceptedObjectsMask() const override;

    //Set layers usage
    virtual void           setLayerUsage() override;

  private:
    //Check intersection on barriers and form errorList
    void checkIntersection(const SdBarrierList &src, const SdBarrierList &dst, bool excludeSameIndex );
  };

#endif // SDPITEMPLATE_H
