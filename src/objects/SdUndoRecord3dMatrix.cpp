#include "SdUndoRecord3dMatrix.h"
#include "SdPItemPart.h"

SdUndoRecord3dMatrix::SdUndoRecord3dMatrix(QMatrix4x4 matrix, SdPItemPart *part) :
  SdUndoRecord(),
  mMatrix(matrix),
  mPart(part)
  {
  }


void SdUndoRecord3dMatrix::undo()
  {
  //We must apply inverted conversion, so we get inverted matrix
  mMatrix = mMatrix.inverted();

  //Apply undo operation
  mPart->matrixMap( mMatrix, nullptr );
  }
