/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConfig.h"
#include "SdObjectInfo.h"
#include "SdIds.h"
#include <QSettings>

SdObjectInfo::SdObjectInfo() :
  mIndex(0),
  mRevision(0),
  mPublic(true)
  {

  }




QString SdObjectInfo::getId() const
  {
  //Id consist from name, user, revision and local num
  return mTitle + mAuthor + QString::number(mIndex,32) + QString::number(mRevision,32);
  }





QString SdObjectInfo::getIdFileName() const
  {
  //Сформировать имя
  QString fname = QString("%1.%2.i%3.r%4" SD_BASE_EXTENSION ).arg( mTitle ).arg( mAuthor ).arg( mIndex,0,16 ).arg( mRevision, 0, 16);
  //Заменить все специальные символы на знак _
  for( int i = 0; i < fname.length(); i++ )
    if( fname[i] != QChar('.') && fname[i] != QChar('-') && !fname[i].isLetterOrNumber() )
      fname[i] = QChar('_');
  return fname;
  }




QString SdObjectInfo::getExtendTitle() const
  {
  return QString("%1 [r%2] (%3)").arg(mTitle).arg(mRevision).arg(mAuthor);
  }




void SdObjectInfo::setTitle(const QString title)
  {
  QSettings s;
  //Item author (registered program copy name)
  mAuthor = s.value( SDK_GLOBAL_ID_MACHINE ).toString();
  //Local index in author context
  mIndex  = getGlobalId();
  //Item revision
  mRevision = 0;
  //If true - item make public by copy to SaliLAB server
  mPublic   = true;
  //Title setup
  mTitle    = title;
  }




void SdObjectInfo::writeObject(QJsonObject &obj) const
  {
  obj.insert( QStringLiteral("Title"),    mTitle );
  obj.insert( QStringLiteral("Author"),   mAuthor );
  obj.insert( QStringLiteral("Index"),    mIndex );
  obj.insert( QStringLiteral("Revision"), mRevision );
  obj.insert( QStringLiteral("Public"),   mPublic );
  }




void SdObjectInfo::readObject(const QJsonObject obj)
  {
  mTitle    = obj.value( QStringLiteral("Title") ).toString();
  mAuthor   = obj.value( QStringLiteral("Author") ).toString();
  mIndex    = obj.value( QStringLiteral("Index") ).toInt();
  mRevision = obj.value( QStringLiteral("Revision") ).toInt();
  mPublic   = obj.value( QStringLiteral("Public") ).toBool();
  }




void SdObjectInfo::copyFrom(const SdObjectInfo &src)
  {
  mTitle    = src.mTitle;
  mAuthor   = src.mAuthor;
  mIndex    = src.mIndex;
  mRevision = src.mRevision;
  mPublic   = src.mPublic;
  }

