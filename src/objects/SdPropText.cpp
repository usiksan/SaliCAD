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

SdPropText::SdPropText()
  {

  }

void SdPropText::operator =(const SdPropText &prop)
  {
  mSize   = prop.mSize;   //Размер текста
  mDir    = prop.mDir;    //Направление
  mLayer  = prop.mLayer;  //Слой
  mFont   = prop.mFont;   //Идентификатор шрифта
  mHorz   = prop.mHorz;   //Выравнивание горизонтальное, вертикальное и зеркальность
  mVert   = prop.mVert;
  mMirror = prop.mMirror;
  }

void SdPropText::clear() {
  mSize.clear();   //Размер текста
  mDir.clear();    //Направление
  mLayer.clear();  //Слой
  mFont.clear();   //Идентификатор шрифта
  mHorz.clear();   //Выравнивание горизонтальное, вертикальное и зеркальность
  mVert.clear();
  mMirror.clear();
  }

bool SdPropText::match( SdPropText const &prop ) {
  return mSize.match( prop.mSize ) &&
         mDir.match( prop.mDir ) &&
         mLayer.match( prop.mLayer ) &&
         mFont.match( prop.mFont ) &&
         mHorz.match( prop.mHorz ) &&
         mVert.match( prop.mVert ) &&
      mMirror.match( prop.mMirror);
  }




void SdPropText::json(const QString prefix, SvJsonWriter &js) const
  {
  mLayer.json( prefix + QStringLiteral("TextLayer"), js ); //Слой
  mSize.json( prefix + QStringLiteral("TextSize"), js );   //Размер текста
  mFont.json( prefix + QStringLiteral("TextFont"), js );   //Идентификатор шрифта

  //Convert attributes to string
  QString attr;
  if( mDir == da270 ) attr.append( QChar('3') );
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

//  mDir.json( prefix + QStringLiteral("TextDir"), js );     //Направление
//  mHorz.json( prefix + QStringLiteral("TextHorz"), js );   //Выравнивание горизонтальное, вертикальное и зеркальность
//  mVert.json( prefix + QStringLiteral("TextVert"), js );
//  mMirror.json( prefix + QStringLiteral("TextMirror"), js );
  }



void SdPropText::json(const QString prefix, const SvJsonReader &js)
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


void SdPropText::assign( SdPropText const &p ) {
  operator = ( p );
  }


void SdPropText::append( SdPropText const &prop ) {
  mSize.append( prop.mSize );
  mDir.append( prop.mDir );
  mLayer.append( prop.mLayer );
  mFont.append( prop.mFont );
  mHorz.append( prop.mHorz );
  mVert.append( prop.mVert );
  mMirror.append( prop.mMirror);
  }



void SdPropText::saveState(SdPropTextState *dst)
  {
  dst->mLayer = mLayer.layer();
  dst->mSize  = mSize.getValue();
  dst->mDir   = mDir.getValue();
  dst->mFont  = mFont.getValue();
  dst->mHorz  = mHorz.getValue();
  dst->mVert  = mVert.getValue();
  dst->mMirror= mMirror.getValue();
  }




void SdPropText::swapState(SdPropTextState *src)
  {
  src->mLayer  = mLayer.swap( src->mLayer );
  src->mSize   = mSize.swap( src->mSize );
  src->mDir    = mDir.swap( src->mDir );
  src->mFont   = mFont.swap( src->mFont );
  src->mHorz   = mHorz.swap( src->mHorz );
  src->mVert   = mVert.swap( src->mVert );
  src->mMirror = mMirror.swap( src->mMirror );
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
