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
    SdRect           mPartRow;           //Row for allocation autoinserted parts
    SdPadAssociation mPadAssociation;    //Pad to pin assotiation table

    //Not saved
    SdRatNet         mRatNet;            //Rat net is unconnected pairs
    bool             mRatNetDirty;
  public:
    SdPItemPlate();

//    void                  netForEach( quint64 classMask, const QString  std::function<bool(SdObject*)> fun1 );


    SdGraphPartImp        *allocPartImp(int *section, SdPItemPart *part, SdPItemComponent *comp, SdPItemSymbol *sym , SdUndo *undo);

    //Draw pad
    void                   drawPad( SdContext *dc, SdPoint p, const QString pinType ) const;

    //Set flag to update rat net
    void                   setDirtyRatNet();

    //Draw rat net
    void                   drawRatNet( SdContext *dc );

    //Build rat net
    void                   buildRatNet();


//    DPrtImpPic*  FindCompPart( DPrtPic *part, CPChar name, int numSection );
//    DPrtImpPic*  FindCompById( CPChar ident );

    // SdObject interface
  public:
    virtual QString        getType() const override;
    virtual quint64        getClass() const override;
    virtual void           cloneFrom(const SdObject *src) override;
    virtual void           writeObject(QJsonObject &obj) const override;
    virtual void           readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdProjectItem interface
  public:
    virtual QString        getIconName() const override;
    virtual quint64        getAcceptedObjectsMask() const override;
    virtual void           insertObjects( SdPoint offset, SdSelector *sel, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next ) override;

  };

#endif // SDPITEMPLATE_H
