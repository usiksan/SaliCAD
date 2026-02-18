/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Text graph object
*/
#ifndef SDGRAPHTEXT_H
#define SDGRAPHTEXT_H

#include "SdGraph.h"
#include "SdPropText.h"
#include "SdRect.h"
#include <QString>

#define SD_TYPE_TEXT "Text"

class SdGraphText : public SdGraph
  {
  protected:
    SdPoint    mOrigin;
    SdPropText mProp;
    QString    mString;
    SdRect     mOverRect;
  public:
    SdGraphText();
    SdGraphText( SdPoint org, const QString &str, SdRect r, const SdPropText &p );

    //Information
    QString           getText() const { return mString; }
    SdPoint           getOrigin() const { return mOrigin; }
    const SdPropText& getPropText() const { return mProp; }

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void    cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void    json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void    json( const SdJsonReader &js ) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    transform(const QTransform &map, SdPvAngle angle) override;
    virtual void    setProp(SdPropSelected &prop) override;
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void    setLayerUsage() override;
    virtual void    setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual int     behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHTEXT_H
