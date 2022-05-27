/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Fragment insertion in symbol mode. With this we can select and insert symbol
  from library when editing current symbol. So our symbol can be as complex
  mix of other symbols and our graphics
*/
#include "SdModeSymbolFragment.h"
#include "windows/SdDGetObject.h"
#include "windows/SdWEditorGraph.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdObjectFactory.h"

SdModeSymbolFragment::SdModeSymbolFragment(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeSelect( editor, obj )
  {

  }


void SdModeSymbolFragment::activate()
  {
  SdModeSelect::activate();
  setStep(smPaste);
  getSymbolFragment();
  }


void SdModeSymbolFragment::enterPoint( SdPoint p )
  {
  SdModeSelect::enterPoint( p );
  getSymbolFragment();
  }



void SdModeSymbolFragment::cancelPoint( SdPoint )
  {
  getSymbolFragment();
  }

int SdModeSymbolFragment::getIndex() const
  {
  return MD_SYMBOL_FRAGMENT;
  }







QString SdModeSymbolFragment::getStepHelp() const
  {
  return QObject::tr( "Enter position of insertion for fragment" );
  }




QString SdModeSymbolFragment::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeSymbolFragment.htm" );
  }



QString SdModeSymbolFragment::getStepThema() const
  {
  return QStringLiteral( MODE_HELP "ModeSymbolFragment.htm#Insertion" );
  }




void SdModeSymbolFragment::getSymbolFragment()
  {
  cancelPaste();
  //Query from user symbol for section
  QString uid = SdDGetObject::getObjectUid( dctSymbol, QObject::tr("Select symbol for insertion"), mEditor );
  //If user selected symbol then append it as section
  if( !uid.isEmpty() ) {
    //Symbol selected
    mPastePrj = new SdProject{};
    SdPItemSymbol *symbol = sdObjectOnly<SdPItemSymbol>( SdObjectFactory::extractObject( uid, false, mEditor ) );
    if( symbol != nullptr ) {
      mPastePrj->insertChild( symbol, mPastePrj->getUndo() );
      //At now we select all content of symbol
      mPaste.selectAll( symbol, dctSymbolObjects );
      mPaste.setOrigin( symbol->getOrigin() );
      setStep(smPaste);
      return;
      }
    }
  cancelMode();
  }
