/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Pin-to-pad association table map as object. It allow to save it in library
  and retrive from external library for share using.
*/
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
    SdPadMap mMap;  //Pin-to-pad association table map
    QString  mName; //Name of association
    QString  mId;   //Unical object id
  public:
    SdPadAssociation();
    SdPadAssociation(const SdPadMap map, const QString name , const QString author);

    SdPadMap     getMap() const { return mMap; }

    QString      getName() const { return mName; }

    void         setMap( const QString name, const SdPadMap &src, SdUndo *undo );

    bool         contains( const QString pinType ) const { return mMap.contains(pinType); }

    const SdPad  pin( const QString pinType ) const { return mMap[pinType]; }

    // SdObject interface
  public:
    virtual QString getId() const override { return mId; }
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDPADASSOCIATION_H
