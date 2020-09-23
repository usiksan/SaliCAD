#include "SdStepReader.h"

SdStepReader::SdStepReader()
  {

  }




bool SdStepReader::read(QIODevice *in)
  {
  bool doData = false;
  while( !in->atEnd() ) {
    QByteArray line = in->readLine().simplified();
    if( doData ) {
      //Check end of data section
      if( line == "ENDSEC;" ) return true;

      //Parse data line
      if( line.length() < 1 ) continue;

      if( line.at(0) != '#' ) continue;

      QByteArrayList equation = line.split('=');
      if( equation.count() != 2 ) continue;

      mStepMap.insert( QString::fromLatin1(equation.at(0).simplified()), QString::fromLatin1(equation.at(1).simplified()) );
      }
    else {
      //Parse header
      if( line == "DATA;" ) doData = true;
      }
    }
  return false;
  }





QStringList SdStepReader::styledItemList() const
  {
  return itemList( QStringLiteral("STYLED_ITEM") );
  }




QStringList SdStepReader::parseList(const QString line) const
  {
  QStringList outList;
  QString     param;
  int level = 0;
  int index = 0;
  //Blank while open '('
  while( index < line.count() && line.at(index) != QChar('(') ) index++;
  if( index < line.count() ) {
    //Skeep first '('
    index++;
    //Split 'line'
    while( index < line.count() ) {
      char ch = line.at(index).toLatin1();
      switch( ch ) {
        case '(' :
          level++;
          break;

        case ')' :
          if( level )
            level--;
          else {
            outList.append( param.simplified() );
            index = line.count();
            continue;
            }
          break;

        case ',' :
          if( level == 0 ) {
            outList.append( param.simplified() );
            param.clear();
            index++;
            continue;
            }
          break;
        }
      param.append( line.at(index) );
      index++;
      }
    }
  return outList;
  }




QStringList SdStepReader::parseListFromValue(const QString &id) const
  {
  return parseList( mStepMap.value(id) );
  }




QStringList SdStepReader::itemList(const QString itemName) const
  {
  QStringList list;
  for( auto item = mStepMap.cbegin(); item != mStepMap.cend(); item++ )
    //If item contents start with itemName then append this item to output list
    if( item.value().startsWith( itemName ) )
      list.append( item.key() );
  return list;
  }
