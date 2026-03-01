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
#include "SdUndoRecordInsertDelete.h"
#include "SdUndoRecordLinkSection.h"
#include "SdUndoRecordSymImpPin.h"
#include "SdUndoRecordProjectItem.h"
#include "SdUndoRecordStringMapItem.h"
#include "SdUndoRecord3dMatrix.h"
#include "windows/SdWCommand.h"

#include <QDebug>

SdUndo::SdUndo() :
  mUndoCount(0)
  {

  }




void SdUndo::insertObject(SdContainer *container, SdObject *object)
  {
  addUndo( new SdUndoRecordInsertDelete( container, object, true ) );
  }




void SdUndo::deleteObject(SdContainer *container, SdObject *object)
  {
  addUndo( new SdUndoRecordInsertDelete( container, object, false ) );
  }
















void SdUndo::pinSymImpStatus(SdGraphSymImp *sym, const QString symPinName)
  {
  addUndo( new SdUndoRecordSymImpPin( sym, symPinName )  );
  }











void SdUndo::linkSection(int section, SdGraphSymImp *sym, SdGraphPartImp *part, bool link)
  {
  addUndo( new SdUndoRecordLinkSection( section, sym, part, link ) );
  }












//!
//! \brief begin Appends "begin" record of group of undo commands. When undo then
//!              group undo as single unit
//! \param title Title for undo
//! \param item  Item on which undo executed
//! \param is3d  When true then item used in 3d view mode
//!
void SdUndo::begin(QString title, SdProjectItem *item, bool is3d)
  {
  if( mUndo.count() && mUndo.top()->isStep() ) {
    //Previous record is empty. Remove it
    delete mUndo.pop();
    mUndoCount--;
    }
  addUndo( new SdUndoRecordBegin(title, item, is3d) );
  }





void SdUndo::projectItemInfo(SdProjectItem *item, QString *title, QString *author, SdFileUid *fileUid, bool *editEnable, bool *publicSrc)
  {
  addUndo( new SdUndoRecordProjectItem( item, title, author, fileUid, editEnable, publicSrc ) );
  }







void SdUndo::stringMapItem(SdStringMap *assoc, const QString key)
  {
  addUndo( new SdUndoRecordStringMapItem( assoc, key ) );
  }













//!
//! \brief matrix3d Add undo for 3d matrix conversion
//! \param matrix   Matrix for conversion
//! \param part     Part on which conversion applied
//!
void SdUndo::matrix3d(QMatrix4x4 matrix, SdPItemPart *part)
  {
  addUndo( new SdUndoRecord3dMatrix( matrix, part ) );
  }






void SdUndo::undoStep()
  {
  //On undo we shift records from mUndo to mRedo
  //at each shift we undo record
  bool isStep = false;
  while( mUndo.count() && !isStep ) {
    isStep = mUndo.top()->isStep();
    mUndo.top()->undo();
    mRedo.push( mUndo.pop() );
    }
  if( mUndoCount ) mUndoCount--;
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
  }





//Update undo and redo commands status
void SdUndo::undoRedoUpdate()
  {
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

  //On each undo all redo must be clear
  if( mRedo.count() ) {
    qDeleteAll( mRedo );
    mRedo.clear();
    }

  //Add record
  mUndo.push( ptr );
  }

