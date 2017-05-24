/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDCONTAINERSHEETNET_H
#define SDCONTAINERSHEETNET_H

#include "SdContainer.h"

class SdSelector;

#define SD_TYPE_SHEET_NET "SheetNet"


class SdContainerSheetNet : public SdContainer
  {
    QString mNetName; //Name of net
  public:
    SdContainerSheetNet();
    SdContainerSheetNet( const QString netName );

    QString getNetName() const { return mNetName; }
    void    accumLinked( SdPoint a, SdPoint b, SdSelector *sel );

    // SdObject interface
  public:
    virtual QString getType() const override { return SD_TYPE_SHEET_NET; }
    virtual quint64 getClass() const override { return dctSheetNet; }
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDCONTAINERSHEETNET_H
