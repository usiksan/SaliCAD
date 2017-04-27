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
#include <QSettings>
#include <QDateTime>

#define timeOffsetConstant 1000000000L

SdObjectInfo::SdObjectInfo() :
  mCreateTime(0),
  mStatus(soiEditing)
  {

  }



qint64 SdObjectInfo::getTimeFromEpoch() const
  {
  return timeOffsetConstant + static_cast<qint64>(mCreateTime);
  }




QString SdObjectInfo::getId() const
  {
  //Id consist from name, user and time creation
  return mTitle + mAuthor + QString::number(mCreateTime,32);
  }





QString SdObjectInfo::getIdFileName() const
  {
  //Сформировать имя
  QString fname = QString("%1.%2.i%3" SD_BASE_EXTENSION ).arg( mTitle ).arg( mAuthor ).arg( mCreateTime,0,16 );
  //Заменить все специальные символы на знак _
  for( int i = 0; i < fname.length(); i++ )
    if( fname[i] != QChar('.') && fname[i] != QChar('-') && !fname[i].isLetterOrNumber() )
      fname[i] = QChar('_');
  return fname;
  }




QString SdObjectInfo::getExtendTitle() const
  {
  return QString("%1 [r%2] (%3)").arg(mTitle).arg( QDateTime::fromSecsSinceEpoch(getTimeFromEpoch()).toString("yy-M-d H:m:s") ).arg(mAuthor);
  }





void SdObjectInfo::setTitle(const QString title)
  {
  QSettings s;
  //Item author (registered program copy name)
  mAuthor     = s.value( SDK_GLOBAL_ID_MACHINE ).toString();
  //Update creation time
  mCreateTime = static_cast<int>( QDateTime::currentDateTime().toSecsSinceEpoch() - timeOffsetConstant );
  //Set editing status
  mStatus     = soiEditing;
  //Title setup
  mTitle      = title;
  }




void SdObjectInfo::writeObject(QJsonObject &obj) const
  {
  obj.insert( QStringLiteral("Title"),    mTitle );
  obj.insert( QStringLiteral("Author"),   mAuthor );
  obj.insert( QStringLiteral("Created"),  mCreateTime );
  obj.insert( QStringLiteral("Status"),   mStatus );
  }




void SdObjectInfo::readObject(const QJsonObject obj)
  {
  mTitle      = obj.value( QStringLiteral("Title") ).toString();
  mAuthor     = obj.value( QStringLiteral("Author") ).toString();
  mCreateTime = obj.value( QStringLiteral("Created") ).toInt();
  mStatus     = obj.value( QStringLiteral("Status") ).toInt();
  }




void SdObjectInfo::copyFrom(const SdObjectInfo &src)
  {
  mTitle      = src.mTitle;
  mAuthor     = src.mAuthor;
  mCreateTime = src.mCreateTime;
  mStatus     = src.mStatus;
  }

