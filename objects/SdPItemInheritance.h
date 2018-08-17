/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Component inheritance which specify component params
  Component is symbol without graph bun contains symbol sections with pin (name-number) assotiation and component part
*/
#ifndef SDPITEMINHERITANCE_H
#define SDPITEMINHERITANCE_H

#include "SdProjectItem.h"

#define SD_TYPE_INHERITANCE "Inhtn"

class SdPItemInheritance : public SdProjectItem
  {
    QString mComponentUid;   //Inherited component uid
    QString mComponentTitle; //Inherited component title
  public:
    SdPItemInheritance();

    //Return component uid
    QString componentUid() const { return mComponentUid; }

    //Return component title
    QString componentTitle() const { return mComponentTitle; }

    //Set component uid
    void    setComponentUid( const QString uid, SdUndo *undo );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual SdClass getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
    virtual quint64 getAcceptedObjectsMask() const override;
    virtual void    getHeader( SdLibraryHeader &hdr ) const override;
  };

#endif // SDPITEMINHERITANCE_H
