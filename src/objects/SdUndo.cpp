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
#include "SdUndoRecordPropLineAnd3Points.h"
#include "SdUndoRecordPropTextAndText.h"
#include "SdUndoRecordPropLinePointInt.h"
#include "SdUndoRecordPropLineAndPointsList.h"
#include "SdUndoRecordPropLineRect2Int.h"
#include "SdUndoRecordPropSymPin.h"
#include "SdUndoRecordPropPartPin.h"
#include "SdUndoRecordPoint.h"
#include "SdUndoRecordPlatePointer.h"
#include "SdUndoRecordLinkSection.h"
#include "SdUndoRecordSymImp.h"
#include "SdUndoRecordSymImpPin.h"
#include "SdUndoRecordSymImpPins.h"
#include "SdUndoRecordPartImp.h"
#include "SdUndoRecordPartImpPins.h"
#include "SdUndoRecordWire.h"
#include "SdUndoRecordProjectItem.h"
#include "SdUndoRecordString2.h"
#include "SdUndoRecordStringMapItem.h"
#include "SdUndoRecordStringMap.h"
#include "SdUndoRecordPadAssociation.h"
#include "SdUndoRecordRoad.h"
#include "SdUndoRecordVia.h"
#include "SdUndoRecordRules.h"
#include "SdUndoRecordStringList.h"
#include "SdUndoRecordPolygon.h"
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




void SdUndo::propLineAnd3Point(SdPropLine *prp, SdPoint *p1, SdPoint *p2, SdPoint *p3 )
  {
  addUndo( new SdUndoRecordPropLineAnd3Points(prp, p1, p2, p3) );
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





void SdUndo::pinSymImpStatus(SdGraphSymImp *sym, const QString symPinName)
  {
  addUndo( new SdUndoRecordSymImpPin( sym, symPinName )  );
  }






void SdUndo::symImpPins(SdSymImpPinTable *table)
  {
  addUndo( new SdUndoRecordSymImpPins(table) );
  }




void SdUndo::partImpPins(SdPartImpPinTable *table)
  {
  addUndo( new SdUndoRecordPartImpPins(table) );
  }







void SdUndo::linkSection(int section, SdGraphSymImp *sym, SdGraphPartImp *part, bool link)
  {
  addUndo( new SdUndoRecordLinkSection( section, sym, part, link ) );
  }




void SdUndo::symImp(SdPoint *origin, SdPropSymImp *imp, int *logSection, int *logNumber, SdRect *over )
  {
  addUndo( new SdUndoRecordSymImp( origin, imp, logSection, logNumber, over ) );
  }



void SdUndo::partImp(SdPoint *origin, SdPropPartImp *imp, int *logNumber, SdRect *over )
  {
  addUndo( new SdUndoRecordPartImp( origin, imp, logNumber, over ) );
  }



void SdUndo::wire(SdPropLine *prop, SdPoint *p1, SdPoint *p2, bool *dot1, bool *dot2)
  {
  addUndo( new SdUndoRecordWire( prop, p1, p2, dot1, dot2 ) );
  }




void SdUndo::point(SdPoint *src)
  {
  addUndo( new SdUndoRecordPoint(src) );
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





void SdUndo::projectItemInfo(SdProjectItem *item, QString *title, QString *author, int *timeCreation, bool *editEnable)
  {
  addUndo( new SdUndoRecordProjectItem( item, title, author, timeCreation, editEnable ) );
  }






void SdUndo::string2(QString *str1, QString *str2)
  {
  addUndo( new SdUndoRecordString2( str1, str2 ) );
  }




void SdUndo::stringMapItem(SdStringMap *assoc, const QString key)
  {
  addUndo( new SdUndoRecordStringMapItem( assoc, key ) );
  }




void SdUndo::stringMap(SdStringMap *assoc)
  {
  addUndo( new SdUndoRecordStringMap(assoc) );
  }




void SdUndo::padAssociation(QString *id, QString *srcName, SdPadMap *srcMap)
  {
  addUndo( new SdUndoRecordPadAssociation( id, srcName, srcMap ) );
  }





void SdUndo::road(SdPropInt *width, SdPoint *p1, SdPoint *p2)
  {
  addUndo( new SdUndoRecordRoad( width, p1, p2 ) );
  }




void SdUndo::via(SdPropString *pad, SdPoint *pos)
  {
  addUndo( new SdUndoRecordVia( pad, pos ) );
  }




void SdUndo::rule(SdRuleBlock *pcbSrc, SdRuleBlockMap *mapSrc)
  {
  addUndo( new SdUndoRecordRules( pcbSrc, mapSrc ) );
  }

void SdUndo::stringList(int *val, QStringList *list)
  {
  addUndo( new SdUndoRecordStringList( val, list ) );
  }





void SdUndo::polygon(SdPropPolygon *propSource, SdPointList *regionSource, SdPolyWindowList *windowsSource)
  {
  addUndo( new SdUndoRecordPolygon( propSource, regionSource, windowsSource ) );
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

