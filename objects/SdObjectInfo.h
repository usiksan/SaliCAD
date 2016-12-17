/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Object information in global database.
*/

#ifndef SDOBJECTINFO_H
#define SDOBJECTINFO_H

#include <QString>
#include <QDataStream>
#include <QJsonObject>

struct SdObjectInfo
  {
    QString        mTitle;     //Item title
    QString        mAuthor;    //Item author (registered program copy name)
    int            mIndex;     //Local index in author context
    int            mRevision;  //Item revision
    qint8          mPublic;    //0 - private object, 1 - item make public by copy to SaliLAB server, 2 - item is public and copied


    SdObjectInfo();

    QString        getId() const;
    QString        getIdFileName() const;
    QString        getExtendTitle() const;
    void           setTitle( const QString title );

    void           writeObject(QJsonObject &obj) const;
    void           readObject(const QJsonObject obj);

    void           copyFrom( const SdObjectInfo &src );
  };


#endif // SDOBJECTINFO_H
