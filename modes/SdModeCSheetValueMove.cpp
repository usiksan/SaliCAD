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
    return QStringLiteral( MODE_HELP "ModeCSheetValueMove.htm#placeValue" );
  return QStringLiteral( MODE_HELP "ModeCSheetValueMove.htm#selectComp" );
  }




int SdModeCSheetValueMove::getIndex() const
  {
  return MD_SHEET_VALUE;
  }





void SdModeCSheetValueMove::setProp(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  SdPtr<SdGraphSymImp> imp(mImp);
  if( imp.isValid() )
    imp->valueSet( prp, pos, undo );
  }




void SdModeCSheetValueMove::getProp()
  {
  SdPtr<SdGraphSymImp> imp(mImp);
  if( imp.isValid() ) {
    //Get implement matrix
    mImpMatrix     = imp->matrix().inverted();
    mPropSaved     = imp->valueProp();
    mPositionSaved = imp->valuePosition();
    }
  }



