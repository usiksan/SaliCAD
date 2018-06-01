/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for two strings.
*/
#include "SdUndoRecordString2.h"

SdUndoRecordString2::SdUndoRecordString2(QString *str1, QString *str2) :
  mSrc1(str1),
  mSrc2(str2)
  {
  if( mSrc1 != nullptr ) mStr1 = *mSrc1;
  if( mSrc2 != nullptr ) mStr2 = *mSrc2;
  }




void SdUndoRecordString2::undo()
  {
  if( mSrc1 != nullptr ) mSrc1->swap( mStr1 );
  if( mSrc2 != nullptr ) mSrc2->swap( mStr2 );
  }
