/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for value of component moving in the schematic sheet
*/
#include "SdModeCSheetValueMove.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdEnvir.h"
#include "objects/SdPulsar.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

SdModeCSheetValueMove::SdModeCSheetValueMove(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCSheetIdentMove( editor, obj )
  {

  }




QString SdModeCSheetValueMove::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCSheetValueMove.htm" );
  }




QString SdModeCSheetValueMove::getStepThema() const
  {
  if( getStep() )
    return QStringLiteral( MODE_HELP "ModeCSheetValueMove.htm#placeIdent" );
  return QStringLiteral( MODE_HELP "ModeCSheetValueMove.htm#selectComp" );
  }




int SdModeCSheetValueMove::getIndex() const
  {
  return MD_SHEET_VALUE;
  }





void SdModeCSheetValueMove::setProp(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  mImp->valueSet( prp, pos, undo );
  }




void SdModeCSheetValueMove::getProp()
  {
  mPropSaved     = mImp->valueProp();
  mPositionSaved = mImp->valuePosition();
  }



