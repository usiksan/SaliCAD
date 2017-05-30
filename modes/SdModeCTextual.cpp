/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Common mode for all with enter text
*/
#include "SdModeCTextual.h"
#include "objects/SdEnvir.h"
#include "objects/SdPropLine.h"
#include "objects/SdPropText.h"
#include "windows/SdWCommand.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWEditorGraph.h"
#include <QClipboard>
#include <QApplication>
#include <QDebug>

SdModeCTextual::SdModeCTextual(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mPropText(0),     //Text properties
  mString(),       //Work string
  mPos(0),          //Char cursor position
  mStartSel(0),     //Char start select position
  mStopSel(0),      //Char stop select position
  mSelectRect(),   //Selection region
  mCursorP1(),     //Cursor position p1. Cursor is vertical line from p1 to p2 at insert position
  mCursorP2(),    //Cursor position p2
  mShift(false),
  mControl(false)
  {

  }


void SdModeCTextual::keyDown(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_Shift :
      mShift = true;
      break;
    case Qt::Key_Control :
      mControl = true;
      break;
    case Qt::Key_Left :
      movePos( mPos - 1, mShift );
      break;
    case Qt::Key_Right :
      movePos( mPos + 1, mShift );
      break;
    case Qt::Key_Home :
      movePos( 0, mShift );
      break;
    case Qt::Key_End :
      movePos( mString.length(), mShift );
      break;
    case Qt::Key_Backspace :
      if( isSelectionPresent() )
        delSelected();
      movePos( mPos - 1, true );
      delSelected();
      break;
    case Qt::Key_Delete :
      if( mShift )
        clipboardCut();
      else {
        if( isSelectionPresent() )
          delSelected();
        movePos( mPos + 1, true );
        delSelected();
        }
      break;
    case Qt::Key_Insert :
      if( mShift ) clipboradPaste();
      else if( mControl ) clipboardCopy();
      break;
    case Qt::Key_Escape :
      cancelEdit();
      break;
    case Qt::Key_Return :
      applyEdit();
      break;
    default :
      //qDebug() << Q_FUNC_INFO << key;
      if( ch.isPrint() ) {
        //Insert character
        if( isSelectionPresent() )
          delSelected();
        mString.insert( mPos, ch );
        mPos++;
        mStartSel = mStopSel = mPos;
        update();
        }
    }
  }




void SdModeCTextual::keyUp(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_Shift :
      mShift = false;
      break;
    case Qt::Key_Control :
      mControl = false;
      break;
    }
  }




int SdModeCTextual::getCursor() const
  {
  return CUR_TEXT;
  }




//Move cursor (insert) posiotion
void SdModeCTextual::movePos(int pos, bool sel)
  {
  //Check bounds of new position
  //Correct if need
  if( pos < 0 ) pos = 0;
  if( pos > mString.length() ) pos = mString.length();
  if( sel ) {
    //Need selection
    if( isSelectionPresent() ) {
      //Selection already presents
      if( mPos == mStartSel ) {
        mPos = pos;
        if( mPos > mStopSel ) {
          mStartSel = mStopSel;
          mStopSel = mPos;
          }
        else mStartSel = mPos;
        }
      else {
        mPos = pos;
        if( mPos < mStartSel ) {
          mStopSel = mStartSel;
          mStartSel = mPos;
          }
        else mStopSel = mPos;
        }
      }
    else {
      mPos = pos;
      if( mPos > mStartSel ) mStopSel = mPos;
      else mStartSel = mPos;
      }
    }
  else {
    mStartSel = mStopSel = mPos = pos;
    }
  update();
  }




//Define selection
void SdModeCTextual::setSel(int start, int stop)
  {
  if( start >= 0 && start <= mString.length() && stop >= start && stop <= mString.length() ) {
    mStartSel = start;
    mPos = mStopSel = stop;
    update();
    }
  }




//Insert text in current position with selection param
void SdModeCTextual::insertText(const QString str, bool sel)
  {
  //If selection present - delete it
  if( isSelectionPresent() )
    delSelected();

  if( !str.isEmpty() ) {
    //Make insertion
    mString.insert( mPos, str );

    //End of selection in any chois is equal position after insertion
    mStopSel = mPos += str.length();

    //If no selection, then startSel equal stopSel
    if( !sel )
      mStartSel = mStopSel;
    }

  update();
  }



//Clear old text and insert new one with selection param
void SdModeCTextual::setText(const QString str, bool sel)
  {

  //At first clear old text
  mString.clear();
  mPos = mStopSel = mStartSel = 0;

  //Insert new text with selection param
  insertText( str, sel );
  }




//Delete selection if present
void SdModeCTextual::delSelected()
  {
  //Check if selection present
  if( isSelectionPresent() ) {
    //Remove selection
    mString.remove( mStartSel, mStopSel - mStartSel );
    mPos = mStopSel = mStartSel;
    update();
    }
  }




void SdModeCTextual::clipboardCopy()
  {
  if( isSelectionPresent() ) {
    QClipboard *clip = QApplication::clipboard();
    clip->setText( mString.mid( mStartSel, mStopSel - mStartSel ) );
    }
  }




void SdModeCTextual::clipboradPaste()
  {
  if( isSelectionPresent() )
    delSelected();

  QClipboard *clip = QApplication::clipboard();
  if( !clip->text().isEmpty() )
    insertText( clip->text(), false );
  }




void SdModeCTextual::clipboardCut()
  {
  clipboardCopy();
  delSelected();
  }






int SdModeCTextual::getPropBarId() const
  {
  return PB_TEXT;
  }




void SdModeCTextual::propGetFromBar()
  {
  if( mPropText ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) {
      tbar->getPropText( mPropText );
      mEditor->setFocus();
      update();
      }
    }
  }




void SdModeCTextual::propSetToBar()
  {
  if( mPropText ) {
    SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
    if( tbar ) {
      tbar->setPropText( mPropText, mEditor->getPPM() );
      }
    }
  }




void SdModeCTextual::enterPoint(SdPoint enter)
  {
  Q_UNUSED(enter)
  applyEdit();
  }




void SdModeCTextual::cancelPoint(SdPoint p)
  {
  Q_UNUSED(p)
  cancelEdit();
  }




void SdModeCTextual::drawText(SdContext *dc)
  {
  if( mPropText ) {
    dc->setFont( *mPropText );
    dc->setPen(0,sdEnvir->getSysColor(scEnter), dltSolid );
    dc->textEx( mPrev, mOverRect, mString,
              mPropText->mDir.getValue(), mPropText->mHorz.getValue(), mPropText->mVert.getValue(),
              mPos, &mCursorP1, &mCursorP2,
              &mSelectRect, mStartSel, mStopSel );

    //Draw cursor as vertical line
    dc->line( mCursorP1, mCursorP2 );

    //Draw selection rect
    if( isSelectionPresent() ) {
      dc->setPen(0,sdEnvir->getSysColor(scEnter), dltDotted );
      dc->rect( mSelectRect );
      }
    }
  }

