#include "IgesReader.h"

#include <QFile>

IgesReader::IgesReader()
  {

  }

bool IgesReader::scanFile(const QString &fname)
  {
  //Open file
  mFile.setFileName( fname );
  if( mFile.open(QIODevice::ReadOnly) ) {
    //Read next line
    while( nextLine() ) {

      //Test if it is termination
      if( mLine.at(72) == 'T' )
        break;

      //Scan current line
      if( !scanLine() )
        return false;

      }
    //Successfull scan complete
    return true;
    }
  //Fail to open file
  return false;
  }




bool IgesReader::parse()
  {
  for( auto it = mDirectoryEntryMap.begin(); it != mDirectoryEntryMap.end(); it++ )
    if( !it.value()->parse( this ) ) return false;
  return true;
  }



void IgesReader::setLine(const QByteArray &line)
  {
  mLine = line;
  mIndex = 0;
  }

bool IgesReader::paramInt(int &val)
  {
  if( !scanInt( val, 0, false ) ) return false;
  if( mLine.at(mIndex) != mParametrDelimiter && mLine.at(mIndex) != mRecordDelimiter ) return false;
  mIndex++;
  return true;
  }



bool IgesReader::paramReal(double &val)
  {
  if( !scanReal( val, 0.0, false ) ) return false;
  if( mIndex > mLine.count() || (mLine.at(mIndex) != mParametrDelimiter && mLine.at(mIndex) != mRecordDelimiter) ) return false;
  mIndex++;
  return true;
  }




bool IgesReader::nextLine()
  {
  //Get next line
  mLine = mFile.readLine();

  //If line size not equal 80 characters we stop
  if( mLine.size() != 81 )
    return false;

  //Decode line index
  mLineIndex = mLine.mid( 73 ).simplified().toInt();
  mIndex = 0;

  return true;
  }



bool IgesReader::scanLine()
  {
  //Scan different sections
  switch( mLine.at(72) ) {
    case 'S' : return scanStart();
    case 'G' : return scanGlobal();
    case 'D' : return scanDirectoryEntry();
    case 'P' : return scanParametrData();
    default: return false;
    }
  }



bool IgesReader::scanReal(double &val, double defVal, bool thereDef)
  {
  bool ok = true;
  QByteArray ar;
  while( mIndex < mLine.count() && mLine.at(mIndex) != mParametrDelimiter && mLine.at(mIndex) != mRecordDelimiter )
    ar.append( mLine.at(mIndex++) );
  if( ar.isEmpty() ) {
    if( !thereDef ) return false;
    val = defVal;
    }
  else
    val = ar.toDouble( &ok );
  return ok;
  }









bool IgesReader::scanChars(QByteArray &dest, const QByteArray &def, bool thereDef)
  {
  dest.clear();
  //Test for default string (i.e.empty)
  if( !QChar(mLine.at(mIndex)).isDigit() ) {
    if( !thereDef ) return false;
    dest = def;
    }
  else {
    //Get symbol count
    int countLastPos = mLine.indexOf( 'H', mIndex ) - mIndex;
    if( countLastPos < 0 ) return false;
    int count = mLine.mid( mIndex, countLastPos ).toInt();
    mIndex += countLastPos + 1;
    dest.reserve( count );
    for( int i = 0; i < count; i++ ) {
      if( mIndex >= mLine.count() ) return false;
      dest.append( mLine.at(mIndex++) );
      }
    }
  return true;
  }






bool IgesReader::scanParametrDelimiter()
  {
  return mLine.at(mIndex++) == mParametrDelimiter;
  }



bool IgesReader::scanRecordDelimiter()
  {
  return mLine.at(mIndex++) == mRecordDelimiter;
  }




bool IgesReader::scanInt(int &val, int defVal, bool thereDef)
  {
  bool ok = true;
  QByteArray ar;
  //Prefix blank
  while( mIndex < mLine.count() && mLine.at(mIndex) == ' ' ) mIndex++;
  if( mLine.at(mIndex) == '+' || mLine.at(mIndex) == '-' )
    ar.append( mLine.at(mIndex++) );
  while( mIndex < mLine.count() && mLine.at(mIndex) == ' ' ) mIndex++;
  while( mIndex < mLine.count() && QChar(mLine.at(mIndex)).isDigit() )
    ar.append( mLine.at(mIndex++) );
  while( mIndex < mLine.count() && mLine.at(mIndex) == ' ' ) mIndex++;
  if( ar.isEmpty() ) {
    if( !thereDef ) return false;
    val = defVal;
    }
  else
    val = ar.toInt( &ok );
  return ok;
  }




bool IgesReader::scanStart()
  {
  //Do nothing
  return true;
  }



bool IgesReader::scanGlobal()
  {
  QByteArray chars;

  //1 Get parametr delimiter
  if( !scanChars( chars, ",", true ) ) return false;
  mParametrDelimiter = chars.at(0);
  if( !scanParametrDelimiter() ) return false;

  //2 Get record delimiter
  if( !scanChars( chars, ";", true ) ) return false;
  mRecordDelimiter = chars.at(0);
  if( !scanParametrDelimiter() ) return false;

  //Append all Global lines
  QByteArray line = mLine.left(72);
  while( !line.simplified().endsWith(mRecordDelimiter) ) {
    if( !nextLine() || mLine.at(72) != 'G' ) return false;
    line += mLine.left(72);
    }

  //All parts of Global readed repeate scan
  setLine( line );

  //1 Get parametr delimiter
  if( !scanChars( chars, ",", true ) ) return false;
  mParametrDelimiter = chars.at(0);
  if( !scanParametrDelimiter() ) return false;

  //2 Get record delimiter
  if( !scanChars( chars, ";", true ) ) return false;
  mRecordDelimiter = chars.at(0);
  if( !scanParametrDelimiter() ) return false;

  //3 Product identification
  if( !scanChars( chars, "", false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //4 File name
  if( !scanChars( chars, "", false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //5 Native system ID
  if( !scanChars( chars, "", false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //6 Preprocessor version
  if( !scanChars( chars, "", false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  int tmp;
  //7 Number of binary bits for integer representation
  if( !scanInt( tmp, 0, false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //8 Maximum power often representable in a single-precision floating point number on the sending system
  if( !scanInt( tmp, 0, false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //9 Number of significant digits in a single-precision floating point number on the sending system
  if( !scanInt( tmp, 0, false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //10 Maximum power of ten representable in a double-precision floating point number on the sending system
  if( !scanInt( tmp, 0, false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //11 Number of significant digits in a double-precision floating point number on the sending system
  if( !scanInt( tmp, 0, false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //12 Product identification for the receiving system
  if( !scanChars( chars, "", true ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //13 Model space scale
  if( !scanReal( mScale, 1.0, true ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //14 Units Flag
  if( !scanInt( tmp, 1, true ) ) return false;
  if( !scanParametrDelimiter() ) return false;
  //flag Model Units
  // 1   Inches (default)
  // 2   Millimeters
  // 3   (See Parameter 15 for name of units)
  // 4   Feet
  // 5   Miles
  // 6   Meters
  // 7   Kilometers
  // 8   Mils (i.e., 0.001 inch)
  // 9   Microns
  //10   Centimeters
  //11   Microinches
  double factors[12] = { 1.0, 25.4, 1.0, 304.8, 1609344, 1000, 1000000, 0.0254, 0.001, 10, 0.0000254 };
  mUnitFactor = factors[tmp];

  //15 Units Name.
  if( !scanChars( chars, "", true ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //16 Maximum number of line weight gradations.
  if( !scanInt( mLineWeightGradations, 1, true ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //17 Width of maximum line weight in units.
  if( !scanReal( mLineMaxWeight, 1.0, false ) ) return false;
  if( !scanParametrDelimiter() ) return false;
  mLineMaxWeight *= mUnitFactor;

  //18 Date and time of exchange file generation
  if( !scanChars( chars, "", false ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //19 Minimum user-intended resolution or granularity of the model in units specified by Parameter 14
  if( !scanReal( mMinimumResolution, 1.0, false ) ) return false;
  if( !scanParametrDelimiter() ) return false;
  mMinimumResolution *= mUnitFactor;

  //20 Approximate maximum coordinate value occurring in the model in units specified by Parameter 14
  if( !scanReal( mMaximumCoordinate, 0.0, true ) ) return false;
  if( !scanParametrDelimiter() ) return false;
  mMaximumCoordinate *= mUnitFactor;

  //21 Name of author
  if( !scanChars( chars, "", true ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //22 Author’s organization
  if( !scanChars( chars, "", true ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //23 Flag value corresponding to the version of the Specification to which this file complies
  if( !scanInt( tmp, 11, true ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //24 Flag value corresponding to the drafting standard to which this file complies, if any
  if( !scanInt( tmp, 0, true ) ) return false;
  if( !scanParametrDelimiter() ) return false;

  //25 Date and time the model was created or last modified, in same format as field 18
  if( !scanChars( chars, "", true ) ) return false;
  return true;
//  if( !scanParametrDelimiter() ) return false;

//  //26 Descriptor indicating application protocol, application subset, Mil-specification, or user-defined protocol or subset, if any
//  if( !scanChars( chars, "", true ) ) return false;
//  return scanRecordDelimiter();
  }



bool IgesReader::scanDirectoryEntry()
  {
  //First line
  //Entity type
  int type = mLine.left(8).simplified().toInt();
  IgesEntity *entity = IgesEntity::build( type );
  mDirectoryEntryMap.insert( mLineIndex, entity );

  for( int i = 1; i < 9; i++ )
    entity->setField( i + 1, mLine.mid( i * 8, 8 ) );


  //Second line
  if( !nextLine() )
    return false;

  if( mLine.at(72) != 'D' || mLine.left(8).simplified().toInt() != type )
    return false;

  for( int i = 1; i < 9; i++ )
    entity->setField( i + 11, mLine.mid( i * 8, 8 ) );

  return true;
  }



bool IgesReader::scanParametrData()
  {
  QByteArray data;
  int lineIndex = mLineIndex;
  while( !mLine.left(64).simplified().endsWith( mRecordDelimiter ) ) {
    data.append( mLine.left(64) );

    if( !nextLine() )
      return false;

    if( mLine.at(72) != 'P' )
      return false;
    }
  //Finish
  data.append( mLine.left(64) );

  //Append parameter data to list
  mParameterDataMap.insert( lineIndex, data );

  return true;
  }