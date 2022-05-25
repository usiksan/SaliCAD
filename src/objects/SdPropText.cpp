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

void SdPropText::write(const QString prefix, QJsonObject &obj) const
  {
  mLayer.write( prefix + QStringLiteral("TextLayer"), obj ); //Слой
  mSize.write( prefix + QStringLiteral("TextSize"), obj );   //Размер текста
  mDir.write( prefix + QStringLiteral("TextDir"), obj );     //Направление
  mFont.write( prefix + QStringLiteral("TextFont"), obj );   //Идентификатор шрифта
  mHorz.write( prefix + QStringLiteral("TextHorz"), obj );   //Выравнивание горизонтальное, вертикальное и зеркальность
  mVert.write( prefix + QStringLiteral("TextVert"), obj );
  mMirror.write( prefix + QStringLiteral("TextMirror"), obj );
  }




void SdPropText::read(const QString prefix, const QJsonObject obj)
  {
  mLayer.read( prefix + QStringLiteral("TextLayer"), obj ); //Слой
  mSize.read( prefix + QStringLiteral("TextSize"), obj );   //Размер текста
  mDir.read( prefix + QStringLiteral("TextDir"), obj );     //Направление
  mFont.read( prefix + QStringLiteral("TextFont"), obj );   //Идентификатор шрифта
  mHorz.read( prefix + QStringLiteral("TextHorz"), obj );   //Выравнивание горизонтальное, вертикальное и зеркальность
  mVert.read( prefix + QStringLiteral("TextVert"), obj );
  mMirror.read( prefix + QStringLiteral("TextMirror"), obj );
  }



void SdPropText::json(const QString prefix, SvJsonWriter &js) const
  {
//  js.jsonValue( prefix + QStringLiteral("TextSize"), mSize );
//  js.jsonValue( prefix + QStringLiteral("TextDir"), mDir );

  write( prefix, js.object() );
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
