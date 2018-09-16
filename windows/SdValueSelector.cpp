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
#include "SdConfig.h"
#include "SdValueSelector.h"
#include "SdDRowValue.h"
#include <functional>
#include <QMap>
#include <QMessageBox>

typedef std::function<bool(SdStringMap &map, QWidget *parent)> SdSelectorFunction;

struct SdSelectorDescriptor {
    SdSelectorFunction mFunction;         //Selector function
    QString            mHelpFile;         //Name of help file
    QString            mShortDescription; //Short description of selector
  };

static QMap<QString,SdSelectorDescriptor> sdSelectorDescriptorTable;

static void appendDescriptor( const QString name, SdSelectorFunction fun, const QString helpFile, const QString shortDescription ) {
  //Desrciptor for selecotr
  SdSelectorDescriptor d;
  //Fill descriptor fields
  d.mFunction         = fun;
  d.mHelpFile         = helpFile;
  d.mShortDescription = shortDescription;
  //Insert descriptor into table
  sdSelectorDescriptorTable.insert( name, d );
  }


//Build selector table if it not builded already
static void buildSelectorTable() {
  if( sdSelectorDescriptorTable.count() == 0 ) {
    appendDescriptor( QString("resistor"), [] (SdStringMap &map, QWidget *parent ) ->bool {
      SdDRowValue rv( map, sdValueModifierOm, parent );
      return rv.exec();
      },
      QString("SelectorResistor.htm"),
      QObject::tr("Allow select value of resistors")
      );

    appendDescriptor( QString("condensator"), [] (SdStringMap &map, QWidget *parent ) ->bool {
      SdDRowValue rv( map, sdValueModifierFarad, parent );
      return rv.exec();
      return false;
      },
      QString("SelectorCondensator.htm"),
      QObject::tr("Allow select value of condensator")
      );

    }
  }




//Test if selector description present in map and appropriate selector available
bool SdValueSelector::isSelectorAvailable(const SdStringMap &map)
  {
  buildSelectorTable();
  if( !map.contains( stdParamValueSelector ) )
    return false;
  QString sel = map.value( stdParamValueSelector );
  return sdSelectorDescriptorTable.contains( sel );
  }




//Get available selectors
QStringList SdValueSelector::availableSelectors()
  {
  buildSelectorTable();
  return sdSelectorDescriptorTable.keys();
  }




//Get short selector description
QString SdValueSelector::shortSelectorDescription(const QString selector)
  {
  buildSelectorTable();
  return sdSelectorDescriptorTable.value(selector).mShortDescription;
  }




//Get selector help page
QString SdValueSelector::selectorHelpPage(const QString selector)
  {
  buildSelectorTable();
  return sdSelectorDescriptorTable.value(selector).mHelpFile;
  }








//Call appropriate selector if it available for this param.
//Change params in place
bool SdValueSelector::select(SdStringMap &map, QWidget *parent)
  {
  //Build selector table if needed
  buildSelectorTable();

  //Test if selector setuped in map
  if( !map.contains( stdParamValueSelector ) )
    return false;

  //Selector setuped. Test if selector available
  QString sel = map.value( stdParamValueSelector );
  if( !sdSelectorDescriptorTable.contains( sel ) )
    return false;

  //Selector available. Execute
  return sdSelectorDescriptorTable.value(sel).mFunction( map, parent );
  }
