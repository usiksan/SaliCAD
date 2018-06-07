#ifndef SDPADASSOCIATION_H
#define SDPADASSOCIATION_H

#include "SdObject.h"
#include "SdPad.h"

#include <QMap>

class SdContext;

typedef QMap<QString,SdPad> SdPadMap;

#define SD_TYPE_PAD_ASSOCIATION "PadAssociation"

class SdPadAssociation : public SdObject
  {
    SdPadMap mMap;
  public:
    SdPadAssociation();

    SdPadMap     getMap() const { return mMap; }

    void         setMap( const SdPadMap &src, SdUndo *undo );

//    void         drawPad( );
//    const SdPad *pin( const QString pinType ) const;

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void cloneFrom(const SdObject *src) override;
    virtual void writeObject(QJsonObject &obj) const override;
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDPADASSOCIATION_H
