/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropText.h"
#include "SdJsonIO.h"



//!
//! \brief json Write text properties to JSON with specified prefix
//! \param prefix JSON key prefix for grouped properties
//! \param js     JSON writer object
//!
void SdPropText::json(const QString prefix, SvJsonWriter &js) const
  {
  mLayer.json( prefix + QStringLiteral("TextLayer"), js ); //Слой
  mSize.json( prefix + QStringLiteral("TextSize"), js );   //Размер текста
  mFont.json( prefix + QStringLiteral("TextFont"), js );   //Идентификатор шрифта

  //Convert attributes to string
  QString attr;
  if( mDir == SdPvInt(da270) ) attr.append( QChar('3') );
  else if( mDir == da90 ) attr.append( QChar('1') );
  else if( mDir == da180 ) attr.append( QChar('2') );
  else attr.append( QChar('0') );

  if( mMirror == 0 ) attr.append( QChar('S') );
  else attr.append( QChar('M') );

  if( mHorz == dhjRight ) attr.append( QChar('R') );
  else if( mHorz == dhjCenter ) attr.append( QChar('C') );
  else attr.append( QChar('L') );

  if( mVert == dvjBottom ) attr.append( QChar('B') );
  else if( mVert == dvjMiddle ) attr.append( QChar('M') );
  else attr.append( QChar('T') );

  js.jsonString( prefix + QStringLiteral("TextDirMirHorVer"), attr );
  }



//!
//! \brief json Read text properties from JSON with specified prefix
//! \param prefix JSON key prefix for grouped properties
//! \param js     JSON reader object
//!
void SdPropText::json(const QString prefix, const SdJsonReader &js)
  {
  mLayer.json( prefix + QStringLiteral("TextLayer"), js ); //Слой
  mSize.json( prefix + QStringLiteral("TextSize"), js );   //Размер текста
  mFont.json( prefix + QStringLiteral("TextFont"), js );   //Идентификатор шрифта
  if( js.contains( prefix + QStringLiteral("TextDirMirHorVer") ) ) {
    //Parse attributes from string
    QString attr;
    js.jsonString( prefix + QStringLiteral("TextDirMirHorVer"), attr );

    if( attr.length() == 4 ) {
      if( attr.at(0) == QChar('1') ) mDir = da90;
      else if( attr.at(0) == QChar('2') ) mDir = da180;
      else if( attr.at(0) == QChar('3') ) mDir = da270;
      else mDir = da0;

      if( attr.at(1) == QChar('M') ) mMirror = 1;
      else mMirror = 0;

      if( attr.at(2) == QChar('C') ) mHorz = dhjCenter;
      else if( attr.at(2) == QChar('R') ) mHorz = dhjRight;
      else mHorz = dhjLeft;

      if( attr.at(3) == QChar('M') ) mVert = dvjMiddle;
      else if( attr.at(3) == QChar('B') ) mVert = dvjBottom;
      else mVert = dvjTop;

      }
    }
  else {
    //Unpacked variant
    mDir.json( prefix + QStringLiteral("TextDir"), js );     //Направление
    mHorz.json( prefix + QStringLiteral("TextHorz"), js );   //Выравнивание горизонтальное, вертикальное и зеркальность
    mVert.json( prefix + QStringLiteral("TextVert"), js );
    mMirror.json( prefix + QStringLiteral("TextMirror"), js );
    }
  }




//!
//! \brief swap Swap all text properties with another instance
//! \param other Other SdPropText object to swap with
//!
void SdPropText::swap(SdPropText &other)
  {
  mLayer.swap( other.mLayer );
  mSize.swap( other.mSize );
  mDir.swap( other.mDir );
  mFont.swap( other.mFont );
  mHorz.swap( other.mHorz );
  mVert.swap( other.mVert );
  mMirror.swap( other.mMirror );
  }





//Parse src string and find numeric substring. Numeric substring converted into int, int incremented
//and convert to substring. Substring, at end, inserted into src string and return result
//Example: src=PORT18CDF will result PORT19CDF
QString nextText(const QString src)
  {
  if( src.isEmpty() )
    return src;
  int end = 0, begin = 0;
  int p = src.length() - 1;
  //Find number in string from end
  while( p >= 0 && !src.at(p).isDigit() ) p--;
  if( p < 0 )
    //no digits in src
    return src;
  if( src.at(p).isDigit() ) end = p+1;
  do {
    begin = p;
    p--;
    }
  while( p >= 0 && src.at(p).isDigit() );

  if( end ) {
    //Number present
    int i = src.mid(begin,end-begin).toInt(); //Get numerical value
    i++;                                      //Increment, so next
    QString res;
    if( begin ) res = src.left(begin);
    res.append( QString::number(i) );
    if( end < src.length() ) res.append( src.mid(end) );
    return res;
    }
  return src;
  }
