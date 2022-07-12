#include "IgesDirectoryEntry.h"

IgesDirectoryEntry::IgesDirectoryEntry() :
  mEntity(nullptr)
  {

  }

IgesDirectoryEntry::~IgesDirectoryEntry()
  {
  if( mEntity != nullptr )
    delete mEntity;
  }

void IgesDirectoryEntry::setField(int i, const QByteArray &ar)
  {
  if( i < 8 || (i > 9 && i < 16) || i == 17 ) mFields[i] = ar.simplified().toInt();
  else if( i == 8 ) memcpy( mStatus, ar.data(), 8 );
  else if( i == 16 ) memcpy( mLabel, ar.data(), 8 );

  if( mEntity == nullptr && i == 0 )
    mEntity = IgesEntity::build( mFields[0] );
  }



void IgesDirectoryEntry::parse(const IgesReader &reader)
  {

  }


