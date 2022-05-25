#include "SdPItemVariant.h"
#include "SdJsonIO.h"

SdPItemVariant::SdPItemVariant() :
  SdProjectItem(),
  mVariantFieldCount(0)
  {

  }




//To variant table access
void SdPItemVariant::variantTableGet(qint32 &fieldCount, QStringList &list) const
  {
  fieldCount = mVariantFieldCount;
  list       = mVariantTable;
  }




void SdPItemVariant::variantTableSet(qint32 fieldCount, QStringList list, SdUndo *undo)
  {
  if( undo )
    undo->stringList( &mVariantFieldCount, &mVariantTable );
  mVariantFieldCount = fieldCount;
  mVariantTable      = list;
  }




//Test if field name present in variant table
bool SdPItemVariant::isFieldPresent(const QString fieldName) const
  {
  //Scan all fields and test
  for( int i = 0; i < mVariantFieldCount; i++ )
    if( mVariantTable.at(i) == fieldName ) return true;
  return false;
  }




QString SdPItemVariant::getType() const
  {
  return QStringLiteral(SD_TYPE_VARIANT);
  }




SdClass SdPItemVariant::getClass() const
  {
  return dctVariant;
  }





//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdPItemVariant::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdProjectItem::cloneFrom( src, copyMap, next );
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
  SdProjectItem::getHeader( hdr );
  hdr.mVariantFieldCount = mVariantFieldCount;
  hdr.mVariantTable      = mVariantTable;
  }




QString SdPItemVariant::getIconName() const
  {
  return QString(":/pic/iconComp.png");
  }




quint64 SdPItemVariant::getAcceptedObjectsMask() const
  {
  return 0;
  }


void SdPItemVariant::json( SdJsonWriter &js ) const
  {
  //For space economy we write variant table only if it present
  if( mVariantFieldCount ) {
    js.jsonInt( QStringLiteral("VariantFC"), mVariantFieldCount );
    js.jsonListString( QStringLiteral("VariantTab"), mVariantTable );
    }
  SdProjectItem::json( js );
  }



void SdPItemVariant::json( const SdJsonReader &js )
  {
  if( js.contains(QStringLiteral("VariantFC")) ) {
    js.jsonInt( QStringLiteral("VariantFC"), mVariantFieldCount );
    js.jsonListString( QStringLiteral("VariantTab"), mVariantTable );
    }
  SdProjectItem::json( js );
  }
