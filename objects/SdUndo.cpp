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

void SdUndo::point(SdPoint *p)
  {

  }

void SdUndo::pointIndex(SdPointTable *table, int index)
  {

  }

void SdUndo::intValue(int *v)
  {

  }

void SdUndo::propInt(SdPropInt *propInt)
  {

  }

void SdUndo::propLayer(SdPropLayer *propLayer)
  {

  }

void SdUndo::propString(QString *str)
  {

  }

void SdUndo::begin(QString title)
  {
  addUndo( new SdUndoRecordBegin(title) );
  }




void SdUndo::undoStep()
  {
  while( mUndo.count() && !mUndo.top()->isStep() ) {
    mUndo.top()->undo();
    delete mUndo.pop();
    }
  if( mUndo.count() )
    delete mUndo.pop();
  SdWCommand::cmEditUndo->setEnabled( isUndoPresent() );
  SdWCommand::cmEditRedo->setEnabled( isRedoPresent() );
  }





void SdUndo::redoStep()
  {

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

