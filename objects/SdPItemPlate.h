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
#include <QMap>
#include <QVector>

class SdGraphPartImp;
class SdGraphSymImp;
class SdPItemPart;
class SdPItemSymbol;

#define SD_TYPE_PLATE "Plate"


struct SdRatNetPair {
    SdPoint a,b;
  };

typedef QVector<SdRatNetPair> SdRatNet;

typedef QMap<QString,SdPItemPart*> SdPadAssotiation;

class SdPItemPlate : public SdProjectItem
  {
    SdRect           mPartRow;           //Row for allocation autoinserted parts
    SdPadAssotiation mPadAssotiation;    //Pad to pin assotiation table

    //Not saved
    SdRatNet         mRatNet;            //Rat net is unconnected pairs
    bool             mRatNetDirty;
    QVector<int>     mSubNet;            //Contains indexes of subnet
  public:
    SdPItemPlate();

//    void                  netForEach( quint64 classMask, const QString  std::function<bool(SdObject*)> fun1 );


    SdGraphPartImp       *allocPartImp(int *section, SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *sym , SdUndo *undo);

    //Get pad
    SdPItemPart          *getPad( const QString pinType ) const { return mPadAssotiation.value( pinType ); }

    //Set flag to update rat net
    void                  setDirtyRatNet();

    //Draw rat net
    void                  drawRatNet( SdContext *dc );

    //Get subnet position index (cell number in mSubNet witch contains subNet index)
    int                   getSubNetRef(SdObject *last, const QString netName, SdPoint p, SdStratum s );


//    DPrtImpPic*  FindCompPart( DPrtPic *part, CPChar name, int numSection );
//    DPrtImpPic*  FindCompById( CPChar ident );

    // SdObject interface
  public:
    virtual QString       getType() const override;
    virtual quint64       getClass() const override;
    virtual void          cloneFrom(const SdObject *src) override;

    // SdProjectItem interface
  public:
    virtual QString       getIconName() const override;
    virtual SdGraphIdent *createIdent() override;
  };

#endif // SDPITEMPLATE_H
