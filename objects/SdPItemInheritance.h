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
#define SD_COMPONENT_UID    "CompUid"
#define SD_COMPONENT_TITLE  "CompTitle"

class SdPItemInheritance : public SdProjectItem
  {
  public:
    SdPItemInheritance();

    //Return component uid
    QString componentUid() const;

    //Return component title
    QString componentTitle() const;

    //Set component uid
    void    setComponentUid( const QString uid, SdUndo *undo );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual SdClass getClass() const override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
    virtual quint64 getAcceptedObjectsMask() const override;
  };

#endif // SDPITEMINHERITANCE_H
