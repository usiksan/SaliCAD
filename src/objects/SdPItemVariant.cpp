/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Variant contains table with parameters variant
  Table is square array with columns - parameters and with rows - values of parameters.
  This table contained in StringList, first element of it is fields names
*/
#include "SdPItemVariant.h"
#include "SdJsonIO.h"

SdPItemVariant::SdPItemVariant() :
  SdPItemWithPart(),
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




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdPItemVariant::json( SdJsonWriter &js ) const
  {
  //For space economy we write variant table only if it present
  if( mVariantFieldCount ) {
    js.jsonInt( QStringLiteral("VariantFC"), mVariantFieldCount );
    js.jsonListString( QStringLiteral("VariantTab"), mVariantTable );
    }
  SdProjectItem::json( js );
  }






//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdPItemVariant::json( const SdJsonReader &js )
  {
  if( js.contains(QStringLiteral("VariantFC")) ) {
    js.jsonInt( QStringLiteral("VariantFC"), mVariantFieldCount );
    js.jsonListString( QStringLiteral("VariantTab"), mVariantTable );
    }
  SdProjectItem::json( js );
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





