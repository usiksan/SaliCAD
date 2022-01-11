/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Special variable. When we assign to it then 2d elements appended to the PartItem.
*/
#include "SdM3dPartGraph.h"
#include "objects/SdPItemPart.h"

SdM3dPartGraph::SdM3dPartGraph(SdPItemPart *part) :
  SdM3dVariable(),
  mMasterPart(part)
  {

  }



void SdM3dPartGraph::assign(SdM3dValuePtr src)
  {
  if( mMasterPart.mItem != nullptr ) {
    SdScriptVal2dGraph flat = src->toGraph();
    switch( flat.mType ) {
      case SdScriptVal2dGraph::sdm2dLine :
        mMasterPart.addLine( flat.pointA().x(), flat.pointA().y(), flat.pointB().x(), flat.pointB().y() );
        break;
      case SdScriptVal2dGraph::sdm2dRect :
        mMasterPart.addRect( flat.pointA().x(), flat.pointA().y(), flat.pointB().x(), flat.pointB().y() );
        break;
      case SdScriptVal2dGraph::sdm2dCircle :
        mMasterPart.addCircle( flat.circleCenter().x(), flat.circleCenter().y(), flat.circleRadius() );
        break;
      case SdScriptVal2dGraph::sdm2dFRect :
        mMasterPart.addFRect( flat.pointA().x(), flat.pointA().y(), flat.pointB().x(), flat.pointB().y() );
        break;
      case SdScriptVal2dGraph::sdm2dPin :
        mMasterPart.addPinEx( flat.pinPos(), flat.pinPad(), flat.pinNumberPos(), flat.pinNumber(), flat.pinNumberAttr(), flat.pinNamePos(), flat.pinNameAttr() );
        break;
      case SdScriptVal2dGraph::sdm2dIdent :
        mMasterPart.setId( SdPoint(flat.identPos()) );
        break;
      case SdScriptVal2dGraph::sdm2dValue :
        mMasterPart.setValue( SdPoint(flat.valuePos()) );
        break;
      }
    }
  }
