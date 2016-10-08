/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPROJECT_H
#define SDPROJECT_H

#include "SdOwner.h"
#include "SdProjectItem.h"
#include <QList>

#define SD_TYPE_PROJECT "Project"

class SdProject : public SdOwner
  {
    QList<SdProjectItemPtr> mItems;
  public:
    SdProject();
    SdProject( SdProject *src );
    ~SdProject();

    virtual QString   getType() const override;
    virtual int       getClass() const override;
    virtual SdObject *copy() override;
    virtual void      writeObject(QJsonObject &obj) const override;
    virtual void      forEach(SdIterator &i) override;

    static SdProject *load( const QString fname );
  };

#endif // SDPROJECT_H
