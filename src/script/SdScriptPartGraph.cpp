/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Special variable. When we assign to it then 2d elements appended to the PartItem.
*/
#include "SdScriptPartGraph.h"
#include "objects/SdPItemPart.h"

SdScriptPartGraph::SdScriptPartGraph(SdPItemPart *part) :
  SdScriptValueVariable(),
  mMasterPart(part)
  {

  }



void SdScriptPartGraph::assign(SdScriptValuePtr src)
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
