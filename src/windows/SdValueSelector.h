/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog value selector. Contains a lot of some selectors for individual component type.
*/
#ifndef SDVALUESELECTOR_H
#define SDVALUESELECTOR_H

#include "library/SdStringMap.h"
#include <QWidget>


struct SdValueSelector
  {
    //Test if selector description present in map and appropriate selector available
    static bool        isSelectorAvailable( const SdStringMap &map );

    //Get available selectors
    static QStringList availableSelectors();

    //Get short selector description
    static QString     shortSelectorDescription( const QString selector );

    //Get selector help page
    static QString     selectorHelpPage( const QString selector );

    //Call appropriate selector if it available for this param.
    //Change params in place
    static bool        select(SdStringMap *map, QWidget *parent );
  };

#endif // SDVALUESELECTOR_H
