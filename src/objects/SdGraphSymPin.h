/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Symbol pin.
*/
#ifndef SDGRAPHSYMPIN_H
#define SDGRAPHSYMPIN_H

#include "SdGraph.h"
#include "SdPoint.h"
#include "SdPropText.h"
#include "SdPropSymPin.h"

#define SD_TYPE_SYM_PIN "SymPin"

class SdGraphSymPin : public SdGraph
  {
    SdPoint      mOrigin;     //!< Pin origin
    SdPropSymPin mPinProp;    //!< Pin properties
    SdPoint      mNumberPos;  //!< Pin number position
    SdRect       mNumberRect; //!< Pin number over rect
    SdPropText   mNumberProp; //!< Pin number properties
    QString      mNumber;     //!< Default pin number
    SdPoint      mNamePos;    //!< Pin name position
    SdRect       mNameRect;   //!< Pin name over rect
    SdPropText   mNameProp;   //!< Pin name properties
    QString      mName;       //!< Pin name

    //Different pin part selection
    bool         mPinSelect;  //Pin selected
    bool         mNumSelect;  //Pin number selected
    bool         mNamSelect;  //Pin name selected
  public:
    SdGraphSymPin();
    SdGraphSymPin( SdPoint org, const SdPropSymPin &pinProp, SdPoint numberPos, const SdPropText &numberProp, SdPoint namePos, const SdPropText &nameProp, const QString name, const QString number = QString{} );

    QString  getPinName() const { return mName; }
    SdPoint  getPinOrigin() const { return mOrigin; }
    //Draw pin in symbol implementation
    void     drawImp( SdContext *dc, const QString pinNumber, bool com );

    // SdObject interface
  public:
    virtual QString   getType() const override { return QStringLiteral( SD_TYPE_SYM_PIN ); }
    virtual quint64   getClass() const override { return dctSymPin; }
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

    // SdGraph interface
  public:
    virtual void      saveState(SdUndo *undo) override;
    virtual void      move(SdPoint offset) override;
    virtual void      rotate(SdPoint center, SdPropAngle angle) override;
    virtual void      mirror(SdPoint a, SdPoint b) override;
    virtual void      setProp(SdPropSelected &prop) override;
    virtual void      getProp(SdPropSelected &prop) override;
    virtual void      setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void      selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void      selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void      select(SdSelector *selector) override;
    virtual bool      isVisible() override;
    virtual SdRect    getOverRect() const override;
    virtual void      draw(SdContext *dc) override;
    virtual int       behindCursor(SdPoint p) override;
    virtual int       behindText( SdPoint p, SdPoint &org, QString &dest, SdPropText &prop ) override;
    //Find snap point on object
    virtual void      snapPoint(SdSnapInfo *snap) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHSYMPIN_H
