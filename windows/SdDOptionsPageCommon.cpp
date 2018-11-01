/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Some common params:
    - interface language
*/
#include "SdDOptionsPageCommon.h"
#include "SdLanguage.h"
#include "SdConfig.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>


SdDOptionsPageCommon::SdDOptionsPageCommon(QWidget *parent) :
  QWidget(parent)
  {
  QSettings s;
  setWindowTitle( tr("Paths") );

  QGridLayout *grid = new QGridLayout();

  grid->addWidget( new QLabel(tr("Interface language:")), 0, 0 );
  grid->addWidget( mLanguage = new QComboBox(), 0, 1 );


  QString defLang = s.value( SDK_LANGUAGE ).toString();
  QString defLangTitle;

  //Get supported language list and fill language table
  SdLanguage::SdLanguageList list = SdLanguage::languageList();
  for( const SdLanguage &lang : list ) {
    //Insert language title to combo box
    mLanguage->addItem( lang.mTitle, lang.mId );
    if( defLang == lang.mId )
      defLangTitle = lang.mTitle;
    }

  //Set current language
  if( defLangTitle.isEmpty() )
    defLangTitle = list.at(0).mTitle;
  mLanguage->setCurrentText( defLangTitle );

  setLayout( grid );
  }



void SdDOptionsPageCommon::accept()
  {
  QString lang = mLanguage->currentData().toString();
  QSettings s;
  s.setValue( SDK_LANGUAGE, lang );
  }

