/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Interface language definition
  Table of supported interface languages
*/
#include "SdLanguage.h"


SdLanguage::SdLanguage(const QString title, const QString id) :
  mTitle(title),
  mId(id)
  {
  }



//Table of supported languages
SdLanguage::SdLanguageList SdLanguage::languageList()
  {
  SdLanguageList list;
  //When appending language then insert its desription here
  list.append( SdLanguage(QStringLiteral("English"), QStringLiteral("en")) );
  list.append( SdLanguage(QStringLiteral("Русский"), QStringLiteral("ru")) );

  return list;
  }
