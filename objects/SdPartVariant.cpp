/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part variant contains one part variant for component. Variants may be
  different component placement with the same part or different parts of one component.
*/
#include "SdPartVariant.h"
#include "SdObjectFactory.h"
#include "SdPItemPart.h"

SdPartVariant::SdPartVariant() :
  SdObject(),
  mDefault(false)
  {

  }



SdPItemPart *SdPartVariant::extractFromFactory(bool soft, QWidget *parent) const
  {
  return dynamic_cast<SdPItemPart*>( SdObjectFactory::extractObject( mPartTitle, mPartAuthor, soft, parent ) );
  }




QString SdPartVariant::getTitle() const
  {
  return QString( "%1 (%2)" ).arg(mPartTitle).arg(mPartAuthor);
  }




void SdPartVariant::updateFromPart(SdPItemPart *part)
  {
  if( part ) {
    mPartTitle  = part->getTitle();
    mPartAuthor = part->getAuthor();
    }
  }



QString SdPartVariant::getType() const
  {
  return QString( SD_TYPE_PART_VARIANT );
  }





quint64 SdPartVariant::getClass() const
  {
  return dctPartVariant;
  }





void SdPartVariant::cloneFrom(const SdObject *src)
  {
  SdObject::cloneFrom( src );
  const SdPartVariant *part = dynamic_cast<const SdPartVariant*>(src);
  if( part ) {
    mPartTitle   = part->mPartTitle;
    mPartAuthor = part->mPartAuthor;
    mDefault    = part->mDefault;
    }
  }




void SdPartVariant::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  obj.insert( QString("PartName"), mPartTitle );
  obj.insert( QString("PartAuthor"), mPartAuthor );
  obj.insert( QString("Default"), mDefault );
  }




void SdPartVariant::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mPartTitle   = obj.value( QString("PartName") ).toString();
  mPartAuthor = obj.value( QString("PartAuthor") ).toString();
  mDefault    = obj.value( QString("Default") ).toBool();
  }


