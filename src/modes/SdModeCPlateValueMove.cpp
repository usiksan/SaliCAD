/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for value of component moving in the plate
*/
#include "SdModeCPlateValueMove.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdEnvir.h"
#include "objects/SdPulsar.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

SdModeCPlateValueMove::SdModeCPlateValueMove(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCPlateIdentMove( editor, obj )
  {

  }




QString SdModeCPlateValueMove::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCPlateValueMove.htm" );
  }




QString SdModeCPlateValueMove::getStepThema() const
  {
  if( getStep() )
    return QStringLiteral( MODE_HELP "ModeCPlateValueMove.htm#placeValue" );
  return QStringLiteral( MODE_HELP "ModeCPlateValueMove.htm#selectComp" );
  }





int SdModeCPlateValueMove::getIndex() const
  {
  return MD_PLATE_VALUE;
  }




void SdModeCPlateValueMove::setProp(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  SdPtr<SdGraphPartImp> imp(mImp);
  if( imp.isValid() )
    imp->valueSet( prp, pos, undo );
  }




void SdModeCPlateValueMove::getProp()
  {
  SdPtr<SdGraphPartImp> imp(mImp);
  if( imp.isValid() ) {
    //Get implement matrix
    mImpMatrix     = imp->matrix().inverted();
    //Get properties
    mPropSaved     = imp->valueProp();
    mPositionSaved = imp->valuePosition();
    }
  }


