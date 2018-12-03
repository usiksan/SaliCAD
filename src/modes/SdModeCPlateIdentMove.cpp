/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for ident of component moving in the plate
*/
#include "SdModeCPlateIdentMove.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdEnvir.h"
#include "objects/SdPulsar.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

SdModeCPlateIdentMove::SdModeCPlateIdentMove(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCSheetIdentMove( editor, obj )
  {

  }

QString SdModeCPlateIdentMove::getStepHelp() const
  {
  if( getStep() )
    return QObject::tr("Enter new ident or value position.");
  else {
    if( mBehindCursorTable.count() ) {
      SdPtr<SdGraphPartImp> imp(mBehindCursorTable.at(mBehindCursorIndex));
      if( imp.isValid() )
        return QObject::tr("Left button: %1; F6 - switch to next component behind cursor").arg(imp->ident());
      }
    }
  return QObject::tr("Select component to move ident or value");
  }



QString SdModeCPlateIdentMove::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCPlateIdentMove.htm" );
  }




QString SdModeCPlateIdentMove::getStepThema() const
  {
  if( getStep() )
    return QStringLiteral( MODE_HELP "ModeCPlateIdentMove.htm#placeIdent" );
  return QStringLiteral( MODE_HELP "ModeCPlateIdentMove.htm#selectComp" );
  }




int SdModeCPlateIdentMove::getIndex() const
  {
  return MD_PLATE_IDENT;
  }




void SdModeCPlateIdentMove::setProp(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  SdPtr<SdGraphPartImp> imp(mImp);
  if( imp.isValid() )
    imp->identSet( prp, pos, undo );
  }




void SdModeCPlateIdentMove::getProp()
  {
  SdPtr<SdGraphPartImp> imp(mImp);
  if( imp.isValid() ) {
    //Get implement matrix
    mImpMatrix     = imp->matrix().inverted();
    //Get properties
    mPropSaved     = imp->identProp();
    mPositionSaved = imp->identPosition();
    }
  }



