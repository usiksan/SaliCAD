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




//!
//! \brief isFieldPresent Test if field name present in variant table
//! \param fieldName      Field name for test
//! \return               true if field with this name presents in variant table
//!
bool SdPItemVariant::isFieldPresent(const QString fieldName) const
  {
  //Scan all fields and test
  for( int i = 0; i < mVariantFieldCount; i++ )
    if( mVariantTable.at(i) == fieldName ) return true;
  return false;
  }




//!
//! \brief partIsPresent Returns true if contains one or more part variants
//! \return              true if contains one or more part variants
//!
bool SdPItemVariant::partIsPresent() const
  {
  return partGet() != nullptr;
  }




//!
//! \brief partTitleGet Returns part title for visual presentation
//! \param id           Id of part variant which title need to be returned. If empty then return primary part title
//! \return             Part title for visual presentation
//!
QString SdPItemVariant::partTitleGet() const
  {
  SdPartVariant *part = partGet();
  if( part != nullptr )
    return part->getPartTitle();
  return QString();
  }




//!
//! \brief partIdGet Returns part id for primary part variant
//! \return          Part id for primary part variant
//!
QString SdPItemVariant::partIdGet() const
  {
  SdPartVariant *part = partGet();
  if( part != nullptr )
    return part->getPartId();
  return QString();
  }




//!
//! \brief partIdSet Setup new part id for primary part variant
//! \param id        New id for primary part
//! \param undo
//!
void SdPItemVariant::partIdSet(const QString id, SdUndo *undo)
  {
  SdPartVariant *part = partGet();
  if( part == nullptr ) {
    //Part yet not present
    //Insert part variant
    part = new SdPartVariant();
    insertChild( part, undo );
    }
  part->setPartId( id, undo );
  }




//!
//! \brief partExtractFromFactory Return part descripted part variant
//! \return                       Part descripted part variant
//!
SdPItemPart *SdPItemVariant::partExtractFromFactory() const
  {
  SdPartVariant *part = partGet();
  if( part != nullptr )
    return part->extractFromFactory();
  return nullptr;
  }




//!
//! \brief partGet Returns part variant for id. If id is empty then return primary part variant
//! \return
//!
SdPartVariant *SdPItemVariant::partGet() const
  {
  //Pointer where reside SdPartVariant
  SdPartVariantPtr part = nullptr;
  //Scan all objects and find SdPartVariant
  forEachConst( dctPartVariant, [&part] (SdObject *obj) -> bool {
    part = dynamic_cast<SdPartVariant*>(obj);
    Q_ASSERT( part != nullptr );
    return false;
    });
  return part;
  }




//!
//! \brief partRemove Removes part variant from part list
//! \param undo
//!
void SdPItemVariant::partRemove(SdUndo *undo)
  {
  SdPartVariant *part = partGet();
  if( part != nullptr )
    deleteChild( part, undo );
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
  hdr.mPartPresent       = partIsPresent();
  }









