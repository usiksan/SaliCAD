/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo link beatween symbol implementation and part implementation
*/
#include "SdUndoRecordLinkSection.h"
#include "SdGraphSymImp.h"
#include "SdGraphPartImp.h"

SdUndoRecordLinkSection::SdUndoRecordLinkSection(int section, SdGraphSymImp *sym, SdGraphPartImp *part, bool link ) :
  SdUndoRecord(),
  mSection(section),
  mLink(link),
  mSym(sym),
  mPart(part)
  {

  }


void SdUndoRecordLinkSection::undo()
  {
  if( mPart )
    mPart->setLinkSection( mSection, mLink ? mSym : nullptr );
  if( mSym )
    mSym->setLinkSection( mSection, mLink ? mPart : nullptr );
  mLink = !mLink;
  }
