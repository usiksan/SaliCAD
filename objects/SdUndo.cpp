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




void SdUndo::propTextAndText(SdPropText *prp, SdPoint *org, SdRect *r, QString *str)
  {
  addUndo( new SdUndoRecordPropTextAndText( prp, r, str ) );
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

