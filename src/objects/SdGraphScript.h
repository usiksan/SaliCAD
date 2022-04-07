#ifndef SDGRAPHSCRIPT_H
#define SDGRAPHSCRIPT_H

#include "SdGraphParam.h"
#include "SdGraphScriptRefMap.h"
#include "script/SdScriptProgramm.h"

#define SD_TYPE_SCRIPT "Script"

class SdGraphScript : public SdGraphParam
  {
    SdGraphScriptRefMap mRefMap;
    QString             mScript;
    SdPoint             mOrigin;
    SdPropText          mProp;
    SdRect              mOverRect;

    QString             mError;
    SdScriptProgrammPtr mProgramm;
  public:
    SdGraphScript();
    SdGraphScript( const QString &script, SdPoint org, const SdPropText &prp );

    QString scriptGet() const { return mScript; }

    void    scriptSet( const QString &scr, SdUndo *undo );

    void    parse();

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual SdClass getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    move(SdPoint offset) override;
    virtual void    setProp(SdPropSelected &prop) override;
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void    setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void setLayerUsage() override;
    virtual bool isVisible() override;
    virtual SdRect getOverRect() const override;
    virtual void draw(SdContext *dc) override;
    virtual int behindCursor(SdPoint p) override;
    virtual int behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop) override;
  };

#endif // SDGRAPHSCRIPT_H
