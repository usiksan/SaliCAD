#include "SdPItemVariant.h"

SdPItemVariant::SdPItemVariant() :
  SdProjectItem(),
  mVariantFieldCount(0)
  {

  }




QString SdPItemVariant::getType() const
  {
  return QStringLiteral(SD_TYPE_VARIANT);
  }




SdClass SdPItemVariant::getClass() const
  {
  return dctVariant;
  }




void SdPItemVariant::cloneFrom(const SdObject *src)
  {
  SdProjectItem::cloneFrom( src );
  SdPtrConst<SdPItemVariant> var(src);
  if( var.isValid() ) {
    mVariantFieldCount = var->mVariantFieldCount;
    mVariantTable      = var->mVariantTable;
    }
  }




void SdPItemVariant::writeObject(QJsonObject &obj) const
  {
  SdProjectItem::writeObject( obj );
  //For space economy we write variant table only if it present
  if( mVariantFieldCount ) {
    obj.insert( QStringLiteral("VariantFC"), mVariantFieldCount );
    obj.insert( QStringLiteral("VariantTab"), QJsonArray::fromStringList(mVariantTable) );
    }
  }




void SdPItemVariant::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdProjectItem::readObject( map, obj );
  if( obj.contains(QStringLiteral("VariantFC")) ) {
    mVariantFieldCount = obj.value( QStringLiteral("VariantFC") ).toInt();
    QJsonArray ar = obj.value( QStringLiteral("VariantTab") ).toArray();
    mVariantTable.clear();
    for( auto iter = ar.constBegin(); iter != ar.constEnd(); iter++ )
      mVariantTable.append( iter->toString() );
    }
  }




void SdPItemVariant::getHeader(SdLibraryHeader &hdr) const
  {
  }

QString SdPItemVariant::getIconName() const
  {
  }

quint64 SdPItemVariant::getAcceptedObjectsMask() const
  {
  }
