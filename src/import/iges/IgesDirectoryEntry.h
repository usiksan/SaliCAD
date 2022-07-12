#ifndef IGESDIRECTORYENTRY_H
#define IGESDIRECTORYENTRY_H

#include "IgesEntity.h"

#include <QByteArray>
#include <QList>

class IgesReader;

class IgesDirectoryEntry
  {
    //int         mIndex;
    int         mFields[18];
    char        mStatus[8];
    char        mLabel[8];

    IgesEntity *mEntity;
  public:
    IgesDirectoryEntry();
    ~IgesDirectoryEntry();

    void setField( int i, const QByteArray &ar );

    IgesEntity *entity() const { return mEntity; }

    void        parse( const IgesReader &reader );
  };

using IgesDirectoryEntryList = QList<IgesDirectoryEntry>;

#endif // IGESDIRECTORYENTRY_H
