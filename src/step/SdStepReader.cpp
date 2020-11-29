#include "SdStepReader.h"

#include <QColor>
#include <QDebug>

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
  QStringList outList; //Param out list
  QString     param;   //Current param
  int level = 0;       //Current level of params. Parameters enclosed in parentheses can be nested
  int index = 0;       //Current char index

  //Blank while open '('
  while( index < line.count() && line.at(index) != QChar('(') ) index++;

  //Continue parsing only if line not empty
  if( index < line.count() ) {
    //Skeep first '('
    index++;
    //Split 'line' into param list only for level = 0, for other levels are the nested lists and treat as sigle param
    while( index < line.count() ) {
      //Next char to analise
      char ch = line.at(index).toLatin1();
      switch( ch ) {
        //Open bracket - shift up level
        case '(' :
          level++;
          break;

        //Close bracket - shift down level or break analise
        case ')' :
          if( level )
            //Level not equals 0 - shift down level
            level--;
          else {
            //Append last accumulated param
            outList.append( param.simplified() );
            //Stop parsing
            index = line.count();
            continue;
            }
          break;

        //Param delimiter. Only for level 0, for other levels - they are nested params and treats as single param
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





bool SdStepParserParamList::parse(const QString &line, const SdStepReader &reader)
  {
  QStringList paramList = reader.parseList(line);
  if( paramList.count() != mParamHandlerList.count() ) {
    qDebug() << "SdStepParserParamList wrong param count" << mItemName;
    return false;
    }
  for( int i = 0; i < paramList.count(); i++ )
    if( !mParamHandlerList.at(i)->parse( paramList.at(i), reader ) )
      return false;

  mAppler();
  return true;
  }


bool SdStepParserParamSkeeper::parse(const QString &line, const SdStepReader &reader)
  {
  Q_UNUSED(line)
  Q_UNUSED(reader)
  return true;
  }


bool SdStepParserAsParamSingleId::parse(const QString &line, const SdStepReader &reader)
  {
  //line is id
  QString param = reader.value( line );
  //From list select variant which match param
  for( auto ref : mReferenceList )
    if( ref->matchName(param) )
      //Matched reference found. Apply it to parse param
      return ref->parse( param, reader );
  return false;
  }



bool SdStepParserAsParamIdList::parse(const QString &line, const SdStepReader &reader)
  {
  QStringList paramIdList = reader.parseList(line);
  for( auto const &id : paramIdList ) {
    QString param = reader.value( id );
    //From list select variant which match param
    bool parsed = false;
    for( auto ref : mReferenceList )
      if( ref->matchName(param) ) {
        //Matched reference found. Apply it to parse param
        if( !ref->parse( param, reader ) ) return false;
        parsed = true;
        break;
        }
    if( !parsed ) {
      //Param not found
      qDebug() << "SdStepParserAsParamIdList" << param;
      return false;
      }
    }
  return true;
  }




bool SdStepParserAsParamString::parse(const QString &line, const SdStepReader &reader)
  {
  Q_UNUSED(reader)
  mSelected = mStrings.indexOf(line);
  return mSelected >= 0;
  }



bool SdStepParserAsParamDouble::parse(const QString &line, const SdStepReader &reader)
  {
  Q_UNUSED(reader)
  bool res;
  mValue = line.simplified().toDouble( &res );
  return res;
  }


bool SdStepParserColorRgb::parse(const QString &line, const SdStepReader &reader)
  {
  QStringList rgb = reader.parseList( line );
  if( rgb.count() == 4 ) {
    double r = rgb.at(1).simplified().toDouble() * 255;
    double g = rgb.at(2).simplified().toDouble() * 255;
    double b = rgb.at(3).simplified().toDouble() * 255;
    mColor = qRgb( static_cast<int>(r), static_cast<int>(g), static_cast<int>(b) );
    return true;
    }
  return false;
  }

QString SdStepParserColorRgb::name() const
  {
  return QStringLiteral("COLOUR_RGB");
  }








bool SdStepParserCartesianPoint::parse(const QString &line, const SdStepReader &reader)
  {
  QStringList paramList = reader.parseList( line );
  if( paramList.count() == 2 ) {
    paramList = reader.parseList( paramList.at(1) );
    if( paramList.count() == 3 ) {
      x = paramList.at(0).simplified().toDouble();
      y = paramList.at(1).simplified().toDouble();
      z = paramList.at(2).simplified().toDouble();
      return true;
      }
    }
  return false;
  }

QString SdStepParserCartesianPoint::name() const
  {
  return QStringLiteral("CARTESIAN_POINT");
  }


QString SdStepParserDirection::name() const
  {
  return QStringLiteral("DIRECTION");
  }


