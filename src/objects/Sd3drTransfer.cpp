#include "Sd3drTransfer.h"

Sd3drTransfer::Sd3drTransfer()
  {

  }

QMatrix4x4 Sd3drTransfer::mapGet() const
  {
  QMatrix4x4 map;
  map.rotate( mAngle, mRotate );
  map.translate( mOffset );

  return map;
  }
