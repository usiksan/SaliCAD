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

#define soiEditing          0x00 //Object in editing state
#define soiReadyForPublic   0x01 //Object used and prepared for publicing
#define soiPublished        0x02 //Object already published
#define soiPrivate          0x10 //Private object. This possibility only for licensed saliCAD

struct SdObjectInfo
  {
    QString        mTitle;      //Item title
    QString        mAuthor;     //Item author (registered program copy name)
    int            mCreateTime; //Create time with sec from 2000year
    int            mStatus;     //soiXXX constant


    SdObjectInfo();

    QString        getTitle() const { return mTitle; }
    QString        getAuthor() const { return mAuthor; }
    int            getTime() const { return mCreateTime; }
    qint64         getTimeFromEpoch() const;

    bool           isEditing() const { return mStatus == soiEditing; }
    bool           isReadyForPublic() const { return mStatus == soiReadyForPublic; }
    bool           isPublished() const { return mStatus == soiPublished; }

    QString        getId() const;
    QString        getIdFileName() const;
    QString        getExtendTitle() const;
    void           setTitle( const QString title );

    void           writeObject(QJsonObject &obj) const;
    void           readObject(const QJsonObject obj);

    void           copyFrom( const SdObjectInfo &src );
  };


#endif // SDOBJECTINFO_H
