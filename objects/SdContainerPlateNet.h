/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDCONTAINERPLATENET_H
#define SDCONTAINERPLATENET_H

#include "SdContainer.h"

class SdSelector;
class SdPItemPlate;
class SdGraphRoad;

#define SD_TYPE_PLATE_NET "PlateNet"

class SdContainerPlateNet : public SdContainer
  {
    QString mNetName; //Name of net
  public:
    SdContainerPlateNet();
    SdContainerPlateNet( const QString netName );

    //Information
    //Return net name
    QString         getNetName() const { return mNetName; }
    //Return plate where in net recides
    SdPItemPlate   *getPlate() const;

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_PLATE_NET); }
    virtual quint64 getClass() const override { return dctPlateNet; }
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDCONTAINERPLATENET_H
