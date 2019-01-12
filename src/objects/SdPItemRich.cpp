/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rich and plain text representation
*/
#include "SdPItemRich.h"

#include <QTextEdit>

SdPItemRich::SdPItemRich()
  {

  }




QString SdPItemRich::getType() const
  {
  return QStringLiteral(SD_TYPE_RICH);
  }




SdClass SdPItemRich::getClass() const
  {
  return dctRich;
  }




void SdPItemRich::cloneFrom(const SdObject *src)
  {
  SdProjectItem::cloneFrom( src );
  SdPtrConst<SdPItemRich> rich(src);
  if( rich.isValid() )
    mContents = rich->mContents;
  }




void SdPItemRich::writeObject(QJsonObject &obj) const
  {
  SdProjectItem::writeObject( obj );
  if( mTextEditor )
    obj.insert( QStringLiteral("contents"), mTextEditor->toHtml() );
  else
    obj.insert( QStringLiteral("contents"), mContents );
  }





void SdPItemRich::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdProjectItem::readObject( map, obj );
  mContents = obj.value( QStringLiteral("contents") ).toString();
  }




QString SdPItemRich::getIconName() const
  {
  return QStringLiteral(":/pic/iconRich.png");
  }




quint64 SdPItemRich::getAcceptedObjectsMask() const
  {
  return 0ul;
  }
