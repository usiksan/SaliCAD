/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Class present schematic net wire part: wire segment or wire name
*/
#ifndef SDGRAPHWIRE_H
#define SDGRAPHWIRE_H

#include "SdGraph.h"
#include "SdPItemSheet.h"


class SdGraphNet : public SdGraph
  {
  protected:
    QString mNetName;
  public:
    SdGraphNet();
    SdGraphNet( const QString netName );

    SdPItemSheet *getSheet() const { return dynamic_cast<SdPItemSheet*>(getParent()); }

    QString       getNetName() const { return mNetName; }

    virtual void  setNetName( const QString netName, SdUndo *undo );

    // SdObject interface
  public:
    virtual void cloneFrom(const SdObject *src) override;
    virtual void writeObject(QJsonObject &obj) const override;
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void setProp(SdPropSelected &prop) override;
    virtual void getProp(SdPropSelected &prop) override;
    virtual void saveState(SdUndo *undo) override;
  };


#endif // SDGRAPHWIRE_H
