/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdUndo.h"
#include "SdUndoRecordBegin.h"
#include "SdUndoRecordInsert.h"
#include "SdUndoRecordPropLineAnd2Points.h"
#include "SdUndoRecordPropTextAndText.h"
#include "SdUndoRecordPropLinePointInt.h"
#include "SdUndoRecordPropLineAndPointsList.h"
#include "SdUndoRecordPropLineRect2Int.h"
#include "SdUndoRecordPropSymPin.h"
#include "SdUndoRecordPropPartPin.h"
#include "SdUndoRecordPoint.h"
#include "SdUndoRecordPlatePointer.h"
#include "SdUndoRecordImpPin.h"
#include "SdUndoRecordLinkSection.h"
#include "SdUndoRecordSymImp.h"
#include "SdUndoRecordPartImp.h"
#include "SdUndoRecordWire.h"
#include "windows/SdWCommand.h"

SdUndo::SdUndo() :
  mUndoCount(0)
  {

  }




void SdUndo::insertObject(SdContainer *container, SdObject *object)
  {
  addUndo( new SdUndoRecordInsert( container, object ) );
  }




void SdUndo::deleteObject(SdContainer *container, SdObject *object)
  {
  //TODO deleteObject
  }




void SdUndo::propLineAnd2Point(SdPropLine *prp, SdPoint *p1, SdPoint *p2)
  {
  addUndo( new SdUndoRecordPropLineAnd2Points(prp, p1, p2) );
  }




void SdUndo::propLinePointInt(SdPropLine *prp, SdPoint *p, int *val)
  {
  addUndo( new SdUndoRecordPropLinePointInt( prp, p, val ) );
  }




void SdUndo::propLinePointTable(SdPropLine *prp, SdPointList *list)
  {
  addUndo( new SdUndoRecordPropLineAndPointsList( prp, list ) );
  }




void SdUndo::propLineRect2Int(SdPropLine *prp, SdRect *r, int *val1, int *val2)
  {
  addUndo( new SdUndoRecordPropLineRect2Int( prp, r, val1, val2 ) );
  }




void SdUndo::propTextAndText(SdPropText *prp, SdPoint *org, SdRect *r, QString *str)
  {
  addUndo( new SdUndoRecordPropTextAndText( prp, org, r, str ) );
  }




void SdUndo::propSymPin(SdPropSymPin *prp, SdPoint *org)
  {
  addUndo( new SdUndoRecordPropSymPin( prp, org ) );
  }




void SdUndo::propPartPin(SdPropPartPin *prp, SdPoint *org)
  {
  addUndo( new SdUndoRecordPropPartPin( prp, org ) );
  }




void SdUndo::platePointer(SdPItemPlatePtr *ptr)
  {
  addUndo( new SdUndoRecordPlatePointer(ptr) );
  }





void SdUndo::pinImpConnect(SdGraphSymImp *sym, int symPinIndex, SdGraphPartImp *part, int partPinIndex, const QString wireName, bool com)
  {
  addUndo( new SdUndoRecordImpPin( sym, symPinIndex, part, partPinIndex, wireName, com ) );
  }




void SdUndo::linkSection(int section, SdGraphSymImp *sym, SdGraphPartImp *part, bool link)
  {
  addUndo( new SdUndoRecordLinkSection( section, sym, part, link ) );
  }




void SdUndo::symImp(SdPoint *origin, SdPropSymImp *imp, int *logSection, int *logNumber, SdRect *over, QString *prefix, SdPropText *identProp, SdPoint *identOrigin, SdPoint *identPos, SdRect *identRect)
  {
  addUndo( new SdUndoRecordSymImp( origin, imp, logSection, logNumber, over, prefix, identProp, identOrigin, identPos, identRect ) );
  }



void SdUndo::partImp(SdPoint *origin, SdPropPartImp *imp, int *logNumber, SdRect *over, QString *prefix, SdPropText *identProp, SdPoint *identOrigin, SdPoint *identPos, SdRect *identRect)
  {
  addUndo( new SdUndoRecordPartImp( origin, imp, logNumber, over, prefix, identProp, identOrigin, identPos, identRect ) );
  }



void SdUndo::wire(SdPropLine *prop, SdPoint *p1, SdPoint *p2, bool *dot1, bool *dot2)
  {
  addUndo( new SdUndoRecordWire( prop, p1, p2, dot1, dot2 ) );
  }




void SdUndo::point(SdPoint *src)
  {
  addUndo( new SdUndoRecordPoint(src) );
  }



void SdUndo::begin(QString title)
  {
  addUndo( new SdUndoRecordBegin(title) );
  }




void SdUndo::undoStep()
  {
  //On undo we shift records from mUndo to mRedo
  //at each shift we undo record
  while( mUndo.count() && !mUndo.top()->isStep() ) {
    mUndo.top()->undo();
    mRedo.push( mUndo.pop() );
    }
  if( mUndo.count() )
    mRedo.push( mUndo.pop() );
  if( mUndoCount ) mUndoCount--;
  SdWCommand::cmEditUndo->setEnabled( isUndoPresent() );
  SdWCommand::cmEditRedo->setEnabled( isRedoPresent() );
  }





void SdUndo::redoStep()
  {
  //On undo we shift records from mRedo to mUndo
  //at each shift we redo record
  do {
    mRedo.top()->redo();
    mUndo.push( mRedo.pop() );
    }
  while( mRedo.count() && !mRedo.top()->isStep() );

  mUndoCount++;

  SdWCommand::cmEditUndo->setEnabled( isUndoPresent() );
  SdWCommand::cmEditRedo->setEnabled( isRedoPresent() );
  }




void SdUndo::addUndo(SdUndoRecordPtr ptr)
  {
  //If it "begin" record then count up
  if( ptr->isStep() ) {
    mUndoCount++;
    SdWCommand::cmEditUndo->setEnabled(true);
    SdWCommand::cmEditRedo->setEnabled(false);
    }

  if( mUndoCount >= UNDO_HISTORY_SIZE ) {
    //If undo step count above limit - remove oldest step
    mUndoCount--;
    //Delete first record. It must be "begin" record
    delete mUndo.first();
    mUndo.removeFirst();
    //Delete records while not "begin"
    while( mUndo.count() && !mUndo.first()->isStep() ) {
      delete mUndo.first();
      mUndo.removeFirst();
      }
    }

  //On each ondo all redo must be clear
  if( mRedo.count() ) {
    qDeleteAll( mRedo );
    mRedo.clear();
    }

  //Add record
  mUndo.push( ptr );
  }

