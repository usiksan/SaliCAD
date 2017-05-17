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
#include <QList>

#define SD_TYPE_SYM_IMP "SymImp"

class SdGraphArea;
class SdGraphSymPin;
class SdPItemSymbol;
class SdPItemSheet;
class SdPItemPart;
class SdGraphPartImp;

//Pin for symbol implementation
struct SdSymImpPin {
  SdGraphSymPin  *mPin;       //Pin
  QString         mPinName;   //Pin name in symbol
  QString         mPinNumber; //Pin number in part
  SdPoint         mPosition;  //Pin position in sheet context
  QString         mWireName;  //Net, which pin connected to
  bool            mCom;       //State of pin to net connectivity

  SdSymImpPin();

  void operator = ( SdSymImpPin &pin );
  void draw( SdContext *dc );
  void connect( SdPoint a, SdPoint b, const QString &name, SdGraphPartImp *prt );
  void ifConnect( SdGraphPartImp *prt );
  void disconnect( SdPoint a, SdPoint b, const QString &name, SdGraphPartImp *prt );
  void ifDisconnect( SdGraphPartImp *prt );
  void ucom( SdGraphPartImp *prt );
  void prepareMove( SdPItemSheet *sheet, SdSelector *selector );
  void connectTest( SdPItemSheet *sheet, SdGraphPartImp *prt );
  };


typedef QList<SdSymImpPin> SdSymImpPinTable;


class SdGraphSymImp : public SdGraph
  {
    SdGraphArea      *mArea;        //PCB where this symbol implement contains in
    int               mSectionIndex;//Section index (from 0)
    int               mLogSection;  //Logical symbol section number (from 1)
    int               mLogNumber;   //Logical part number (from 1)
    QString           mName;        //Name of component
    SdPoint           mOrigin;      //Position of Implement
    SdPropSymImp      mProp;        //Implement properties
    SdRect            mOverRect;    //Over rect
    QString           mPrefix;      //Part identificator prefix
    SdPropText        mIdentProp;   //Part identificator text properties
    QString           mIdent;       //Full implement identificator contains prefix, logNumber and logSection
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
    SdGraphSymImp( SdPItemSymbol *comp, SdPItemSymbol *sym, SdPItemPart *part );



    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_SYM_IMP ); }
    virtual quint64 getClass() const override { return dctSymImp; }

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    moveComplete(SdUndo *undo) override;
    virtual void    prepareMove() override;
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
    virtual int     behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop) override;
    virtual bool    getInfo(SdPoint p, QString &info, bool extInfo) override;
    virtual bool    snapPoint(SdSnapInfo *snap) override;

  private:
    SdPItemSheet *getSheet() const;
    void          updatePinsPositions();
    //Unconnect all pins from wires
    void          ucomAllPins();
    //Create new pins
    void          createPins();
    //Ucom part
    void          unLinkFromPart();
  };

#endif // SDGRAPHSYMIMP_H
