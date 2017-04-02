/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdTextProp.h"

SdTextProp::SdTextProp()
  {

  }

void SdTextProp::operator =(const SdTextProp &prop)
  {
  mSize   = prop.mSize;   //Размер текста
  mDir    = prop.mDir;    //Направление
  mLayer  = prop.mLayer;  //Слой
  mFont   = prop.mFont;   //Идентификатор шрифта
  mHorz   = prop.mHorz;   //Выравнивание горизонтальное, вертикальное и зеркальность
  mVert   = prop.mVert;
  mMirror = prop.mMirror;
  }

void SdTextProp::clear() {
  mSize.clear();   //Размер текста
  mDir.clear();    //Направление
  mLayer.clear();  //Слой
  mFont.clear();   //Идентификатор шрифта
  mHorz.clear();   //Выравнивание горизонтальное, вертикальное и зеркальность
  mVert.clear();
  mMirror.clear();
  }

bool SdTextProp::match( SdTextProp const &prop ) {
  return mSize.match( prop.mSize ) &&
         mDir.match( prop.mDir ) &&
         mLayer.match( prop.mLayer ) &&
         mFont.match( prop.mFont ) &&
         mHorz.match( prop.mHorz ) &&
         mVert.match( prop.mVert ) &&
         mMirror.match( prop.mMirror);
  }


void SdTextProp::assign( SdTextProp const &p ) {
  operator = ( p );
  }


void SdTextProp::append( SdTextProp const &prop ) {
  mSize.append( prop.mSize );
  mDir.append( prop.mDir );
  mLayer.append( prop.mLayer );
  mFont.append( prop.mFont );
  mHorz.append( prop.mHorz );
  mVert.append( prop.mVert );
  mMirror.append( prop.mMirror);
  }
