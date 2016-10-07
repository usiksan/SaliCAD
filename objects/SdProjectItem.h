/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPROJECTITEM_H
#define SDPROJECTITEM_H

#include "SdObject.h"
#include <QString>

#define SDKO_TITLE "title"

class SdProjectItem : public SdObject
  {
    QString mTitle;
  public:
    SdProjectItem();
    SdProjectItem( SdProjectItem &src );

    QString                getTitle() const { return mTitle; }

    virtual QString        getIconName() const = 0;


    // SdObject interface
  public:
    virtual void writeObject(QJsonObject &obj) override;
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDPROJECTITEM_H
