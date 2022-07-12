#include "IgesEntity.h"
#include "IgesEntityColor.h"

#include "IgesReader.h"

IgesEntity::IgesEntity()
  {

  }



void IgesEntity::setField(int fieldIndex, const QByteArray &ar)
  {
  bool inv = ar.simplified().isEmpty();
  if( fieldIndex >= 2 && fieldIndex <= 8 )
    mFields[fieldIndex-2] = inv ? IgesIntEmpty : ar.simplified().toInt();
  else if( fieldIndex == 9 )
    memcpy( mStatus, ar.data(), 8 );
  else if( fieldIndex >= 12 && fieldIndex <= 15 )
    mFields[fieldIndex-12+7] = inv ? IgesIntEmpty : ar.simplified().toInt();
  else if( fieldIndex == 18 )
    memcpy( mEntityLabel, ar.data(), 8 );
  else if( fieldIndex == 19 )
    mFields[11] = inv ? IgesIntEmpty : ar.simplified().toInt();
  }

bool IgesEntity::parse(IgesReader *reader)
  {
  reader->parameterDataInit( mParameterData() );

  //Skeep entity type
  int type;
  return reader->paramInt( type );
  }



IgesEntity *IgesEntity::build(int type)
  {
  switch( type ) {
    case 314 : return new IgesEntityColor();
    }
  return new IgesEntity();
  }



